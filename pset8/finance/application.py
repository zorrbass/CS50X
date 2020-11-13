import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# regular expressions. used to determain the validity of of password
import re

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # query all the transaction. find all stocks of the current user which have a positive amount
    quotes = db.execute("SELECT quote, SUM(shares) FROM (SELECT user_id, quote, shares FROM transactions WHERE NOT quote='CA$H') WHERE user_id = :user_id GROUP BY quote"
                        , user_id=session["user_id"])

    # check the current cash balance
    cash_re = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
    cash = float(cash_re[0]["cash"])

    leng = len(quotes)

    # define a dictionarry of lists that can store all the info that need to be passed on to the html
    index_dict = {
        "Symbol": [],
        "Name": [],
        "Shares": [],
        "Price": [],
        "Total": [],
    }

    total=cash
    for index in range(len(quotes)):

        if quotes[index]['SUM(shares)'] == 0:
            leng -= 1
        else:
            # check the API for the latest data
            result = lookup(quotes[index]['quote'])

            # fill the dictionary with the respectife data
            index_dict["Symbol"].append(quotes[index]['quote'])
            index_dict["Name"].append(result["name"])
            index_dict["Shares"].append(int(quotes[index]['SUM(shares)']))
            index_dict["Price"].append(usd(result["price"]))
            index_dict["Total"].append(usd(float(result["price"])*float(quotes[index]['SUM(shares)'])))

            # add the amount of each position to the total value of the portfolio
            total += float(result["price"])*float(quotes[index]['SUM(shares)'])

    # return all data back to the html
    return render_template("index.html", index_dict=index_dict, leng=leng, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # If the page is requestet from a button click that posts data to be used
    if request.method == "POST":

        # Ensure a symbol was entered
        if not request.form.get("symbol"):
            return apology("must enter a symbol", 403)

        # Ensure a symbol was entered
        if int(request.form.get("shares")) < 0:
            return apology("must enter a positive number in shares", 403)

        # save API return in a variable (symbol, name, price)
        result = lookup(request.form.get("symbol"))

        # when the API looup funktion returns no data
        if not result:
            return apology("Ticker not found", 403)

        # number of shares to be bought
        shares = request.form.get("shares")

        total = float(result["price"]) * float(shares)

        # get the current available cash
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        current_cash = rows[0]["cash"]

        # check if cash is suficiant to buy the stocks
        if current_cash < total:
            return apology("not enough money", 403)
        else:
            # if so isert the new transation into the database
            db.execute("INSERT INTO transactions (user_id, quote, shares, price) VALUES (:user_id, :quote, :shares, :price)"
                        , user_id=session["user_id"], quote=request.form.get("symbol").upper()
                        , shares=request.form.get("shares"), price=float(result["price"]))

            # and update the current available cash
            new_cash = rows[0]["cash"] - total
            db.execute("UPDATE users SET cash=:cash WHERE id = :id",cash=new_cash, id=session["user_id"])

        # then go back to the index page
        return redirect("/")
    else:
        # if it was opend via a link, simply render the page.
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # query the transaction table for any transaction of the current user
    quotes = db.execute("SELECT quote, shares, price, date FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])

    leng = len(quotes)

    # define a dictionary of lists that will be forwarded to the html
    history_dict = {
        "Symbol": [],
        "Shares": [],
        "Price": [],
        "Date": [],
    }

    # fill with data
    for index in range(len(quotes)):
        history_dict["Symbol"].append(quotes[index]['quote'])
        history_dict["Shares"].append(int(quotes[index]['shares']))
        history_dict["Price"].append(usd(quotes[index]['price']))
        history_dict["Date"].append(quotes[index]['date'])

    # pass on to html
    return render_template("history.html", history_dict=history_dict, leng=leng)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # lookup API and save result in dictionary
        result = lookup(request.form.get("symbol"))

        # checks if the entered Sympol is a valide quote
        if not result:
            return apology("invalid ticker symbol", 403)
        else:
            return render_template("quoted.html", name=result["name"], price=usd(result["price"]), symbol=result["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # check the three entry fields
        # Ensure username was entered
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was entered
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password was typed twice for conformation
        elif not request.form.get("conformation"):
            return apology("must confirm password", 403)


        # Query database for already existing username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # if query returns the username it is alread taken and an other one has to be chosen
        if len(rows):
            return apology("username is already taken", 403)

        # DONE #
        # make some extra checks for minimun password strenght reqirements
        if not (re.search("[a-z]", request.form.get("password"))
            and re.search("[A-Z]", request.form.get("password"))
            and re.search("[0-9]", request.form.get("password"))
            and len(request.form.get("password"))>7):
            return apology("password must have at least eight characters and contain at least one uppercase, one lowercase, and one number", 403)

        # Ensure that the entered passwords are identical
        if request.form.get("password") != request.form.get("conformation"):
            return apology("password not identical", 403)

        # if all entered data are correct, write them into the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)"
                    , username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # Ensure a symbol was entered
        if not request.form.get("symbol"):
            return apology("must enter a symbol", 403)


        # Ensure a symbol was entered
        if int(request.form.get("shares")) < 0:
            return apology("must enter a positive number in shares", 403)

        # check the API for the requested symbol
        result = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")
        total = float(result["price"]) * float(shares)

        # query all the one specific stock of the current user and sum up how many that are
        rows = db.execute("SELECT quote, SUM(shares) FROM transactions WHERE user_id = :user_id AND quote=:quote GROUP BY quote"
                            , user_id=session["user_id"], quote=request.form.get("symbol").upper())

        # if nothing returns, this user hasn't gotten any to sell
        if not rows:
            return apology("you don't own this stock", 403)

        # check current cash balance
        cash_re = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])


        if float(rows[0]["SUM(shares)"]) < float(shares):
            return apology("not enough shares to sell", 403)
        else:
            # insert into database
            db.execute("INSERT INTO transactions (user_id, quote, shares, price) VALUES (:user_id, :quote, :shares, :price)"
                        , user_id=session["user_id"], quote=request.form.get("symbol").upper()
                        , shares= float(shares)*(-1), price=float(result["price"]))

            # update new cash balance
            new_cash = cash_re[0]["cash"] + total
            db.execute("UPDATE users SET cash=:cash WHERE id = :id",cash=new_cash, id=session["user_id"])
        return redirect("/")

    else:
        # gather all the stocks the current user owns so that they can be chosen as an option from the dropdown menue
        quotes = db.execute("SELECT quote, SUM(shares) FROM (SELECT user_id, quote, shares FROM transactions WHERE NOT quote='CA$H') WHERE user_id = :user_id GROUP BY quote"
                            , user_id=session["user_id"])

        leng = len(quotes)
        # store the stocks in a listt so they can be passed on the the html after the stocks that add up to 0 were deletet
        quote_list=[]

        for index in range(len(quotes)):
            if quotes[index]['SUM(shares)'] == 0:
                leng -= 1
            else:
                quote_list.append(quotes[index]['quote'])

        # pass data to html (dropdown menue)
        return render_template("sell.html",leng=leng, quotes=quote_list)


@app.route("/funds", methods=["GET", "POST"])
@login_required
def funds():
    """Add or withdraw cash"""
    if request.method == "POST":

        # Ensure a symbol was entered
        if not request.form.get("funds"):
            return apology("you can either fund or withdraw", 403)

        # check the curren cash balance
        cash_re = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        cash = float(cash_re[0]["cash"])

        # check when withdrawal that cash is suficiant
        if float(request.form.get("funds")) < 0 and cash < abs(float(request.form.get("funds"))):
            return apology("you can't withdraw more than you have in cash", 403)
        elif float(request.form.get("funds")) > 1000000000000:
            return apology("you wish ;)", 403)
        else:
            # id so update cash and enter new transaction into database
            new_cash = cash + float(request.form.get("funds"))
            db.execute("UPDATE users SET cash=:cash WHERE id = :id", cash=new_cash, id=session["user_id"])
            db.execute("INSERT INTO transactions (user_id, quote, shares, price) VALUES (:user_id, :quote, :shares, :price)"
                        , user_id=session["user_id"], quote="CA$H", shares=1
                        , price=float(request.form.get("funds")))

        return render_template("funds.html", cash=usd(new_cash))
    else:
        # when opend in get mode check for cash and pass on to html
        cash_re = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        cash = float(cash_re[0]["cash"])
        return render_template("funds.html", cash=usd(cash))


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
