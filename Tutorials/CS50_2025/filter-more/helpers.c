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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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
    // Calculate Gx & Gy
    int red_x;
    int green_x;
    int blue_x;
    int red_y;
    int green_y;
    int blue_y;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i > 0 && i < height - 1) // If not on a vertical edge
            {
                if (j > 0 && j < width - 1) // If not on a horizontal edge
                {
                    red_x = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i][j - 1].rgbtRed -
                            copy[i + 1][j - 1].rgbtRed + copy[i - 1][j + 1].rgbtRed +
                            2 * copy[i][j + 1].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                    green_x = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i][j - 1].rgbtGreen -
                              copy[i + 1][j - 1].rgbtGreen + copy[i - 1][j + 1].rgbtGreen +
                              2 * copy[i][j + 1].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                    blue_x = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i][j - 1].rgbtBlue -
                             copy[i + 1][j - 1].rgbtBlue + copy[i - 1][j + 1].rgbtBlue +
                             2 * copy[i][j + 1].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                    red_y = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i - 1][j].rgbtRed -
                            copy[i - 1][j + 1].rgbtRed + copy[i + 1][j - 1].rgbtRed +
                            2 * copy[i + 1][j].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                    green_y = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i - 1][j].rgbtGreen -
                              copy[i - 1][j + 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen +
                              2 * copy[i + 1][j].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                    blue_y = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i - 1][j].rgbtBlue -
                             copy[i - 1][j + 1].rgbtBlue + copy[i + 1][j - 1].rgbtBlue +
                             2 * copy[i + 1][j].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                }
                else if (j == 0) // If left column
                {
                    red_x = copy[i - 1][j + 1].rgbtRed + 2 * copy[i][j + 1].rgbtRed +
                            copy[i + 1][j + 1].rgbtRed;
                    green_x = copy[i - 1][j + 1].rgbtGreen + 2 * copy[i][j + 1].rgbtGreen +
                              copy[i + 1][j + 1].rgbtGreen;
                    blue_x = copy[i - 1][j + 1].rgbtBlue + 2 * copy[i][j + 1].rgbtBlue +
                             copy[i + 1][j + 1].rgbtBlue;
                    red_y = -2 * copy[i - 1][j].rgbtRed - copy[i - 1][j + 1].rgbtRed +
                            2 * copy[i + 1][j].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                    green_y = -2 * copy[i - 1][j].rgbtGreen - copy[i - 1][j + 1].rgbtGreen +
                              2 * copy[i + 1][j].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                    blue_y = -2 * copy[i - 1][j].rgbtBlue - copy[i - 1][j + 1].rgbtBlue +
                             2 * copy[i + 1][j].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                }
                else // If right column
                {
                    red_x = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i][j - 1].rgbtRed -
                            copy[i + 1][j - 1].rgbtRed;
                    green_x = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i][j - 1].rgbtGreen -
                              copy[i + 1][j - 1].rgbtGreen;
                    blue_x = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i][j - 1].rgbtBlue -
                             copy[i + 1][j - 1].rgbtBlue;
                    red_y = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i - 1][j].rgbtRed +
                            copy[i + 1][j - 1].rgbtRed + 2 * copy[i + 1][j].rgbtRed;
                    green_y = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i - 1][j].rgbtGreen +
                              copy[i + 1][j - 1].rgbtGreen + 2 * copy[i + 1][j].rgbtGreen;
                    blue_y = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i - 1][j].rgbtBlue +
                             copy[i + 1][j - 1].rgbtBlue + 2 * copy[i + 1][j].rgbtBlue;
                }
            }
            else if (i == 0) // If top row
            {
                if (j > 0 && j < width - 1) // If not on a horizontal edge
                {
                    red_x = -2 * copy[i][j - 1].rgbtRed - copy[i + 1][j - 1].rgbtRed +
                            2 * copy[i][j + 1].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                    green_x = -2 * copy[i][j - 1].rgbtGreen - copy[i + 1][j - 1].rgbtGreen +
                              2 * copy[i][j + 1].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                    blue_x = -2 * copy[i][j - 1].rgbtBlue - copy[i + 1][j - 1].rgbtBlue +
                             2 * copy[i][j + 1].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                    red_y = copy[i + 1][j - 1].rgbtRed + 2 * copy[i + 1][j].rgbtRed +
                            copy[i + 1][j + 1].rgbtRed;
                    green_y = copy[i + 1][j - 1].rgbtGreen + 2 * copy[i + 1][j].rgbtGreen +
                              copy[i + 1][j + 1].rgbtGreen;
                    blue_y = copy[i + 1][j - 1].rgbtBlue + 2 * copy[i + 1][j].rgbtBlue +
                             copy[i + 1][j + 1].rgbtBlue;
                }
                else if (j == 0) // If left column
                {
                    red_x = 2 * copy[i][j + 1].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                    green_x = 2 * copy[i][j + 1].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                    blue_x = 2 * copy[i][j + 1].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                    red_y = 2 * copy[i + 1][j].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                    green_y = 2 * copy[i + 1][j].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                    blue_y = 2 * copy[i + 1][j].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                }
                else // If right column
                {
                    red_x = -2 * copy[i][j - 1].rgbtRed - copy[i + 1][j - 1].rgbtRed;
                    green_x = -2 * copy[i][j - 1].rgbtGreen - copy[i + 1][j - 1].rgbtGreen;
                    blue_x = -2 * copy[i][j - 1].rgbtBlue - copy[i + 1][j - 1].rgbtBlue;
                    red_y = copy[i + 1][j - 1].rgbtRed + 2 * copy[i + 1][j].rgbtRed;
                    green_y = copy[i + 1][j - 1].rgbtGreen + 2 * copy[i + 1][j].rgbtGreen;
                    blue_y = copy[i + 1][j - 1].rgbtBlue + 2 * copy[i + 1][j].rgbtBlue;
                }
            }
            else // If bottom row
            {
                if (j > 0 && j < width - 1) // If not on a horizontal edge
                {
                    red_x = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i][j - 1].rgbtRed +
                            copy[i - 1][j + 1].rgbtRed + 2 * copy[i][j + 1].rgbtRed;
                    green_x = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i][j - 1].rgbtGreen +
                              copy[i - 1][j + 1].rgbtGreen + 2 * copy[i][j + 1].rgbtGreen;
                    blue_x = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i][j - 1].rgbtBlue +
                             copy[i - 1][j + 1].rgbtBlue + 2 * copy[i][j + 1].rgbtBlue;
                    red_y = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i - 1][j].rgbtRed -
                            copy[i - 1][j + 1].rgbtRed;
                    green_y = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i - 1][j].rgbtGreen -
                              copy[i - 1][j + 1].rgbtGreen;
                    blue_y = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i - 1][j].rgbtBlue -
                             copy[i - 1][j + 1].rgbtBlue;
                }
                else if (j == 0) // If left column
                {
                    red_x = copy[i - 1][j + 1].rgbtRed + 2 * copy[i][j + 1].rgbtRed;
                    green_x = copy[i - 1][j + 1].rgbtGreen + 2 * copy[i][j + 1].rgbtGreen;
                    blue_x = copy[i - 1][j + 1].rgbtBlue + 2 * copy[i][j + 1].rgbtBlue;
                    red_y = -2 * copy[i - 1][j].rgbtRed - copy[i - 1][j + 1].rgbtRed;
                    green_y = -2 * copy[i - 1][j].rgbtGreen - copy[i - 1][j + 1].rgbtGreen;
                    blue_y = -2 * copy[i - 1][j].rgbtBlue - copy[i - 1][j + 1].rgbtBlue;
                }
                else // If right column
                {
                    red_x = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i][j - 1].rgbtRed;
                    green_x = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i][j - 1].rgbtGreen;
                    blue_x = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i][j - 1].rgbtBlue;
                    red_y = -copy[i - 1][j - 1].rgbtRed - 2 * copy[i - 1][j].rgbtRed;
                    green_y = -copy[i - 1][j - 1].rgbtGreen - 2 * copy[i - 1][j].rgbtGreen;
                    blue_y = -copy[i - 1][j - 1].rgbtBlue - 2 * copy[i - 1][j].rgbtBlue;
                }
            }
            // Write
            int temp;
            // Red
            temp = round(sqrt(pow(red_x, 2) + pow(red_y, 2)));
            if (temp > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = temp;
            }
            // Green
            temp = round(sqrt(pow(green_x, 2) + pow(green_y, 2)));
            if (temp > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = temp;
            }
            // Blue
            temp = round(sqrt(pow(blue_x, 2) + pow(blue_y, 2)));
            if (temp > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = temp;
            }
        }
    }
}
