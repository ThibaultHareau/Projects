#include <cs50.h>
#include <stdio.h>

void print_row(int spaces, int bricks);
void print_char(int spaces, char c);

int main(void)
{
    // Prompt the user for the pyramid's height
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // Print a pyramid of that height
    for (int i = 0; i < n; i++)
    {
        // Print row of bricks
        print_row(n - i - 1, i + 1);
    }
}

void print_row(int spaces, int bricks)
{
    // Define spaces and bricks
    const char space = ' ';
    const char brick = '#';

    // Print row
    print_char(spaces, space);
    print_char(bricks, brick);
    print_char(2, space);
    print_char(bricks, brick);
    printf("\n");
}

void print_char(int n, char c)
{
    // Print custom char
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}
