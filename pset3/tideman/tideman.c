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
bool check_cycle(int winner, int looser, int locked_count, int o_winner);

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
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i ", preferences[i][j]);
    //     }
    //     printf("\n");
    // }
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

    // WP Print sorted pairs
    // printf("\n");
    // printf("sort_pairs\n");
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%i %i \n", pairs[i].winner, pairs[i].loser);
    // }
    // printf("\n");
    // WP end

    lock_pairs();

    // WP Print the preferences matrix
    // printf("\n");
    // printf("lock_pairs\n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i ", locked[i][j]);
    //     }
    //     printf("\n");
    // }
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
        if (min_idx != 0)
        {
            swap(&pairs[min_idx].winner, &pairs[i].winner);
            swap(&pairs[min_idx].loser, &pairs[i].loser);
            swap(&strenght_unsorted[min_idx], &strenght_unsorted[i]);
        }
    }

    // WP Print sorted strenght
    // printf("\n");
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%i  ", strenght_unsorted[i]);
    // }
    // WP end

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
// DONE :)))


// initialize a new global variable wich can be accessed by the recursiv function check_cycle
pair locked_pairs[MAX * (MAX - 1) / 2];

void lock_pairs(void)
{   
    // the firs pair is a given lock since it is the stronges win
    // write into the locked_pairs list
    locked_pairs[0] = pairs[0];
    
    locked[pairs[0].winner][pairs[0].loser] = true; // the strongest winner is a guarateed lock.
    int locked_count = 0;

    // loop through the sorted list of pairs and check for cycles using the check_cycle recursive function
    for (int i = 0; i < pair_count; i++)            
    {
        // check every pair in the sorted list for cycles with the allready locked pairs
        if (check_cycle(pairs[i].winner, pairs[i].loser, locked_count, pairs[i].winner) == false)
        {   
            // if no cycle found add the pair the the matrix and the locked list and update the list lenght couter
            locked[pairs[i].winner][pairs[i].loser] = true;
            locked_pairs[locked_count] = pairs[i];
            locked_count++;
           
            // WP Print locked matrix
            // for (int k = 0; k < candidate_count; k++)
            // {
            //     for (int h = 0; h < candidate_count; h++)
            //     {
            //         printf("%i ", locked[k][h]);
            //     }
            // printf("\n");
            // }
            // printf("\n");
            // WP end
        }
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
    // DONE
    for (int i = 0; i < candidate_count; i++)
    {
        int sum = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            sum += locked[j][i];
        }
        if (sum == 0)
        {
            printf("%s\n", candidates[i]); 
        }
    }
    return;
}

// recursive function to check whether there is a cycle in the graph
// it take the current pair as input. also the count of already locked pairs
// and again the winner of the pair to check which will be passed on to the recursive funktion
// as it is one base case
bool check_cycle(int winner, int looser, int locked_count, int o_winner)
{
    // for the first pair no check is nessesary
    // it is the storongest win pair and is a given lock
    // hence ruturn fals (no cycle)
    if (locked_count == 0)
        {
            return false;
        }
        // base case. when the current loser in the recursion stack equals the original winner
        // then there is a cycle present -> true
        else if (looser == o_winner)
        {
            return true;
        }
    // loop through all the pair that are already locked
    for (int i = 0; i < locked_count; i++)
    {
        // if the looser of the latest pair matches with one of the winners
        // check if that pair also continues the trail
        if (looser == locked_pairs[i].winner)
        {
            // here comes the recursion we check the next pair
            // it is now used on the list of allready locked pairs
            // if no cycle found keep looping though the rest of the already locked pairs
            // if none is a cycle we exit the forloo and return false
            if (check_cycle(locked_pairs[i].winner, locked_pairs[i].loser, locked_count, o_winner))
            {
                return true;
            }
        }
    }
    return false;
}