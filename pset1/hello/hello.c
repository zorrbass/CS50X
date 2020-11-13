#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //promt the user to enter its name 
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}