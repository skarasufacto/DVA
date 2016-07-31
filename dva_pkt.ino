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

//PKT management constants
#ifndef _PKT_MNG_CONST
#define PKT_END '#'
#define _PKT_MNG_CONST
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

/*	rx_chMode_packet function
 * Reads Serial (bluetooth) and searches
 * for a valid ch_mode packet.
 *---------------------------------
 * returns: char: will be '0' if
 * 		no valid packet was found
 * 		or '1'|'2' if a valid
 * 		packet was read
 * ------------------------------*/
char rx_chMode_packet(){
	boolean initialized = false;
	char currentValue;
	String rxPacket;
	int len = 0;
	
	while(Serial.available() > 0){
		currentValue = Serial.read();
		
		if(currentValue == PKT_END){
			break;
		}
		else{
			if(!initialized){
				rxPacket = String(currentValue);
				len += 1;
				initialized = true;
			}
			else{
				len += 1;
				rxPacket += currentValue;
			}
		}
	}
	
	//Packet must be something like "DvA IDIDID (1|2) #" so the new mode must be at pos 9 and packet len must always be 10!
	if(len == 10){
		if(rxPacket.startsWith("DvA")){
			if(rxPacket.charAt(9) == '1' || rxPacket.charAt(9) == '2')
				return rxPacket.charAt(9);
		}
	}
	
	return '0';
}
//TODO add functions to send and receive the packets from RF modules
/*void rf_tx(){
	
}

char* rf_rx(){
	
}*/

