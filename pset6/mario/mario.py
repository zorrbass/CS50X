import cs50

while True:
    height = cs50.get_int("Height: ")           # get the input with the help of cs50 function
    if height in range(1, 9):                   # check if the value is in the correct range    
        break                                   # if so, breake out of the while loop

for i in range(1, height + 1):                  # loop through all lines of the pyramide
    print(" " * (height - i), end="")           # print the empty spaces and dismiss the standard new line of the print function
    print("#" * i, end="")                      # print the rest
    print(" " * 2, end="")
    print("#" * i)
    


