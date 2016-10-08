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
void gps_setup(){
	Serial1.begin(GPSBAUD);
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
void gps_read(Dva *myDva){
	while (Serial1.available() > 0){
		if (gps.encode(Serial1.read())){
			if(gps.location.isValid()){
				myDva->latitude = gps.location.lat();
				myDva->longitude = gps.location.lng();
			}
		}
	}
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
	double course = TinyGPSPlus::courseTo(first.latitude, first.longitude, second.latitude, second.longitude);
	int result = N;
	
	if(course < 0){
		if(course >= 22.5 && course < 67.5){
			result = NE;
		}
		else if(course >= 67.5 && course < 112.5){
			result = E;
		}
		else if(course >= 112.5 && course < 157.5){
			result = SE;
		}
		else if(course >= 157.5){
			result = S;
		}
	}
	else{
		if(course <= -22.5 && course > -67.5){
			result = NW;
		}
		else if(course <= -67.5 && course > -112.5){
			result = W;
		}
		else if(course <= -112.5 && course > -157.5){
			result = SW;
		}
		else if(course <= -157.5){
			result = S;
		}
	}
	
	return result;
}
