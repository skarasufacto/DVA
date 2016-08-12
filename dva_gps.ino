/*		dva_gps file
 *	Defines the vars and
 *	functions used to
 *	interact with the gps
 * 	module
 *--------------------------*/

#ifnodef _SOFTWARE_SERIAL_INCLUDED
#include <SoftwareSerial.h>
#define _SOFTWARE_SERIAL_INCLUDED
#endif

#ifnodef _GPS_CONST
#define GPS_TX_PIN 6
#define GPS_RX_PIN 7
#define GPS_MAX_FRAME_LEN 100
#define _GPS_CONST
#endif

//Private vars
SoftwareSerial gps = SoftwareSerial(GPS_RX_PIN, GPS_TX_PIN);
char tramaGPS[GPS_MAX_FRAME_LEN];

/*	gps_setup function
 * Used to setup the gps module
 *---------------------------------
 * returns: void
 * ------------------------------*/
void buzz_setup(){
	pinMode(GPS_RX_PIN, INPUT);
	pinMode(GPS_TX_PIN, OUTPUT);
	gps.begin(4800);
}
