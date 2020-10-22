#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h> //to use true and false
/*
        REFERENCES:
rgb to grayscale: 
1. https://stackoverflow.com/questions/54173733/convert-rgb-to-grayscale-in-bare-c
2. https://www.quora.com/How-do-you-turn-an-image-into-grayscale-in-C

3. 
finding jpeg image width n height: 
i.https://stackoverflow.com/questions/18264357/how-to-get-the-width-height-of-jpeg-file-without-using-library
ii.https://web.archive.org/web/20131016210645/http://www.64lines.com/jpeg-width-height

4. using library to process image in C: https://processing.org/tutorials/pixels/


*/


/*
        ********* stuff to look into***************

        1. converting the camera images into .bmp (bitmap) files if not already does so automatically ==> for easier transmission of image
        2. see if its possible to use processing program on an sd card and access it via arduino to save arduino memory space 

*/









// -------------from ref. 1:
// from ref. #3 ii.:

//Gets the JPEG size from the array of data passed to the function, file reference: http://www.obrador.com/essentialjpeg/headerinfo.htm
/*
static char get_jpeg_size(unsigned char *data, unsigned int data_size, unsigned short *widthJPEG, unsigned short *heightJPEG)
{

    //Check for valid JPEG image
    int i = 0; // Keeps track of the position within the file
    if (data[i] == 0xFF && data[i + 1] == 0xD8 && data[i + 2] == 0xFF && data[i + 3] == 0xE0)
    {
        i += 4;
        // Check for valid JPEG header (null terminated JFIF)
        if (data[i + 2] == 'J' && data[i + 3] == 'F' && data[i + 4] == 'I' && data[i + 5] == 'F' && data[i + 6] == 0x00)
        {
            //Retrieve the block length of the first block since the first block will not contain the size of file
            unsigned short block_length = data[i] * 256 + data[i + 1];
            while (i < data_size)
            {
                i += block_length; //Increase the file index to get to the next block
                if (i >= data_size)
                    return false; //Check to protect against segmentation faults
                if (data[i] != 0xFF)
                    return false; //Check that we are truly at the start of another block
                if (data[i + 1] == 0xC0)
                { //0xFFC0 is the "Start of frame" marker which contains the file size
                    //The structure of the 0xFFC0 block is quite simple [0xFFC0][ushort length][uchar precision][ushort x][ushort y]
                    *heightJPEG = data[i + 5] * 256 + data[i + 6];
                    *widthJPEG = data[i + 7] * 256 + data[i + 8];
                    return true;
                }
                else
                {
                    i += 2;                                     //Skip the block marker
                    block_length = data[i] * 256 + data[i + 1]; //Go to the next block
                }
            }
            return false; //If this point is reached then no size was found
        }
        else
        {
            return false;
        } //Not a valid JFIF string
    }
    else
    {
        return false;
    } //Not a valid SOI header
   
} 
*/
// converting image to grayscale:
    int main()
    {
        // importing image:

        FILE *fIn = fopen("HumanThermalimage_3.bmp", "rb");
        FILE *fOut = fopen("HumanThermalimage_3_gray.bmp", "wb");
        if (!fIn || !fOut)
        {
            printf("File error.\n");
            return 0;
        }

        unsigned char header[54];
        fread(header, sizeof(unsigned char), 54, fIn);
        fwrite(header, sizeof(unsigned char), 54, fOut);

        int width = *(int *)&header[18];
        int height = abs(*(int *)&header[22]);
        int stride = (width * 3 + 3) & ~3;
        int padding = stride - width * 3;

        printf("width: %d (%d)\n", width, width * 3);
        printf("height: %d\n", height);
        printf("stride: %d\n", stride);
        printf("padding: %d\n", padding);

        unsigned char pixel[3];
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                fread(pixel, 3, 1, fIn);
                unsigned char gray = pixel[0] * 0.299 + pixel[1] * 0.587 + pixel[2] * 0.114; // rgb eqn.(pixel[0]=r,pixel[1]=g,pixel[3]=b) <== eqn from ref. #2
                memset(pixel, gray, sizeof(pixel));
                fwrite(&pixel, 3, 1, fOut);
            }
            fread(pixel, padding, 1, fIn);
            fwrite(pixel, padding, 1, fOut);
        }
        fclose(fOut);
        fclose(fIn);
        return 0;
    }

