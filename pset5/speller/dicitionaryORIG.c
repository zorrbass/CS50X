// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// 100000 seems to get pretty good results 
const unsigned int N = 200000; 

// Hash table
node *table[N];

// used to cout the word that are loaded into the hash table from the dictionary
unsigned int counter_size = 0;


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // DONE
    // create a pointer to the linked list at position that hash returns for the specific word to check
    node *cursor = table[hash(word)];
    
    // search for as long as that cursor/pointer doesn't point to NULL
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)        // compare caseinsensitive if the string is present  
        {
            return true;                                // if so, return true
        }
        else
        {
            cursor = cursor->next;                      // otherwhise move one element further in the linked list
        }
    }
    
    return false;                                       // if no elment found in the linked list retur false -> word not in dict
}


// Hashes word to a number
unsigned int hash(const char *word)
{
    
    // DONE
    // Source: djib2 by Dan Bernstein (https://stackoverflow.com/questions/7666509/hash-function-for-string)
    // sligtly modified values for hopefully faster rusults
    
    unsigned long hash = 381;
    int c = *word;
    c = tolower(c);
    
    while (*word != 0)
    {
        hash = ((hash << 4) + hash) + c;
        c = *word++;
        c = tolower(c);
    }
    
    return hash % N;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // DONE
    
    char word[LENGTH + 1];                      // array of chars for every word defined to have a max len of 45
    FILE *dict = fopen(dictionary, "r");        // open the dictionary file in read mode
    
    if (dict == NULL)                           // If it returns NULL something went wrong and the program quits
    {
        printf("File cannot be opend!\n");
        return 1;
    }
    
    while (fscanf(dict, "%s", word) != EOF)     // as long as the end of the file is not reached yet 
    {
        node *n = malloc(sizeof(node));         // allocate space for a pointer of size node which is defined above
        
        if (n == NULL)                          // check if something went wrong with the pointer
        {
            return 1;
        }
        
        strcpy(n->word, word);                  // copy the word in the new pointer n

        int num = hash(word);                   // helper variabel so the hash has to be called only once (mybe not neccesary) 
       
        // the sequence of seting the pointer is crucial here in oder not to orphen the linked list
        n->next = table[num];                   // set the pointer to the current begin of the linked list
            
        table[num] = n;                         // set the new begin to the just added nwe pointer
        
        counter_size++;
    }
    
    fclose(dict);                               // free the memmory used by fopen
    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // DONE
    return counter_size;;     // counter was defined as global variabl so it can be used by this function
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // DONE
    node *cursor;                               // define two hellper pointers
    node *temp;
    
    for (int i = 0; i < N; i++)                 // go through the whole hash table
    {
        cursor = table[i];                      // start at the first linked list
        temp = table[i];
        
        while (cursor != NULL)                  // as long as the curser dosn't point to NULL
        {
            cursor = cursor->next;              // move the cursor one down so we don't lose track of the chain
            free(temp);                         // free the space 
            temp = cursor;                      // now move the temp cursor also one further
        }
    }
    return true;                                // once done, return true
}