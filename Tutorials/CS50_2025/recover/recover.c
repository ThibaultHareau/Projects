#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: recover [filename]\n");
        return 1;
    }

    // Remember filenames
    char *infile = argv[1];
    // Open input file
    FILE *card = fopen(infile, "r");
    if (card == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    // Init image counter and filename
    int counter = 0;
    char outfile[8];
    sprintf(outfile, "%03i.jpg", counter);
    FILE *img = fopen(outfile, "w");

    // Create a buffer
    uint8_t buffer[512];
    int first_file = 1;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Check if JPEG header <=> New file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (first_file == 1)
            {
                first_file = 0;
            }
            else
            {
                // Close the previous file
                fclose(img);
                counter++;
                sprintf(outfile, "%03i.jpg", counter);
                img = fopen(outfile, "w");
            }
            fwrite(buffer, 512, 1, img);
        }
        else
        {
            if (first_file == 0)
            {
                fwrite(buffer, 512, 1, img);
            }
        }
    }

    // Close files
    fclose(card);
    fclose(img);
    return 0;
}
