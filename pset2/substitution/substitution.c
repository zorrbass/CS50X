#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

bool check_validity(char key[]);
void convert_upper(char key[]);
void cipher(char key[], char plaintext[]);

int main(int argc, string argv[])
{
    if (argc != 2)                  // check if theres is one commandline argument.
    {
        printf("Usage: ./substitution key\n");
        return 1;                   // if no or more than two arguments were passed, exit with return 1.
    }
 
    int lenght = (int) strlen(argv[1]);     // get lenght of key argument.
    
    convert_upper(argv[1]);                 // call func to transver to upper case.
    
    if (lenght != 26)                       // if the key argument is 26. if not exit with return 1.
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (check_validity(argv[1]))       // The Key argument passed is ok, proceede with the program.
    {
        string plaintext = get_string("plaintext:  ");      // get the user input
        printf("ciphertext: ");
        cipher(argv[1], plaintext);         // function to cipher the plaintext into ciphered text
        printf("\n");
        return 0;                           // main returns 0. meaning everything is OK
    }
    else
    {
        printf("FALSE Key.\n");
        return 1;
    }
}



///***FUNCTIONS***///

void cipher(char key[], char plaintext[])       // This function assignes any plaintxt char its cipher
{
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        if (plaintext[i] > 64 && plaintext[i] < 91)             // chars A-Z
        {
            printf("%c", (char) key[plaintext[i] - 65]);
        }
        else if (plaintext[i] > 96 && plaintext[i] < 123)       // chars a-z
        {
            printf("%c", ((char) key[plaintext[i] - 97]) + 32);
        }
        else                                                    // all other chars stay the same
        {
            printf("%c", (char) plaintext[i]);              
        }
    }
    printf("\n");
}


void convert_upper(char key[])          // This funktion converts any a-z char into upper case
{
    for (int i = 0; i < 26; i++)
    {
        if (key[i] > 96 && key[i] < 123)
        {
            key[i] = key[i] - 32;
        }
    }
}


bool check_validity(char key[])
{
    for (int i = 0; i < 26; i++)        // This loop checks if there are any other chars than A-Z
    {
        if (key[i] < 65 || key[i] > 90)
        {
            printf("The comandline key has an invalid charactrer\n");
            return false;
        }
    }
    
    for (int i = 0; i < 25; i++)        // This loop checks if any characters are doubled
    {
        for (int j = i + 1; j < 26; j++)
        {
            if (key[i] == key[j])
            {   
                printf("You can only use every letter once!\n");
                return false;
            }
        }
    }
    return true;
}
