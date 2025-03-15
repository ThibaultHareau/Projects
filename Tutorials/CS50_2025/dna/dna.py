import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Missing command-line argument")
        sys.exit(1)

    csv_filename = sys.argv[1]
    text_filename = sys.argv[2]

    # Read database file into a variable
    persons = []
    subsequences = []
    with open(csv_filename) as file:
        reader = csv.DictReader(file)
        subsequences = reader.fieldnames
        for row in reader:
            persons.append(row)

    # Read DNA sequence file into a variable
    dna_sequence = []
    with open(text_filename) as file:
        for x in file:
            dna_sequence.append(x)
            break
    dna_sequence = dna_sequence[0]

    longuest_matches = dict()
    # Find longest match of each STR in DNA sequence
    for subsequence in subsequences[1:]:
        longuest_matches[subsequence] = f"{longest_match(dna_sequence, subsequence)}"

    # Check database for matching profiles
    for person in persons:
        is_person = True
        for subsequence, score in longuest_matches.items():
            if score != person[subsequence]:
                is_person = False
                break
        if is_person:
            print(person["name"])
            return
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
sys.exit(0)
