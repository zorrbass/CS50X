#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");             // user input via the cs50 get_string funktion. a string is an array of chars.
    
    float Lc = 0.0;                                     // initialize the counter variables.
    float Wc = 1.0;                                     // note Wc (word counter) is one ahead because the last word has no space
    float Sc = 0.0;                                     // Letter conter / Word couter and Sentence counter
    
    for (int i = 0; text[i] != '\0'; i++)           // loop through the whole array until one arrives at the NULL value \0
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))       // count upper and lower case letters
        {
            Lc++;
        }
        else if (text[i] == ' ')                                                            // count spaces for words
        {
            Wc++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')                        // Count senteces
        {
            Sc++;
        }
    }
    
    float L = (Lc * 100.0) / (Wc);                    // L is the average number of letters per 100 words in the text
    float S = (Sc * 100.0) / (Wc);                    // S is the average number of sentences per 100 words in the text
    
    
    // The Coleman-Liau index of a text is designed to output what (U.S.) grade level is needed to understand the text. 
    float index = 0.0588 * L - 0.296 * S - 15.8;        
    
    //printf("L = %f\nS = %f\n", L, S);
    //printf("Your text has %f letters, %f words and %f senteces.\n", Lc, Wc, Sc);
    //printf("index: %.3f\n", index);
    
    if (index < 1)          // check if grade is lower than 1 or higher than 16 and print the result
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}