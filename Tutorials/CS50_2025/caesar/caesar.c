#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

bool is_number(string key);
int strtoint(string key);
char cipher(char original, char firstletter, int key);

int main(int argc, char *argv[])
{
    // There must be only one argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // The argument must be a number
    if (!is_number(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    int key = strtoint(argv[1]);
    string message = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(message); i < n; i++)
    {
        if (message[i] >= 'a' && message[i] <= 'z')
        {
            printf("%c", cipher(message[i], 'a', key));
        }
        else if (message[i] >= 'A' && message[i] <= 'Z')
        {
            printf("%c", cipher(message[i], 'A', key));
        }
        else
        {
            printf("%c", message[i]);
        }
    }
    printf("\n");
}

bool is_number(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isdigit(key[i]) && key[0] != '-')
        {
            return false;
        }
    }
    return true;
}

int strtoint(string key)
{
    int result = 0;
    int digit;
    int sign = 1;
    if (key[0] == '-')
    {
        sign = -1;
    }
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (key[i] != '-')
        {
            digit = key[i] - 48;
            result = result * 10 + digit;
        }
    }
    return sign * result;
}

char cipher(char original, char firstletter, int key)
{
    return ((original - firstletter + key) % 26) + firstletter;
}
