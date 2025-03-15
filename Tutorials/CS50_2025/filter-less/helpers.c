#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int intensity;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            intensity =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = intensity;
            image[i][j].rgbtGreen = intensity;
            image[i][j].rgbtBlue = intensity;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                             .189 * image[i][j].rgbtBlue);
            sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                               .168 * image[i][j].rgbtBlue);
            sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                              .131 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
                sepiaRed = 255;
            if (sepiaGreen > 255)
                sepiaGreen = 255;
            if (sepiaBlue > 255)
                sepiaBlue = 255;
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel_buffer;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, halfway = width / 2; j < halfway; j++)
        {
            pixel_buffer = image[i][width - j - 1];
            image[i][width - j - 1] = image[i][j];
            image[i][j] = pixel_buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;
    float total = .0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum_red = 0;
            sum_green = 0;
            sum_blue = 0;
            total = .0;
            for (int i_gap = 0; i_gap <= 2; i_gap++)
            {
                for (int j_gap = 0; j_gap <= 2; j_gap++)
                {
                    if (i + i_gap - 1 >= 0 && i + i_gap - 1 < height && j + j_gap - 1 >= 0 &&
                        j + j_gap - 1 < width)
                    {
                        sum_red += copy[i + i_gap - 1][j + j_gap - 1].rgbtRed;
                        sum_green += copy[i + i_gap - 1][j + j_gap - 1].rgbtGreen;
                        sum_blue += copy[i + i_gap - 1][j + j_gap - 1].rgbtBlue;
                        total = total + 1.0;
                    }
                }
            }
            image[i][j].rgbtRed = round(sum_red / total);
            image[i][j].rgbtGreen = round(sum_green / total);
            image[i][j].rgbtBlue = round(sum_blue / total);
        }
    }
}
