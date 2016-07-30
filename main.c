/*		DVA Main file		*/
/*	Defines main functions	*/
/*	of the DVA device		*/
/*--------------------------*/

//Includes and defines
#ifndef _INCLUDED_WIRE
#include <VirtualWire.h>
#include <Wire.h>
#define _INCLUDED_WIRE
#endif

//RF constants
#ifndef _TX_RX_CONST
#define LED_PIN 13
#define TX_PIN 12
#define RX_PIN 11
#define TX_EN_PIN 3
#define _TX_RX_CONST
#endif

//Buttons constants
#ifndef _BUTTONS_CONST
#define CH_MODE_BUTTON_PIN 8
#define _BUTTONS_CONST
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
	vw_set_tx_pin(TX_PIN);
	vw_set_rx_pin(RX_PIN);
	vw_set_ptt_pin(TX_EN_PIN);
	pinMode(LED_PIN, OUTPUT);
	vw_set_ptt_inverted(true); // Required for DR3100
	
	vw_setup(2000);
	
	//set the button pin
	pinMode(CH_MODE_BUTTON_PIN, INPUT);
	
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
			//print lcd/ble
			//send packet location
	}
	//wait for the RX and TX to end their functions
	delay(1000);
	
	//Now check if the ch_role_button has been pressed
	reading = digitalRead(CH_MODE_BUTTON_PIN);
	
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
