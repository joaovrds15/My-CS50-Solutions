#include <cs50.h>
#include <stdio.h>
#include <string.h>

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

bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool hasACycle(int winnner, int loser);

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
    for(int i = 0; i < MAX;i++){
        for(int j = 0; j < MAX; j++){
            preferences[i][j] = 0;
        }
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
    for(int i = 0; i < candidate_count;i++){
        if(strcmp(name,candidates[i]) == 0){
            ranks[rank] = i;
            return true;
        }

    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for(int i = 0; i < candidate_count;i++){
        for(int j = i; j < candidate_count;j++){
            if(i == j){
                preferences[i][j] = 0;
            }
            else{
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for(int i = 0; i < candidate_count;i++){
        for(int j = i; j < candidate_count; j++){
               if(i != j && preferences[i][j] != preferences[j][i]){
                    if(preferences[i][j] > preferences[j][i]){
                        pairs[pair_count].winner = i;
                        pairs[pair_count].loser = j;
                        pair_count++;
                    }
                    else{
                        pairs[pair_count].winner = j;
                        pairs[pair_count].loser = i;
                        pair_count++;
                    }
               }
    }   }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    for(int i = 0; i < pair_count;i++){
        for(int j = i; j < pair_count;j++){
            int maior = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            pair pair_temp = pairs[i];
            if((preferences[pairs[j].winner][pairs[j].loser]  - preferences[pairs[j].loser][pairs[j].winner])> maior){
                pairs[i] = pairs[j];
                pairs[j] = pair_temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for(int i = 0; i < pair_count; i++){
        if(hasACycle(pairs[i].winner,pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }


    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int column;
    int counter = 0;
    for(int i = 0; i < candidate_count; i++){
        counter = 0;
        for(int j = 0; j < candidate_count;j++){
            if(locked[j][i] == false){
                counter++;
                if(counter == candidate_count){
                    column = i;
                    j = candidate_count;
                    i = candidate_count;
                }
            }
        }
    }
    printf("%s\n",candidates[column]);

}

bool hasACycle(int winner, int loser){
    while(winner != -1 && winner != loser)
    {
        bool found = false;
        for(int i = 0; i < candidate_count;i++)
        {
            if(locked[i][winner])
            {
                found = true;
                winner = i;
            }
        }
        if(!found){
            winner = -1;
        }
    }
    if(winner == loser){
        return true;
    }
    return false;
}


