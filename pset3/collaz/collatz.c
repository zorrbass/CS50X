#include <stdio.h>
#include <cs50.h>


// little bit different implementation of the collatz algorithm.
// example of a recursive funktion. a function that calls upon itself
// needs a break point
int collaz(int n);

int main(void)
{
    int n = get_int("n: ");
    printf("collaz is %i\n", collaz(n));
}

int c = 0;  // global variable to count how many recusion steps are needed

int collaz(int n)
{
    
    if (n == 1)             // breakepoint collatz(1)=0. hence return 0
    {
        return 0;
    }
    
    if (n % 2 == 0)         // if the number is even
    {
        c++;                // update counter by one
        collaz(n / 2);
    }
    else                    // else if the number is odd
    {
        c++;
        collaz(n * 3 + 1);
    }
    return c;               // function returns the number of recursions
}