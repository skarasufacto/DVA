/*		DVA Main file
 *	Defines main functions
 *	of the DVA device
 *---------------------------*/

//Includes and defines
#ifndef _INCLUDED_WIRE
#include <VirtualWire.h>
#include <Wire.h>
#define _INCLUDED_WIRE
#endif

/*	Possible DVA Roles:
 * 		'1' TX
 * 		'2' RX		*/
#ifndef _ROLES_DVA
#define TX_ROLE '1'
#define RX_ROLE '2'
#define _ROLES_DVA
#endif

//PKT valid types
#ifndef _PKT_TYPES_CONST
#define BLE_CHMODE 1
#define BLE_UPDATELOCATION 2
#define RF_SEND_LOCATION 3
//TODO: Add more packet types here
#endif

//Buttons ids, used to read data
#ifndef _BUTTONS_IDS
#define CH_MODE_BUTTON 0
#define _BUTTONS_IDS
#endif

//Packet structure definition
#ifndef _PKT_STRUCT
struct dva_pkt {
	int type;
	boolean initialized;
	String data;
	int len;
};
#define _PKT_STRUCT
#endif

//DVA structure definition
#ifndef _DVA_STRUCT
struct dva {
	String id;
	long lastUpdated;
	float latitude;
	float longitude;

};
#define _DVA_STRUCT
#endif

//Global variables
int lastChModeButtonState;
char role_DVA;
//Timers for the push button
long lastChModeTime;
long ChModeDelay;
struct dva *pos;

/*	setup function
 * Used to set variables and pins
 * the arduino will use
 *---------------------------------
 * returns: void
 * ------------------------------*/
void setup(){
        //Initialize Serial
	Serial.begin(9600);
	Serial.write("Initialized\n");
	
	role_DVA = RX_ROLE;
	lastChModeButtonState = HIGH;
	lastChModeTime = 0;
	ChModeDelay = 5000;
  
    pos->lastUpdated = 0.0;
	pos->latitude = 31.435;
	pos->longitude = 125.485;
	//set the pins for the tx and rx
	rf_setup();
	
	//Initialized buttons pins
	buttons_setup();
	
	//Initialize lcd
	lcd_setup();
	
	//Initialize geoposition
	/*position->latitude = 0.0;
	position->longitude = 0.0;*/
}

/*	loop function
 * Core of the program, does
 * all the functions of the
 * 			DVA
 * ---------------------------
 * returns: void
 * -------------------------*/
 void loop(){
	 struct dva_pkt *pkt;
	 int reading;
	 boolean successfullRead = false;
	 boolean changeRole = false;
	 
	 //if role_DVA == TX
	 if(role_DVA == TX_ROLE){
		 tx_rf(pos, RF_SEND_LOCATION);
	 }
	 //else role_DVA = RX
	 else{
		 if(rx_rf(pkt)){
			 lcd_print((char*)pkt->data.c_str());
			 //print ble
			 //send packet location
		 }
	}
	//wait for the RX and TX to end their functions
	delay(1000);
	
	//Now check if the ch_role_button has been pressed
	reading = buttons_read(CH_MODE_BUTTON);

	if(reading != lastChModeButtonState){
		changeRole = true;
	}
	
	//check ble
	successfullRead = rx_from_ble(pkt);
	if(successfullRead){
		switch(pkt->type){
			case BLE_CHMODE :
				if(pkt->data.charAt(10) != role_DVA && (pkt->data.charAt(10) == TX_ROLE || pkt->data.charAt(10) == RX_ROLE)){
					changeRole = 1;
					lastChModeTime = 0;	//Always change role if ble asks for it
				}
				break;
			case BLE_UPDATELOCATION :
				pkt_update_geoposition(pkt, pos);
				break;
		}
	}
	
	if(changeRole && (millis() - lastChModeTime) > ChModeDelay){
                Serial.print("Changed role\n");
		switch(role_DVA){
			case TX_ROLE :
				role_DVA = RX_ROLE;
				break;
			case RX_ROLE :
				role_DVA = TX_ROLE;
				break;
		}
		
		lastChModeTime = millis();
	}
 }
