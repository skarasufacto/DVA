/*		dva_pkt file
 *	Defines the main
 *	functions and vars to
 *	send and receive data
 *--------------------------*/

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

/*	rf_setup function
 * Used to setup the rf modules
 * pins and variables
 *---------------------------------
 * returns: void
 * ------------------------------*/
void rf_setup(){
	vw_set_tx_pin(TX_PIN);
	vw_set_rx_pin(RX_PIN);
	vw_set_ptt_pin(TX_EN_PIN);
	pinMode(LED_PIN, OUTPUT);
	vw_set_ptt_inverted(true); // Required for DR3100
	
	vw_setup(2000);
}

//TODO add functions to send and receive the packets from RF modules
/*void rf_tx(){
	
}

char* rf_rx(){
	
}*/

