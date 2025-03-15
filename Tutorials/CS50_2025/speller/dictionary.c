// Implements a dictionary's functionality
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

unsigned int word_counter = 0;

// TODO: Choose number of buckets in hash table
const unsigned int DEPTH = 6;
const unsigned int N = pow(27, 6);

// Hash table
node *table[N];

void add_node(const unsigned int hashcode, const char *word, unsigned int char_counter)
{
    node *new_node = malloc(sizeof(node));
    for (int i = 0; i < char_counter; i++)
    {
        new_node->word[i] = toupper(word[i]);
    }
    new_node->word[char_counter] = '\0';
    if (table[hashcode] == NULL)
    {
        new_node->next = NULL;
        table[hashcode] = new_node;
    }
    else
    {
        new_node->next = table[hashcode];
        table[hashcode] = new_node;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // It's only certain that the first letter isn't an apostrophe
    return toupper(word[0]) - 'A';
}

bool through_nodes(const char *word, node *n, int i)
{
    if (n == NULL || i == LENGTH + 1)
    {
        return false;
    }
    if (n->word[i] == '\0' && word[i] == n->word[i])
    {
        return true;
    }
    else if (word[i] == '\0')
    {
        return through_nodes(word, n->next, 1);
    }
    else if (n->word[i] == '\0')
    {
        return false;
    }
    if (n->word[i] == toupper(word[i]))
    {
        return through_nodes(word, n, i + 1);
    }
    if (n->word[i] > toupper(word[i]))
    {
        return through_nodes(word, n->next, 1);
    }
    return false;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hashcode = hash(word);
    return through_nodes(word, table[hashcode], 1);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");

    // Iterate by word
    char ch;
    char word[LENGTH];
    unsigned int char_counter = 0;
    while ((ch = fgetc(source)) != EOF)
    {
        word[char_counter] = ch;
        // End of word
        if (ch == '\n')
        {
            // Add word to counter
            word_counter++;

            // Perform Hash
            unsigned int hashcode = hash(word);

            // Allocate node for word
            add_node(hashcode, word, char_counter);

            // Reinitialize the char counter
            char_counter = 0;
        }
        else
        {
            word[char_counter] = ch;
            char_counter++;
        }
    }
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

void unload_node(node *n)
{
    if (n == NULL)
    {
        return;
    }
    unload_node(n->next);
    free(n);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        unload_node(table[i]);
    }
    return true;
}
