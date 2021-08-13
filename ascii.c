/****************************************************************************
 * ascii.c
 *
 * Digital Media Development 12
 * Final Project
 *
 * Render an ASCII art version of an input image using common characters
 ***************************************************************************/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include <math.h>


int main(int argc, char *argv[])
{

    //Check for number of Arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: infile outfile\n");
        return 1;
    }

    // assign second cammand line argument to input file
    char *infile = argv[1];

    // Open input file unless does not exist
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Check to see if infile is a 24-bit uncompressed BMP
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 3;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // memory allocation for bmp
    RGBTRIPLE(*image)[width] = calloc(height + 1, (width + 1) * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");

        fclose(inptr);
        return 4;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);

    }

    // open outfile
    FILE *outfile;
    outfile = fopen(argv[2], "w");
    fopen(argv[2], "w");

    // iterate through each pixel
    for (int i = height - 1; i < height && i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];

            // average colour of each pixel
            int average = round((pixel.rgbtBlue + pixel.rgbtRed + pixel.rgbtGreen) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtBlue = image[i][j].rgbtGreen = average;

            // Check to see rgb average of each pixel, printing characters onto output file depending on colour value

            if (average <= 50)
            {
                fprintf(outfile, "@@");
            }
            else if (50 < average && average < 100)
            {
                fprintf(outfile, "OO");
            }
            else if (100 <= average && average < 150)
            {
                fprintf(outfile, "//");
            }
            else if (150 <= average && average < 200)
            {
                fprintf(outfile, "..");
            }
            else if (200 <= average && average <= 255)
            {
                fprintf(outfile, "  ");
            }
            if (j == width - 1)
            {
                fprintf(outfile, "\n");
            }

        }
    }
    fclose(outfile);

}