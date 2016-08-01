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
#define DVA_ID "000001"
#endif

//PKT management constants
#ifndef _PKT_MNG_CONST
#define PKT_END '#'
#define PREAMBLE "DvA"
#define PKT_TYPE_POS 9
#define PKT_ID_START_POS 3
#define PKT_ID_END_POS 8
#define PKT_CHMODE_LEN 11
#define PKT_GEO_LEN 26
#define PKT_LATITUDE_START_POS 10
#define PKT_LATITUDE_END_POS 17
#define PKT_LONGITUDE_START_POS 18
#define PKT_LONGITUDE_END_POS 25
#define _PKT_MNG_CONST
#endif

//PKT valid types
#ifndef _PKT_TYPES_CONST
#define BLE_CHMODE 1
#define BLE_UPDATELOCATION 2
#define RF_SEND_LOCATION 3
//TODO: Add more packet types here
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

//Geoposition structure definition
#ifndef _GEO_STRUCT
struct dva_position {
	double latitude;
	double longitude;
	//long lastUpdated;
};
#define _GEO_STRUCT
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

/*	rx_from_ble function
 * Reads Serial (bluetooth) and searches
 * for a valid packet.
 *---------------------------------
 * @pkt: packet structure used to
 * 		store the results of the
 * 		read process
 *---------------------------------
 * returns: true on successfull
 * 		read or false if error
 * ------------------------------*/
 boolean rx_from_ble(struct dva_pkt *pkt){
	 char currentValue;
	 boolean result = false;
	 pkt->initialized = false;
	 
	 while(Serial.available() > 0){
		currentValue = Serial.read();
		
		if(currentValue == PKT_END){
			break;
		}
		else{
			if(!pkt->initialized){
				pkt->data = String(currentValue);
				pkt->len = 1;
				pkt->initialized = true;
			}
			else{
				pkt->len += 1;
				pkt->data += currentValue;
			}
		}
	}
	
	//Packet must be something like "DvA IDIDID (packet_type) PAYLOAD #"; the smallest packet is the CHMODE packet
	if(pkt->len >= PKT_CHMODE_LEN){
		if(pkt->data.startsWith(PREAMBLE) && ((pkt->data.substring(PKT_ID_START_POS, PKT_ID_END_POS + 1)).equals(DVA_ID))){
			pkt->type = pkt->data.charAt(PKT_TYPE_POS) - '0';
			switch(pkt->type){
				case BLE_CHMODE :
					if(pkt->len == PKT_CHMODE_LEN){
						if(pkt->data.charAt(10) == '1' || pkt->data.charAt(10) == '2'){
							result = true;
						}
						else
							result = false;
					}
					else
						result = false;
					break;
				case BLE_UPDATELOCATION :
					//Packet len must be x;
					result = (pkt->len == PKT_GEO_LEN) ? true : false;
					break;
				default :
					result = false;
					break;
			}
		}
		else{
			result = false;
		}
	}
	
	return result;
 }
 
 
 void tx_rf(struct dva_position *pos, int pkt_type){
	 struct dva_pkt *pkt;
	 
	 pkt->type = 0;
	 
	 switch(pkt_type){
		 case RF_SEND_LOCATION :
			pkt->type = pkt_type;
			pkt->len = 26;
			pkt->data = PREAMBLE;
			pkt->data += DVA_ID;
			pkt->data += (char)(pkt_type + '0');
			pkt->data += parseDoubleToString(pos->latitude);
			pkt->data += parseDoubleToString(pos->longitude);
			pkt->data += '#';
			break;
	 }
	 
	 if(pkt->type != 0){
		vw_send((uint8_t*)(pkt->data.c_str()), pkt->len);
		vw_wait_tx();
	}
 }
 
 /*	pkt_uptade_geoposition function
 * Modifies geoposition given
 * 		a certain packet
 *---------------------------------
 * @pkt: packet structure used to
 * 		read data
 * @pos: geoposition structure;
 * 		will store the new
 * 		geoposition
 *---------------------------------
 * returns: void
 * ------------------------------*/
 void pkt_update_geoposition(struct dva_pkt *pkt, struct dva_position *pos){
	 pos->latitude = atof(pkt->data.substring(PKT_LATITUDE_START_POS, PKT_LATITUDE_END_POS +1).c_str());
	 pos->longitude = atof(pkt->data.substring(PKT_LONGITUDE_START_POS, PKT_LONGITUDE_END_POS + 1).c_str());
 }
 
  /*	parseDoubleToString function
 * Returns a custom string of
 * 	len 8
 *---------------------------------
 * @value: the double to parse
 *---------------------------------
 * returns: String value of
 * 		the double
 * ------------------------------*/
 String parseDoubleToString(double value){
	 if(value < 10.0){
		 //cut to 6
		 //return String(value, 6);
	 }
	 else if(value >= 10.0 && value < 100.0){
		 //cut to 5
		 //return String(value, 5);
	 }
	 else{
		 //cut to 4
		 //return String(value, 4); not working! TODO: convert strings
	 }
	 
	 return String(); //TEMP! remove when bugfixed!
 }
