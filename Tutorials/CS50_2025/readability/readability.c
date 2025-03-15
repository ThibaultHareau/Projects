#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

float coleman_liau_index(string text);

int main(void)
{
    // Prompt the user for two words
    string text = get_string("Text :");

    // Apply Coleman-Liau index
    int grade = round(coleman_liau_index(text));

    // Print the output based on the grade
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}

float coleman_liau_index(string text)
{
    int letters = 0;
    int spaces = 0;
    int punctuations = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        else if (isspace(text[i]))
        {
            spaces++;
        }
        else if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            punctuations++;
        }
    }
    int words = spaces + 1;
    int sentences = punctuations;

    float L = 100.0 * (float) letters / words;
    float S = 100.0 * (float) sentences / words;

    return 0.0588 * L - 0.296 * S - 15.8;
}
