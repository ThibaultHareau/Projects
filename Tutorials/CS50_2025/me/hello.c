#include <cs50.h>
#include <stdio.h>

// Print hello world
int main(void)
{
    string name = get_string("What's your name?");
    printf("hello, %s\n", name);
}
