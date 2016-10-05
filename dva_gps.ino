/*		dva_gps file
 *	Defines the vars and
 *	functions used to
 *	interact with the gps
 * 	module
 *--------------------------*/
#include "DVA.h"

#ifndef _SOFTWARE_SERIAL_INCLUDED
#include <SoftwareSerial.h>
#define _SOFTWARE_SERIAL_INCLUDED
#endif

#ifndef _TINYGPS_INCLUDED
#include <TinyGPS++.h>
#define _TINYGPS_INCLUDED
#endif

#ifndef _GPS_CONST
#define GPS_TX_PIN 6
#define GPS_RX_PIN 7
#define GPS_MAX_FRAME_LEN 100
#define GPSBAUD 4800 //TODO: set the correct baud
#define _GPS_CONST
#endif

//Private vars
TinyGPSPlus gps;

/*	gps_setup function
 * Used to setup the gps module
 *---------------------------------
 * returns: void
 * ------------------------------*/
void gps_setup(){
	//pinMode(GPS_RX_PIN, INPUT);
	//pinMode(GPS_TX_PIN, OUTPUT);
	Serial1.begin(9600);
}

void gps_read(Dva *myDva){
	while (Serial1.available() > 0){
		if (gps.encode(Serial1.read())){
			//if(gps.location.isValid()){
				myDva->latitude = gps.location.lat();
				myDva->longitude = gps.location.lng();
Serial.print(gps.location.lat());
			//}
		}
	}
}

double gps_getDistance(Dva first, Dva second){
	return TinyGPSPlus::distanceBetween(first.latitude, first.longitude, second.latitude, second.longitude);
}

double gps_getCourse(Dva first, Dva second){
	return TinyGPSPlus::courseTo(first.latitude, first.longitude, second.latitude, second.longitude);
	/*TODO: examine the result or use TinyGPSPlus.cardinal(course) to get the data*/
}
