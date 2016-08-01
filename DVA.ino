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

//Global variables
char role_DVA = TX_ROLE;
int lastChModeButtonState = LOW;
//Timers for the push button
long lastChModeTime = 0;
long ChModeDelay = 5000;

/*	setup function
 * Used to set variables and pins
 * the arduino will use
 *---------------------------------
 * returns: void
 * ------------------------------*/
void setup(){
	//set the pins for the tx and rx
	rf_setup();
	
	//Initialized buttons pins
	buttons_setup();
	
	//Initialize lcd
	lcd_setup();
	
	//Initialize Serial
	Serial.begin(9600);
	Serial.write("Initialized\n");
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
	 int changeRole = 0;
	 
	 //if role_DVA == TX
	 if(role_DVA == TX_ROLE){
		 //montar packet
		 //send packet
	 }
	 //else role_DVA = RX
	 else{
		 //reset lcd
		 //read packet from rx
			//lcd_print(packet);
			//print ble
			//send packet location
	}
	//wait for the RX and TX to end their functions
	delay(1000);
	
	//Now check if the ch_role_button has been pressed
	reading = buttons_read(CH_MODE_BUTTON);
	
	if(reading != lastChModeButtonState){
		changeRole = 1;
		lastChModeTime = millis();
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
				//update HERE the geoposition!
				break;
		}
	}
	
	if(changeRole && (millis() - lastChModeTime) > ChModeDelay){
		switch(role_DVA){
			case TX_ROLE :
				role_DVA = RX_ROLE;
				break;
			case RX_ROLE :
				role_DVA = TX_ROLE;
				break;
		}
	}
 }
