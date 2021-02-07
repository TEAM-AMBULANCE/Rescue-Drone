//import all the files and libraries:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#define micSampleSize 50
/* ===============================GENERAL IDEA (STEPS):============================
1. 3 audio sensors detect which direction the distress is coming from
2. when near the audio distress, the arducam turns on 
3. then arducam converts the usual 8-bit colour images to lesser 4-bit gray scale images (for faster transmission) 
4. the arduino detects weither or not theres a temperature range being displayed (that of a human radiating heat of their skin)
5. if the cam detects a valid temp range then the camera sends a picture to ground station, with the drones gps location
6. the groundstation then processes the image to detect contours to see if the object is similar shape to that of a human
*/



unsigned concatenate(unsigned x, unsigned y) //from==> https://stackoverflow.com/questions/12700497/how-to-concatenate-two-integers-in-c
{
    unsigned pow = 10;
    while (y >= pow)
        pow *= 10;
    return x * pow + y;
}
int DetectSpeech()
{
    char humanSpeech = 'Y';
    return (char) humanSpeech; 
}

int imgProcessing()
{

    // importing image:
    // 16 bit bitmap contains more shades/colour intensity ie- more steps to white,  then 8 or 4bit==> https://www.photoshopessentials.com/essentials/16-bit/#:~:text=The%20answer%20goes%20back%20to,to%2065%2C536%20shades%20of%20gray.

    FILE *fIn = fopen("HumanThermalimage_1.bmp", "rb");
    //FILE *fIn2 = fopen("HumanThermalimage_3.bmp", "rb");

    FILE *fOut = fopen("HumanThermalimage_1_4bit.bmp", "wb");
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

    unsigned char PIXEL[320][240];
    unsigned char gray, yellow;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fread(pixel, 3, 1, fIn); //read 1st 3 bits of fin and assign to pixel

            if ((pixel[0] <= 180) && (pixel[1] <= 180) && (pixel[2] >= 0))
            {
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 0;
            }

            fwrite(&pixel, 3, 1, fOut);
        }
        // fread(pixel, padding, 1, fIn);
        //fwrite(pixel, padding, 1, fOut);
    }
    fclose(fOut);

    fclose(fIn);

    return 0;
}
//  convert the float data to 3 byte int and flip the higest bit if negative
uint32_t _float2int(float data, int range)
{
  
    uint32_t int_data = (data * range + 0.5)/range;

    return int_data;
}
int main ()
{
    uint32_t MIC1[micSampleSize], MIC2[micSampleSize], MIC3[micSampleSize]; // holds  audio values (sample size can differ for each MIC) but assume same size for simplicity
    uint32_t timeCount_1[micSampleSize];// unint32_t because we need more int bit space since rand() and RAND_MAX floats output exceeds that of INT_MAX values 
    float audio_volt = 5.0;
    float maxAnalogConst = 1024 / 5;
    int maxAnalog;
    if (audio_volt = 5.0)
    {
        maxAnalog = ceil(audio_volt * maxAnalogConst);
    }
    else if (audio_volt = 3.3)
    {
        maxAnalog = ceil(audio_volt * maxAnalogConst);
    }
    int audioRange = maxAnalog;
    float timeRange =   micSampleSize*0.2;
    printf("timeRange: %f \t audio range is: %d \n", timeRange, audioRange);

    size_t sizeMIC1, sizeMIC2, sizeMIC3;
    sizeMIC1 = sizeof(MIC1) / sizeof(*MIC1); // sizeof(*array) gives variable type in bytes

    //printing time array:
    printf("__________________TIME ARRAY:_______________ \n");
    timeCount_1[0] = 0;
    for (int i = 1; i <= micSampleSize; i++)
    {

        timeCount_1[i] = timeCount_1[i-1] + _float2int( (float)rand() / (float)(RAND_MAX) * (timeRange), 10); // equation from==> https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
        printf("%d  \n", timeCount_1[i]);
        //timeCount_2[i] = (timeCount_2[i - 1] + ((float)rand() / (float)(RAND_MAX)) * (timeRange)); // equation from==> https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
        //timeCount_3[i] = (timeCount_3[i - 1] + ((float)rand() / (float)(RAND_MAX)) * (timeRange)); // equation from==> https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
     //floor(1.15*timeCount_1[i - 1]+ 1); // equation from==> https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
    }
    printf("______________MIC ARRAYS:__________  \n");
    int count; //[micSampleSize/2];int j=0;
    for (int i = 0; i <= micSampleSize; i++)
    {

        //assigning values to MIC1-3 doesnt matter
        MIC1[i] = _float2int(((float)rand() / (float)(RAND_MAX)) * (audioRange),10);
        MIC2[i] = _float2int(((float)rand() / (float)(RAND_MAX)) * (audioRange),10);
        MIC3[i] = _float2int(((float)rand() / (float)(RAND_MAX)) * (audioRange),10);
        //floor(1.12*MIC1[i] + ceil(audioRange/micSampleSize));
        //floor(MIC2[i]+ceil(audioRange / (2*micSampleSize)));
        //floor(0.77*MIC3[i]+floor(audioRange / (3*micSampleSize)));
        printf("%d \t %d \t %d \n", MIC1[i], MIC2[i], MIC3[i]);
    }
    
    char humanSpeech = DetectSpeech();
    if (humanSpeech == 'Y')
    {
        printf("WRITE TO SERIAL \n"); 
        char SIMULATE = 'N';
        //========================================================================
        //============================SIMULATE SENDING DATA VIA RADIO===========================================
        if (SIMULATE=='Y')
        {
            // below is a test by writing to a csv or txt file and reading it(simulates sending via radio)
            // 1. import and call convert image to 4-bit function:
            
            int imgProcess = imgProcessing();
            FILE *fIn2 = fopen("HumanThermalimage_1_4bit.bmp", "rb");
            FILE *fOut2 = fopen("AvionicsBayComms.csv", "wb");
            char *cameraHeader = " "; 
            unsigned char header[54];

            cameraHeader = (char *)0x00;
            fwrite(cameraHeader, sizeof(char *), 10, fOut2);
            
            fread(header, sizeof(unsigned char), 54, fIn2);
            fwrite(header, sizeof(unsigned char), 54, fOut2);

            int width = *(int *)&header[18];
            int height = abs(*(int *)&header[22]);
            int stride = (width * 3 + 3) & ~3;
            int padding = stride - width * 3;
            unsigned char pixel[3];
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    fread(pixel, 3, 1, fIn2); //read 1st 3 bits of fin and assign to pixel
                    fwrite(&pixel, 3, 1, fOut2);
                }
            }  
            // 2. write MIC time data to file:
            char *mictimeHeader=" ";
            mictimeHeader = (char*)0x91 ; 
            fwrite(mictimeHeader,sizeof(char*), 10,fOut2); 
            fwrite(timeCount_1,sizeof(int),micSampleSize,fOut2);
            
            //3. write MIC data to file:
            char *mic1Header = " ";
            mic1Header = (char*)0x00;
            fwrite(mic1Header, sizeof(char*), 4, fOut2);
            fwrite(MIC1, sizeof(int), micSampleSize, fOut2);

            char *mic2Header = " ";
            mic2Header = (char*)0x01;
            fwrite(mic2Header, sizeof(char*), 4, fOut2);
            fwrite(MIC2, sizeof(int), micSampleSize, fOut2);

            char *mic3Header = " ";
            mic3Header = (char*)0x02;
            fwrite(mic3Header, sizeof(char*), 4, fOut2);
            fwrite(MIC3, sizeof(int), micSampleSize, fOut2);

            //4. write gps location data:

            //using fixed data for simplicity and testing purposes:
            uint32_t GPSdata[1];
            GPSdata[0] = _float2int(-114.02,10);
            GPSdata[1] = _float2int((51.06),10);
            

            char*gpsHeader= " ";
            gpsHeader = (char*)0x03;
            fwrite(gpsHeader, sizeof(char*), 4, fOut2);
            fwrite(GPSdata, sizeof(int), 2, fOut2);

            fclose(fOut2);

            fclose(fIn2);
        }
        
    }
    
    
    
    return 0; 
}
