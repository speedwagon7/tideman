#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
bool checker(int winner, int loser);
void print_winner(void);


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

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] ++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count ++;
                pairs[pair_count - 1].winner = i;
                pairs[pair_count - 1 ].loser = j;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory (selection sort)
void sort_pairs(void)
{
    // TODO - Find pair with most preferences, swap and repeat for i
    for (int i = 0; i < pair_count; i++)
    {
        int x = i;
        //Find pair with most preferences
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[x].winner][pairs[x].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                x = j;
            }

            //swap values
            if (j == pair_count - 1)
            {
                pair temp[1];
                temp[0].winner = pairs[i].winner;
                pairs[i].winner = pairs[x].winner;
                pairs[x].winner = temp[0].winner;

                temp[0].loser = pairs[i].loser;
                pairs[i].loser = pairs[x].loser;
                pairs[x].loser = temp[0].loser;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {

        if (!checker(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}
//Iterate through candidates to see if the loser of the pair is transitively locked over the winner
bool checker(int winner, int loser)
{
    //base case
    if (loser == winner)
    {
        return false;
    }
    //check if loser is locked into another candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] == true)
        {
            if (!checker(winner, i))
            {
                return false;
            }
        }
    }
    return true;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int edges = 0 ;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                edges++;
            }

        }
        if (edges == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}


