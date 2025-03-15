#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

bool testkey(string key);

int main(int argc, char *argv[])
{
    // There must be only one argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // The key must contain 26 characters
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // The key must be 26 unique letters
    if (!testkey(argv[1]))
    {
        printf("The key must contain 26 unique letters");
        return 1;
    }
    string key = argv[1];
    string message = get_string("plaintext: ");

    // Create the mapping
    int map[26];
    for (int i = 0; i < 26; i++)
    {
        map[i] = toupper(key[i]);
    }

    // Iterate through the sentence to encrypt
    printf("ciphertext: ");
    for (int i = 0, n = strlen(message); i < n; i++)
    {
        if (message[i] >= 'a' && message[i] <= 'z')
        {
            printf("%c", tolower(map[message[i] - 'a']));
        }
        else if (message[i] >= 'A' && message[i] <= 'Z')
        {
            printf("%c", map[message[i] - 'A']);
        }
        else
        {
            printf("%c", message[i]);
        }
    }
    printf("\n");
    return 0;
}

bool testkey(string key)
{
    int counter[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
        }
        if (counter[toupper(key[i]) - 'A'] == 0)
        {
            counter[toupper(key[i]) - 'A']++;
        }
        else
        {
            return false;
        }
    }
    return true;
}
