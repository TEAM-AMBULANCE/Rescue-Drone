#include <TinyGPS++.h>
#include <SoftwareSerial.h>


/**************************************************************************/
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
char GPSdata[5];char GPSdata2[9];

char NMEAheader[15]; 
char LATstr[11];
char LONGstr[11];
float LAT,LONG;  
int i=0;int count=0;int count2;
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("GPS TEST"));

  Serial.println(); 
}
int j=0; int z=0;
void loop()
{
  while (ss.available() > 0)
  {
      char Data = ss.read();
            ss.flush(); 
            //Serial.flush();
         if (Data != ',')
         {
           

             NMEAheader[count] =Data;
             count++;

            if (count > 14)
             {
              memset(NMEAheader,0,strlen(NMEAheader));
              count=0;             

             }
             if (count==14)
             {
                 Serial.print("NMEA header:  \t");

                 Serial.println(NMEAheader);
             }
             
       if ((*strstr(NMEAheader,"$GPRMC") !=NULL) )
        {
      GPSdata[count2] = Data;
             i++; count2++;
           if (count2 > 5)
             {
              memset(GPSdata,0,strlen(GPSdata));
              count2=0;             

             }
            if (count2==5)
             {
                 Serial.print("GPS data is:  \t");

                 Serial.println(GPSdata);
             }
          if (*strstr(GPSdata,"A") != NULL) 
            {
              Serial.println("DATA IS VALID!");
              GPSdata2[j] = Data; j++; 
              if (*strstr(GPSdata2,'N')!=NULL )
             { 
                Serial.print("LATTITUDE is in string:");
                for (int jj=sizeof(GPSdata2)/sizeof(char);jj>=15;jj--)
                {
                LATstr[jj] = GPSdata[jj];
                
                Serial.print(LATstr[jj]); 
                }
                LAT = atof(LATstr);
                Serial.print("\n LAT in float:"); Serial.println(LAT);  
              
             }
        }
      }
     }
  }
      //Serial.write(Data);
  
    
  
}
