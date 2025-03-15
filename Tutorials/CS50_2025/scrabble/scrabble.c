#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int getscore(string word);

// Create the scoring table
// The first element is the score for 'A',
// The last one is the score for 'Z'
const int SCORES[26] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                        1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    // Prompt the user for two words
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Compute the score of each word
    int score1 = getscore(word1);
    int score2 = getscore(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int getscore(string word)
{
    int sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        // Convert the character to uppercase
        // Get the position in the alphabet based on the first element: 'A'
        sum = sum + SCORES[toupper(word[i]) - 'A'];
    }
    return sum;
}
