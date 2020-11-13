#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(int* xp, int* yp);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    // WP Print the preferences matrix
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }
    // WP end


    add_pairs();

    // WP Print the preferences matrix
    //printf("\n");
    //for (int i = 0; i < pair_count; i++)
    //{
    //    printf("%i %i \n", pairs[i].winner, pairs[i].loser);
    //}
    //printf("\n");
    //
    // WP end

    sort_pairs();

    // WP Print the preferences matrix
    printf("\n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("%i %i \n", pairs[i].winner, pairs[i].loser);
    }
    printf("\n");
    // WP end

    lock_pairs();
    

    // WP Print the preferences matrix
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }
    // WP end



    print_winner();
    
    return 0;
}

//Swap funtion
// use pointers so that the funktion changes the actual inputs
void swap(int* xp, int* yp) 
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Update ranks given a new vote
//DONE
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
//DONE
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
//DONE
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
//DONE
void sort_pairs(void)
{

    int strenght_unsorted[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        strenght_unsorted[i] = abs(preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]);
    }

    //WP//////////////////////////////////////////
    //for (int i = 0; i < pair_count; i++)      //
    //{                                         //    
    //   printf("%i  ", strenght_unsorted[i]);  //
    //}                                         //    
    //WP//////////////////////////////////////////

    // Find the minimum element in unsorted array
    for (int i = 0; i < pair_count - 1; i++)
    {
        int min_idx = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (strenght_unsorted[j] > strenght_unsorted[min_idx])
                min_idx = j;
        }
        // Swap the found minimum element
        // with the first element
        //if (min_idx != 0)
        {
            swap(&pairs[min_idx].winner, &pairs[i].winner);
            swap(&pairs[min_idx].loser, &pairs[i].loser);
            swap(&strenght_unsorted[min_idx], &strenght_unsorted[i]);
        }
    }
    
    
    printf("\n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("%i  ", strenght_unsorted[i]);
    }
    
    
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
// TODO!!
void lock_pairs(void)
{
    pair locked_list[pair_count];            //define and initiate a list that only stores the locked pairs
    for (int i = 0; i < pair_count; i++)
    {
        locked_list[i].winner = 99;
        locked_list[i].loser = 99;
    }
    
    
    locked[pairs[0].winner][pairs[0].loser] = true; // the strongest winner is a guarateed lock.
    locked_list[0].winner = pairs[0].winner;
    locked_list[0].loser = pairs[0].loser;
    int locked_list_count = 1;
    
    
    for (int i = 1; i < pair_count; i++) //go through sorted list pairs[i] (the list of sorted winner/losers)
    {
        for (int j = 0; j < pair_count; j++ ) // go through locked_list
        {
            if (pairs[i].winner == locked_list[j].loser)
            {
                printf("check_cycle();");
            }
            else
            {
                locked[pairs[i].winner][pairs[i].loser] = true;
                locked_list[i].winner = pairs[i].winner;
                locked_list[i].loser = pairs[i].loser;
            }
        }
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

