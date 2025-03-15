#include <cs50.h>
#include <stdio.h>

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
} pair;

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
void merge_sort(int left, int right);
void perform_merge(int left, int right, int middle);
bool lock_create_cycle(int winner, int loser);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i] == name)
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
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count - 1; i++)
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
void sort_pairs(void)
{
    merge_sort(0, pair_count - 1);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!lock_create_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int leader = pairs[0].winner;
    bool confirmed = false;
    while (!confirmed)
    {
        confirmed = true;
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][leader])
            {
                leader = i;
                confirmed = false;
                break;
            }
        }
    }
    printf("%s\n", candidates[leader]);
    return;
}

// Merge sort
void merge_sort(int left, int right)
{
    // If the sub-array is more than size 1
    if (left < right)
    {
        // Split in half
        int middle = left + ((right - left) / 2);
        merge_sort(left, middle);
        merge_sort(middle + 1, right);

        perform_merge(left, right, middle);
    }
    return;
}

void perform_merge(int left, int right, int middle)
{
    // Init the left array
    int size_left = middle - left + 1;
    pair temp_left[size_left];
    for (int i = 0; i < size_left; i++)
    {
        temp_left[i] = pairs[left + i];
    }
    // Init the right array
    int size_right = right - middle;
    pair temp_right[size_right];
    for (int i = 0; i < size_right; i++)
    {
        temp_right[i] = pairs[middle + i + 1];
    }
    // Init indexes for left and right
    int i_left = 0;
    int i_right = 0;
    int score_left;
    int score_right;
    // Perform the merge sort
    while (i_left < size_left && i_right < size_right)
    {
        score_left = preferences[temp_left[i_left].winner][temp_left[i_left].loser];
        score_right = preferences[temp_right[i_right].winner][temp_right[i_right].loser];
        if (score_left >= score_right)
        {
            pairs[left + i_left + i_right] = temp_left[i_left];
            i_left++;
        }
        else
        {
            pairs[left + i_left + i_right] = temp_right[i_right];
            i_right++;
        }
    }

    // At this stage, one of the array is completely merged
    // Let's ensure that the other one is correctly merged
    while (i_left < size_left)
    {
        pairs[left + i_left + i_right] = temp_left[i_left];
        i_left++;
    }

    while (i_right < size_right)
    {
        pairs[left + i_left + i_right] = temp_right[i_right];
        i_right++;
    }
}

bool lock_create_cycle(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }
    // pair_index is the index of pair to be locked
    for (int i = 0; i < candidate_count; i++)
    {
        if (i != loser)
        {
            if (locked[loser][i])
            {
                if (lock_create_cycle(winner, i))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
