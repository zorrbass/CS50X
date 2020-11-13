from cs50 import SQL
import sys
import csv
import shlex    #for spliting the string

if len(sys.argv)!=2:                #check if there is only one extra commandline argument
    print("you need one comandline argument")
    sys.exit(0)

db = SQL("sqlite:///students.db")   # define db as SQL database

with open(sys.argv[1], 'r', newline='') as file:    # open the csv file in read mode
    characters = csv.DictReader(file)               # DictRead returns a dictionary with the header as keys
    for line in characters:                         # check line by line
        name_list = shlex.split(line['name'])       # split the name filed at the empty space and check how many wors it has
        
        if len(name_list)==3:                       # if three names insert everything
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", name_list[0], name_list[1], name_list[2], line['house'], line['birth'])
            
        else:                                       # if two names, enter None into the middle name
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", name_list[0], None, name_list[1], line['house'], line['birth'])
