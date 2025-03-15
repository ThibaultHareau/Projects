from cs50 import get_string


def coleman_liau_index(text):
    letters = 0
    punctuations = 0

    words = text.split(' ')
    for word in words:
        for char in word:
            if char.isalpha():
                letters += 1
            elif char in ['!', '?', '.']:
                punctuations += 1

    L = letters / len(words)
    S = punctuations / len(words)
    return 5.88 * L - 29.6 * S - 15.8


text = get_string("Text : ")

grade = coleman_liau_index(text)


if grade < 1:
    print("Before Grade 1")
elif (grade >= 16):
    print("Grade 16+")
else:
    print(f"Grade {round(grade)}")
