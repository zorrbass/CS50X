from cs50 import SQL        
import sys

if len(sys.argv)!=2:                                                # checks if there is only one extra comandline argument
    print("You need exactely one House as comandline argument")
    sys.exit(0)                                                     # if not exit 


db = SQL("sqlite:///students.db")                                   # open the database

# read the query into a list of dictionaries. ? is a placeholder for an argument
return_list = db.execute("SELECT first, middle, last, birth FROM students WHERE house=? ORDER BY last, first", sys.argv[1])

for entry in return_list:                                           # loop through the list of dicts
    if entry['middle']==None:                                       # if no middle name 
        print(f"{entry['first']} {entry['last']}, born {entry['birth']}")
    else:
        print(f"{entry['first']} {entry['middle']} {entry['last']}, born {entry['birth']}")
