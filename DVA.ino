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
#include "DVA.h"

//Global variables
int lastChModeButtonState;
char role_DVA;
//Timers for the push button
long lastChModeTime;
long ChModeDelay;

//Dva's the program actually tracks
Dva my_dva;
Dva distant_dva;

double distance = 0.0;
int course = N;

/*	setup function
 * Used to set variables and pins
 * the arduino will use
 *---------------------------------
 * returns: void
 * ------------------------------*/
void setup(){
    //Initialize Serial
	Serial.begin(9600);
	Serial2.begin(9600);
	Serial.write("Initialized\n");
	
	role_DVA = TX_ROLE;
	lastChModeButtonState = HIGH;
	lastChModeTime = 0;
	ChModeDelay = 5000;
  
    //set the pins for the tx and rx
	rf_setup();
	
	//Initialized buttons pins
	buttons_setup();
	
	//Initialize lcd
	lcd_setup();
	//Initialize buzzer
	buzz_setup();

    //Initialize GPS
    gps_setup();
	
	my_dva.id = "000001";
	my_dva.initialized = false;
}

/*	loop function
 * Core of the program, does
 * all the functions of the
 * 			DVA
 * ---------------------------
 * returns: void
 * -------------------------*/
 void loop(){
	 Dva_pkt pkt;
	 int reading;
	 boolean successfullRead = false;
	 boolean changeRole = false;
	 
	 //if role_DVA == TX
	 if(role_DVA == TX_ROLE){
		 gps_read(&my_dva);
         tx_rf(&my_dva, RF_SEND_LOCATION);
	 }
	 //else role_DVA = RX
	 else{
		 //Read the gps module for usefull data
         gps_read(&my_dva);
                 
		 //Check if the radio has a packet for us
		 if(rx_rf(&pkt)){
			 tx_to_ble(&pkt);	//Send the data to the app
			 array_push(get_pkt_info(&pkt));
		 }
		 
		 //Then check if the array contains a DVA to search and set the arrow and meters to it
		 if(distant_dva.initialized && !array_is_empty()){
			 //calculate bearing, print meters and play sound
			 distance = gps_getDistance(my_dva, distant_dva);
			 course = gps_getCourse(my_dva, distant_dva);
			 display_print_arrow(distance, course);
			 play_beep(distance);
		 }
		 else{
			 if(!array_is_empty()){
				 distant_dva = array_read();
                                 distant_dva.initialized = true;
			 }
			 else{
				 display_print("No DVA found");
			 }
		 }
		 
		 //Last thing, check the buttons state to switch victim or set the actual victim as rescued
		 reading = buttons_read(NEXT_VICTIM_BUTTON);
		 
		 if(reading != lastChModeButtonState /*&& timers to avoid change spam*/){
			 if(array_switch_target()){
				 distant_dva = array_read();
			 }
		 }
		 
		 reading = buttons_read(RESCUED_VICTIM_BUTTON);
		 
		 if(reading != lastChModeButtonState /*&& add timers*/){
			array_pop()
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
	successfullRead = rx_from_ble(&pkt);
	if(successfullRead){
		switch(pkt.type){
			case BLE_CHMODE :
				if(pkt.data.charAt(10) != role_DVA && (pkt.data.charAt(10) == TX_ROLE || pkt.data.charAt(10) == RX_ROLE)){
					changeRole = 1;
					lastChModeTime = 0;	//Always change role if ble asks for it
				}
				break;
			case BLE_UPDATELOCATION :
				pkt_update_geoposition(&pkt, &my_dva);
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
		
		lastChModeTime = millis();
	}
 }
