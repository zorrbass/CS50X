#include <stdio.h>
#include <cs50.h>
#include <math.h>

//program takes an amout of change as input and calculates how manny coins are needed to pay it out
// new comment add test for git

int main(void)
{
    float change;                                       //initialize variable
    do                                                  // make a do loop so you can chack for invalide inputs. no negative numbers
    {
        change = get_float("Change owed? ");     //get_float checks that only numbers are accepted as input
    }
    while (change < 0);
    
    //printf("Your change is %.2f\n", change);          //prints the accepted value of change 
    
    int cents = round(change * 100);                    //round makes sure there are no artefacts left after converting float to int
    //printf("You have %i cents\n", cents);

    int i = 0;                          //keeps track of how many coins will be necessary
    while (cents >= 25)
    {
        i++;
        //printf("%iquater", i);        //to check how many quaters will be needed
        cents = cents - 25;
    }
    while (cents >= 10)
    {
        i++;
        //printf("%idime", i);          //to check how many dimes will be needed
        cents = cents - 10;
    }
    while (cents >= 5)
    {
        i++;
        //printf("%inickel", i);        //to check how many nickels will be needed
        cents = cents - 5;
    }
    while (cents >= 1)
    {
        i++;
        //printf("%ipenny", i);         //to check how many pennies will be needed
        cents = cents - 1;
    }

    printf("%i\n", i);
}