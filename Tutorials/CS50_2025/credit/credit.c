// Libraries
#include <cs50.h>
#include <stdio.h>

// Functions to declare
long get_positive_long();
bool is_correct(long number);
bool luhn_algorithm(long number);
bool is_valid_size(long number);
int get_number_length(long number);
int extract_sum_from_second_to_last(long number);
int extract_sum_from_others(long number);
void print_identity(long number);

int main(void)
{
    long number;
    number = get_positive_long();
    // Check first if the number is correct accordign to size and Luhn's algorithm
    if (is_correct(number))
    {
        // Test identity (AMEX, VISA, Mastercard)
        print_identity(number);
    }
    else
    {
        printf("INVALID\n");
    }
}

void print_identity(long number)
{
    // Considering a correct number, retrieve the identity of the card using first digit(s)
    int size = get_number_length(number);
    if (size == 13)
    {
        if (number / 1000000000000 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (size == 15)
    {
        int digits = number / 10000000000000;
        if (digits == 34 || digits == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (size == 16)
    {
        int digits = number / 100000000000000;
        if (digits >= 51 && digits <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if (digits / 10 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

bool is_correct(long number)
{
    // Check firt if the size is 13, 15 or 16 then
    // Perform the Luhn's algorithm only if the number size is correct
    if (is_valid_size(number))
    {
        if (luhn_algorithm(number))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

long get_positive_long(void)
{
    // Prompt the card number
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 1);
    return number;
}

int get_number_length(long number)
{
    // Return the number of digits in a long number
    int size = 1;
    while (number >= 10)
    {
        number = number / 10;
        size++;
    }
    return size;
}

bool is_valid_size(long number)
{
    // Return if the number is 13,15 or 16 digits long
    int size = get_number_length(number);
    if (size == 13 || size == 15 || size == 16)
    {
        return true;
    }
    return false;
}

bool luhn_algorithm(long number)
{
    // Perform the Luhn's algorithm
    int result = 0;
    result = extract_sum_from_second_to_last(number);
    result = result + extract_sum_from_others(number);
    return result % 10 == 0;
}

int extract_sum_from_second_to_last(long number)
{
    // Do the first step of Luhn's algorithm
    int result = 0;
    int extract;
    while (number > 10)
    {
        extract = ((number / 10) % 10) * 2;
        result = result + (extract / 10) + (extract % 10);
        number = number / 100;
    }
    return result;
}

int extract_sum_from_others(long number)
{
    // Do the second step of Luhn's algorithm
    int result = 0;
    while (number > 0)
    {
        result = result + (number % 10);
        number = number / 100;
    }
    return result;
}
