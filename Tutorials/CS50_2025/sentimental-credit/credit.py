from cs50 import get_string


def get_identity(card_number):
    # Check if size is valid (13, 15, 16)
    if not check_valid_size(card_number):
        print("INVALID")
        return
    # Perform Luhn's algorithm
    if not luhn_algorithm(card_number):
        print("INVALID")
        return
    # Get card number length to get the identity
    get_card_identity(card_number)


def check_valid_size(card_number):
    lenght = len(card_number)
    if lenght in [13, 15, 16]:
        return True
    return False


def luhn_algorithm(card_number):
    result = extract_sum_from_second_to_last(card_number)
    result = result + extract_sum_from_others(card_number)
    return result % 10 == 0


def extract_sum_from_second_to_last(card_number):
    result = 0
    while len(card_number) >= 2:
        extract = int(card_number[:-1][-1]) * 2
        result = result + (extract // 10) + (extract % 10)
        card_number = card_number[:-2]
    return result


def extract_sum_from_others(card_number):
    result = 0
    while len(card_number) > 0:
        result = result + int(card_number[-1])
        if len(card_number) > 1:
            card_number = card_number[:-2]
        else:
            return result
    return result


def get_card_identity(card_number):
    size = len(card_number)
    if size == 13:
        if card_number[0] == '4':
            print("VISA")
            return
        print("INVALID")
        return
    if size == 15:
        if card_number[:2] in ["34", "37"]:
            print("AMEX")
            return
        print("INVALID")
        return
    if size == 16:
        if card_number[:2] in ["51", "52", "53", "54", "55"]:
            print("MASTERCARD")
            return
        if card_number[0] == '4':
            print("VISA")
            return
        print("INVALID")
        return
    print("INVALID")
    return


# Prompt credit card number
card_number = get_string("Number : ")

# Get the identity
get_identity(card_number)
