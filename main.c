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

//Buttons ids, used to read data
#ifndef _BUTTONS_IDS
#define CH_MODE_BUTTON 0
#define _BUTTONS_IDS
#endif

//Global variables
/*	Possible DVA Roles:
 * 		'1' TX
 * 		'2' RX		*/
char role_DVA = '1';
int lastChModeButtonState = LOW;

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
	 int reading;
	 int changeRole = 0;
	 
	 //if role_DVA == TX
	 if(role_DVA == '1'){
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
	}
	
	//check ble for ch_mode pakcet
	//if(packet received){
		//changeRole = 1;
	//}
	
	if(changeRole){
		switch(role_DVA){
			case '1' :
				role_DVA = '2';
				break;
			case '2' :
				role_DVA = '1';
				break;
		}
	}
 }
