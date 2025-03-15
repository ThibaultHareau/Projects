from cs50 import get_int

# Prompt height
while True:
    height = get_int("Height : ")
    if height <= 8 and height > 0:
        break

# Loop over each level
for i in range(1, height + 1):
    print(" "*(height - i), end="")
    print("#"*i, end="")
    print("  ", end="")
    print("#"*i)
