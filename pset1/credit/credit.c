#include <stdio.h>
#include <cs50.h>
#include <math.h>


int get_len(long);                  //function to determine the lenght of the number in digits
int get_checksum(long);             //funcktion to calculate the checksum

int main(void)                      
{
    long number;                    //variable for storing the card number
    
    do
    {
        number = get_long("Number: ");
        //printf("Your number is: %li\n", number);
    }
    while (number < 0);
    
    int digi = get_len(number);     // get the lenght of the input number
    //printf("number has %i digits\n", digi);


    if (get_checksum(number) % 10 == 0)              //if checksum number is correct see if digits and first two numbers match too
    {
        if (digi == 13 && 3999999999999 < number && number < 4999999999999)
        {
            printf("VISA\n");
        }
        else if (digi == 16 && 3999999999999999 < number && number < 4999999999999999)
        {
            printf("VISA\n");
        }
        else if (digi == 15 && 339999999999999 < number && number < 349999999999999)
        {
            printf("AMEX\n");
        }
        else if (digi == 15 && 369999999999999 < number && number < 379999999999999)
        {
            printf("AMEX\n");
        } 
        else if (digi == 16 && 5099999999999999 < number && number < 5600000000000000)
        {
            printf("MASTERCARD\n");
        }                      
        else
        {
            printf("INVALID\n");
        }
    }
    else                                            // if the checksum is not ending in 0 the card is definitely invalid
    {
        printf("INVALID\n");
    }
}

//********************************************************************************************
int get_checksum(long number)
{
    int digi = get_len(number);
    int r;
    int sum = 0;
    int sum2 = 0;
    
    
    for (int i = digi; i > 0; i--)              // start with the last digit which was calculated by get_len()
    {
        r = number % 10;                        // gives the last digit
        //printf("rest %i\n", r);
        number = (number - r) / 10;             // subtract the last digit and divide by 10 
        
        if (i % 2 != digi % 2)                  // sum up digits starting with the second to last
        {
            //printf("2nd r is %i\n", r);
            if (r < 5)                          // numbers smale than 5 are single digit when multiplied by 2
            {
                sum = sum + (2 * r);
            }
            else if (r == 5)                    // numbers 5 and greater need to return the sum of digits 5*2=10 ==> 1+0=1
            {
                sum = sum + 1;
            }
            else if (r == 6)                    // numbers 5 and greater need to return the sum of digits 6*2=12 ==> 1+2=3
            {
                sum = sum + 3;
            }
            else if (r == 7)                    // numbers 5 and greater need to return the sum of digits 7*2=14 ==> 1+4=5
            {
                sum = sum + 5;
            }
            else if (r == 8)
            {
                sum = sum + 7;
            }
            else
            {
                sum = sum + 9;
            }
        }
        else
        {
            sum2 = sum2 + r;                    // sum up every other digit starting with the last one
        }
    }
    //printf("the sum is %i\n", sum);
    //printf("the sum2 is %i\n", sum2);
        
    int checksum = sum + sum2;
    //printf("checksum = %i\n", checksum);
    return checksum;                            // return the value checksum to the function
}

//********************************************************************************************
int get_len(long number)            //function to determine the lenght of the number in digits
{
    int digi = 0;                   // variable for storing the number of digits in the given number
    
    while (number >= pow(10, digi))
    {
        digi++;
    }
    return digi;
}