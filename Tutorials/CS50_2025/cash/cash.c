#include <cs50.h>
#include <stdio.h>

int get_positive_int();
void greedy_algorithm(int owed);
int used_coins(int owed, int value);

int main(void)
{
    int owed;
    owed = get_positive_int();
    greedy_algorithm(owed);
}

int get_positive_int(void)
{
    int owed;
    // Prompt the change owed
    do
    {
        owed = get_int("Changed owed: ");
    }
    while (owed < 1);
    return owed;
}

void greedy_algorithm(int owed)
{
    int used;
    int coins = 0;

    // 25 cents
    used = used_coins(owed, 25);
    coins = coins + used;
    owed = owed - used * 25;

    // 10 cents
    used = used_coins(owed, 10);
    coins = coins + used;
    owed = owed - used * 10;

    // 5 cents
    used = used_coins(owed, 5);
    coins = coins + used;
    owed = owed - used * 5;

    // 1 cent
    used = used_coins(owed, 1);
    coins = coins + used;
    owed = owed - used * 1;

    printf("%i\n", coins);
}

int used_coins(int owed, int value)
{
    return owed / value;
}
