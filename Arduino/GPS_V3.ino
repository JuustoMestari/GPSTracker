#include <SoftwareSerial.h>
#include <EEPROM.h>

#include <SdFat.h>
#include <TinyGPS++.h>
#include <Adafruit_NeoPixel.h>

//ALPHA VERSION 0.1 ( 28/07/2014 )

#define recordFrequency 2000
#define saveStatic false
#define saveStaticMargin 0.00001 //~2meters

//SD
const int chipSelect = 10;
SdFat sd;

//GPS
TinyGPSPlus gps;
SoftwareSerial nss(9, 8);
double old_flat,old_flon;
//default recordFrequency untill getting valid info
int rF=1000;

//LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, 2, NEO_GRB + NEO_KHZ800);

//EEPROM
boolean isEEPROMset;

void setup()
{
    nss.begin(9600); 
    //Serial.begin(9600);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(32);
  
    old_flat=0;
    old_flon=0;
    isEEPROMset=false;
    //eraseEEPROM();
    checkSD();  
}

void loop()
{
  
   unsigned long start = millis();
  
   while (millis() - start < rF)
  {
    feedgps();
  }
  
  if(gps.location.isValid() && gps.date.isValid() && gps.time.isValid())
  {
      //recording frequency becomes the one defined by the user
      rF=recordFrequency;
      
      //if coordinates haven't changed since last record, we don't record them.
      if(!saveStatic && (abs(old_flat-gps.location.lat())<=saveStaticMargin) && (abs(old_flon-gps.location.lng())<=saveStaticMargin))
      {
        swipeLEDS(strip.Color(255,0,255));
        return;
      }
      
      if (!sd.begin(chipSelect, SPI_HALF_SPEED)) 
       {
  	  displayError(3);
       }
      else
       {         
          char buffer[25];
          
          int iyear = gps.date.year();
          int imonth = gps.date.month();
          int iday = gps.date.day();
          int ihour = gps.time.hour();
          int imin = gps.time.minute();
          int isec = gps.time.second();
          
          sprintf(buffer,"LOGS/%d/%d/%d",iyear,imonth,iday);
          sd.mkdir(buffer);
          
          sprintf(buffer,"LOGS/%d/%d/%d/T_%d.CSV",iyear,imonth,iday,getFileName(iday,imonth,iyear-2000));
          ofstream sdlog(buffer, ios::out | ios::app);
          delay(100);
          
          //append data to file
          sdlog << dtostrf(gps.location.lat(), 9, 6, buffer) << "," << dtostrf(gps.location.lng(), 9, 6, buffer) << ",";
          sdlog <<  iday << "/" << imonth << "/" << iyear << " " << ihour << ":" << imin << ":" << isec << endl;                        
            
          if (!sdlog)
          {
             //failed to append text
             displayError(10);                                   
          }
           sdlog.close();
            
           //save new coordinates
           old_flat=gps.location.lat();
           old_flon=gps.location.lng(); 
           
           swipeLEDS(strip.Color(0,255,0));          
        }
  }
  else
  {
    swipeLEDS(strip.Color(0,0,255));
  }  
}

static bool feedgps()
{
  while (nss.available())
  {
    char c = nss.read();
    //Serial.write(c);
    if (gps.encode(c))
      return true;
  }
  return false;
}

int getFileName(int theday, int themonth, int theyear)
{
  int counter =EEPROM.read(0);
  if(isEEPROMset)return counter;
  
  int eeday=EEPROM.read(1);
  int eemonth=EEPROM.read(2);
  int eeyear=EEPROM.read(3);
  
  if(eeday!=theday || eemonth!=themonth || eeyear!=theyear)
  {
	EEPROM.write(1,theday);
	EEPROM.write(2,themonth);
        EEPROM.write(3,theyear);
        counter=0;
  }
  
  EEPROM.write(0,counter+1);
  isEEPROMset=true;
  //do something if counter >255
  return counter+1;
}

void checkSD()
{
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) 
  {
    displayError(6);
  }
  else
  {
    swipeLEDS(strip.Color(255,255,255));
  }
  delay(500);
}

void displayError(int errorNum)
{
  //Errors from 1 -> 15 (4 Leds = 4bits)

  clearLEDS();
  for(int i=0;i<4;i++)
  {
     if(bitRead(errorNum,i)) strip.setPixelColor(i, strip.Color(255,0,0));
  }
  strip.show();
  
  //halt here
  while(1) { }
}
void swipeLEDS(uint32_t Color)
{
  for(int i=0;i<4;i++)
  {
    strip.setPixelColor(i, Color);
    for(int j=0;j<i;j++)strip.setPixelColor(j, strip.Color(0,0,0));//optional line, comment for another swipe effect
    strip.show();
    delay(100);
  }
  clearLEDS();
}
void clearLEDS()
{
  for(int i=0;i<4;i++)strip.setPixelColor(i, strip.Color(0,0,0));
  strip.show();
}
void eraseEEPROM()
{
  for(int i=0;i<4;i++)EEPROM.write(i,0);//Serial.println(EEPROM.read(i));
}
