/*		dva_gps file
 *	Defines the vars and
 *	functions used to
 *	interact with the gps
 * 	module
 *--------------------------*/
#include "DVA.h"

#ifndef _TINYGPS_INCLUDED
#include <TinyGPS++.h>
#define _TINYGPS_INCLUDED
#endif

#ifndef _GPS_CONST
#define GPS_TX_PIN 6
#define GPS_RX_PIN 7
#define GPS_MAX_FRAME_LEN 100
#define GPSBAUD 9600 //TODO: set the correct baud
#define _GPS_CONST
#endif

//Private vars
TinyGPSPlus gps;

/*	gps_setup function
 * Used to setup the gps module
 *---------------------------------
 * returns: void
 * ------------------------------*/
void gps_goodread(Dva *myDva){
  Serial1.begin(GPSBAUD);
  bool ready = false;
  while(!ready){ 
    while (Serial1.available() > 0 && !gps.encode(Serial1.read())){}
    //displayInfo();
    if(gps.location.isUpdated()) {
      ready = true;
      displayInfo();
    }
    myDva->latitude = gps.location.lat();
    myDva->longitude = gps.location.lng();
    if (millis() > 5000 && gps.charsProcessed() < 10){
      Serial.println(F("No GPS detected: check wiring."));
      while(true);
      }
   }
   //Serial.print("Ready!");
   Serial1.end();
}

/*	gps_read function
 * Reads the gps data, parses the
 * 		info and modifies the 
 * 		latitude and longitude of
 * 		the device's structure
 *----------------------------------
 * @my_dva: Dva structure where the
 * 		data will be stored
 *----------------------------------
 * returns: void
 *--------------------------------*/
/*void gps_read(Dva *myDva){
	while (Serial1.available() > 0){
		if (gps.encode(Serial1.read())){
      while(gps.location.isValid()){Serial.println("NO");}
				myDva->latitude = gps.location.lat();
				myDva->longitude = gps.location.lng();
		}
	}
  displayInfo();
}
*/


void displayInfo()
{
  Serial.print(F("Location: ")); 
  //if (gps.location.isValid())
  //{
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print(F(","));
    Serial.print(gps.satellites.value());
  //}
  //else
  //{
    //Serial.print(F("INVALID"));
  //}

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
/*	gps_getDistance function
 * Usefull to know the distance
 * 		between 2 devices
 *----------------------------------
 * @first: A dva structure
 * @second: The second Dva structure
 *----------------------------------
 * retuns: double: the distance
 * 			between first and second
 *--------------------------------*/
double gps_getDistance(Dva first, Dva second){
	return TinyGPSPlus::distanceBetween(first.latitude, first.longitude, second.latitude, second.longitude);
}

/*	gps_getCourse function
 * Returns the direction of a
 * 		distant DVA
 *----------------------------------
 * @first: Our Dva structure
 * @second: The distant Dva
 *----------------------------------
 * returns: int: the direction the 
 * 		second structure is
 *--------------------------------*/
int gps_getCourse(Dva first, Dva second){
	double orientation = TinyGPSPlus::courseTo(first.latitude, first.longitude, second.latitude, second.longitude);

 Serial.println(orientation);
	int result;
	
		if(orientation >= 22.5 && orientation < 67.5){
			//result = NORTHEAST;
     result = 1;
		}
		else if(orientation >= 67.5 && orientation < 112.5){
			//result = EAST;
      result = 2;
		}
		else if(orientation >= 112.5 && orientation < 157.5){
			//result = SOUTHEAST;
     result = 3;
		}
		else if(orientation >= 157.5 && orientation < 202.5){
			//result = SOUTH;
     result = 4;
		}
		if(orientation >= 202.5 && orientation < 247.5){
			//result = NORTHWEST;
     result = 5;
		}
		else if(orientation >= 247.5 && orientation < 292.5){
			//result = WEST;
      result = 6;
		}
		else if(orientation >= 292.5 && orientation < 337.5){
			//result = SOUTHWEST;
     result = 7;
		}
	
	return result;
}
