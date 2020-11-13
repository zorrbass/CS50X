#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;                                 //initiate height variable
    
    do
    {
        height = get_int("Height: ");
    }   
    while (height < 1 || height > 8);           //continue as long as input is not 1-8    
        
    
    for (int i = 1; i < height + 1; i++)        //go through every row (i = number of row)
    {
        for (int j = 0; j < height - i; j++)    //print the initial spaces
        {
            printf(" ");
        }
        
        for (int j = 0; j < i; j++)             //print the first #'s
        {
            printf("#");
        }
        
        printf("  ");                           //print the space between
        
        for (int j = 0; j < i; j++)             //print the second #'s
        {
            printf("#");
        }
        
        printf("\n");                           //jump to new line
    }
}