#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h> //to use true and false
#include <math.h>

/*
        REFERENCES:
rgb to grayscale: 
1. https://stackoverflow.com/questions/54173733/convert-rgb-to-grayscale-in-bare-c
2. https://www.quora.com/How-do-you-turn-an-image-into-grayscale-in-C

3. 
finding jpeg image width n height: 
i.https://stackoverflow.com/questions/18264357/how-to-get-the-width-height-of-jpeg-file-without-using-library
ii.https://web.archive.org/web/20131016210645/http://www.64lines.com/jpeg-width-height


*/


/*
        ********* stuff to look into***************

        1. converting the camera images into .bmp (bitmap) files if not already does so automatically ==> for easier transmission of image
        2. see if its possible to use processing program on an sd card and access it via arduino to save arduino memory space 

*/


//unsigned char GRAY[];
float pixel_2;

// converting image to grayscale:
int main()
{
    // importing image:
    // 16 bit bitmap contains more shades/colour intensity ie- more steps to white,  then 8 or 4bit==> https://www.photoshopessentials.com/essentials/16-bit/#:~:text=The%20answer%20goes%20back%20to,to%2065%2C536%20shades%20of%20gray.

    FILE *fIn = fopen("HumanIRimage_2_24bit_QCIF.bmp", "rb");
    //FILE *fIn2 = fopen("HumanThermalimage_3.bmp", "rb");

    FILE *fOut = fopen("HumanIRimage_2_gray_QCIF.bmp", "wb");
    if (!fIn || !fOut) //||!fIn2)
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
    printf(" header %hhX \n", header);
    unsigned char pixel[3];
    

    unsigned char gray, yellow;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fread(pixel, 3, 1, fIn);  //read 1st 3 bits of fin and assign to pixel
            if (pixel[0] <= 165)
            {
                pixel[0] = 0;
            }
            if (pixel[1] <= 165)
            {
                pixel[1] = 0;
            }
            if (pixel[2] <= 165)
            {
                pixel[2] = 0;
            }
            unsigned char gray = pixel[0] * 31 + pixel[1] * 0.59 + pixel[2] * 0.11; // rgb eqn.(pixel[0]=r,pixel[1]=g,pixel[3]=b) <== eqn from ref. #2
            memset(pixel, gray, sizeof(pixel)); // assign #sizeof(pixel) gray value to  in pixel array
           
            fwrite(&pixel, 3, 1, fOut);

            
            /*
            if ((pixel[0] <= 150) && (pixel[1]<=150) && (pixel[2]>=0))
            {
             pixel[0] =0;
             pixel[1] =0;
             pixel[2] =0;   
            }

            */

            
        }
       fread(pixel, padding, 1, fIn);//read pixels in height
       fwrite(pixel, padding, 1, fOut);
    }
    fclose(fOut);

    fclose(fIn);

    return 0;
}
   
