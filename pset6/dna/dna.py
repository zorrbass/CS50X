import sys
import csv

def maxi(numOne, numTwo):           # Helperfunction to return the bigger of two values            
    if numOne>numTwo:
        return numOne;
    else:
        return numTwo;


file = open(sys.argv[1], newline='')    # open the database and read out the header
reader = csv.reader(file)               # so that we know which STRs to look for
header = next(reader)                   # The first line is the header
file.close()    


with open(sys.argv[2], 'r', newline='') as file:    # Open the sequence file 
    DNA_sequence = file.read()                      # and read its data into a string

result_list = []                                    # define emty list to store the results

for strs in header[1:len(header)]:                  # loop thorough ervery STR  
    counter_max = 0                                 # after every STR reset counter to 0
    
    for j in range(0,len(DNA_sequence)):            # loop through the whole string of DNA sequence
        if DNA_sequence[j:j + len(strs)] == strs:   # if a supstring of the lenght of an STR matches with the STR in question
            counter = 1                             # start counting
            increment = len(strs)                   # AAGT 4 steps, TTGAA 5 steps, GGACTT 6 steps
            while True:
                if DNA_sequence[j+increment:j + len(strs)+ increment] == strs:  # if the next sequence is also a match
                    counter += 1                                                # increment the counter
                    increment += len(strs)                                      # next move one length further
                else:
                    break                           # otherwise break out of the while loop and continue to check the string
                
            counter_max = maxi(counter_max, counter)# check if the new count exides the last one and update if needed
        else:
            counter = 0                             # if no match set the counter back to 0
    
    result_list.append(str(counter_max))            # at the end amend the resuls list with the max counter for later comparisson


match = False                                       # default no match
with open(sys.argv[1], 'r', newline='') as file:    # open the database again to compare with the results from the DNA string
    reader = csv.DictReader(file)
    for line in reader:                             # check line by line
        data_list=[]                                # initialize a list of values for later comparrison
        for strs in header[1:len(header)]:          # loop though all STRs (AGATC,TTTTTTCT,AATG,TCTAG,...)
            data_list.append(line[strs])            # and appent the values form dthe database to the list

        if data_list == result_list:                # now compare the result-list with the data-list
            print(line['name'])                     # if they match, print the name of the match
            match = True                            # set match to tru so the next message doesn't get printed as well

    if match == False:                              # if no match was found
        print("No match")