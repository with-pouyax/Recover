#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define constants
#define BLOCK_SIZE 512 // FAT file system block size is 512 bytes

int main(int argc, char *argv[])
{
    // Ensure the correct number of command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open the input file (forensic image)
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        printf("Could not open file: %s\n", argv[1]);
        return 1;
    }

    // Initialize variables
    unsigned char buffer[BLOCK_SIZE]; // Buffer to store blocks of data
    FILE *output_file = NULL;         // File pointer for the recovered JPEG
    int jpeg_count = 0;               // Count of JPEG files recovered
    int writing_jpeg = 0;             // 0: not writing, 1: currently writing a JPEG

    // Read the forensic image block by block
    while (fread(buffer, sizeof(unsigned char), BLOCK_SIZE, input_file) == BLOCK_SIZE)
    {
        // Check if the current block is the start of a new JPEG
        int is_new_jpeg = (buffer[0] == 0xff &&
                           buffer[1] == 0xd8 &&
                           buffer[2] == 0xff &&
                           (buffer[3] & 0xf0) == 0xe0);

        if (is_new_jpeg)
        {
            // If already writing a JPEG, close the previous file
            if (writing_jpeg)
            {
                fclose(output_file);
            }
            else
            {
                // Start writing for the first time
                writing_jpeg = 1;
            }

            // Create a new filename for the JPEG
            char filename[8];
            sprintf(filename, "%03i.jpg", jpeg_count);
            jpeg_count++;

            // Open a new file for the JPEG
            output_file = fopen(filename, "wb");
            if (output_file == NULL)
            {
                printf("Could not create file: %s\n", filename);
                fclose(input_file);
                return 1;
            }
        }

        // If currently writing a JPEG, write the buffer to the file
        if (writing_jpeg)
        {
            fwrite(buffer, sizeof(unsigned char), BLOCK_SIZE, output_file);
        }
    }

    // Close any remaining open files
    if (output_file != NULL)
    {
        fclose(output_file);
    }
    fclose(input_file);

    return 0;
}
