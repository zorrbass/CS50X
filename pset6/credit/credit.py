import cs50                             

while True:                             # promt the imput as long as the number is not a positive one
    number = cs50.get_int("Number:")
    if number > 0:
        break

digi = len(str(number))                 # get the number of digits
small_num = number / 1000000000000      # helper variable to make the if conditions better readeble

def digitsum(n):                        # function to calculate the sum of digits
    s = 0
    while n:
        s += n % 10
        n //= 10
    return s

def get_checksum(number):               # funktion that returns the checksum
    sum1 = 0
    sum2 = 0
    for i in range(digi, 0, -1):        # loop backwards through the number
        r = number % 10                 # get the last digit
        number //= 10                   # cutoff the last number
        if i % 2 != digi % 2:           # alternate algorithm for every second digit 
            sum1 = sum1 + digitsum(2 * r)
        else:
            sum2 = sum2 + r
    return sum1 + sum2                  # returm the final checksum

if get_checksum(number) % 10 == 0:      # if the checksum ends with a zero it is valide
    if digi == 13 and small_num > 4 and small_num < 5:              # check all possible starting numbers and lengths
        print("VISA")
    elif digi == 16 and 4000 < small_num and small_num < 5000:
        print("VISA")
    elif digi == 15 and 340 < small_num and small_num < 350:
        print("AMEX")
    elif digi == 15 and 370 < small_num and small_num < 380:
        print("AMEX")
    elif digi == 16 and 5100 < small_num and small_num < 5600:
        print("MASTERCARD")
    else:                               # if the card doen't start with known numbers it is considered invalid
        print("INVALID")
else:                                   # if the checksum is not ending in 0 the card is definitely invalid
    print("INVALID")