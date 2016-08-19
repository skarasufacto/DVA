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
#define PKT_MAX_PRESITION 8
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
	
	vw_rx_start();
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
	 
	 while(Serial1.available() > 0){
		currentValue = Serial1.read();
		
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
 
 /*	tx_to_ble function
  * Sends data via Bluetoth
  * -------------------------------
  * @pkt: The packet we will send
  * -------------------------------
  * returns: void
  * -----------------------------*/
 void tx_to_ble(struct dva_pkt *pkt){
	 Serial1.print(pkt->data.c_str());
 }
 
 /*	tx_rf function
  * Sends data via Bluetoth
  * -------------------------------
  * @pos: The dva structure we
  * 	will read the geoposition
  * 	information from
  * @pkt: The packet we will send
  * -------------------------------
  * returns: void
  * -----------------------------*/
 void tx_rf(struct dva *pos, int pkt_type){
	 struct dva_pkt *pkt;
	 
	 pkt->type = 0;
	 
	 switch(pkt_type){
		 case RF_SEND_LOCATION :
			pkt->type = pkt_type;
			pkt->len = 26;
			pkt->data = PREAMBLE;
			pkt->data += DVA_ID;
			pkt->data += (char)(pkt_type + '0');
			pkt->data += parseFloatToString(pos->latitude);
			pkt->data += parseFloatToString(pos->longitude);
			pkt->data += '#';
			break;
	 }

	 if(pkt->type != 0){
		vw_send((uint8_t*)(pkt->data.c_str()), pkt->len);
		vw_wait_tx();
	}
 }
 
  /*	rx_rf function
 * Reads rf and searches
 * for a valid packet.
 *---------------------------------
 * @pkt: packet structure used to
 * 		store the results of the
 * 		read process
 *---------------------------------
 * returns: true on successfull
 * 		read or false if error
 * ------------------------------*/
 boolean rx_rf(struct dva_pkt *pkt){
	 boolean foundEnd = false;
	 uint8_t message[VW_MAX_MESSAGE_LEN];
	 uint8_t messageLength = VW_MAX_MESSAGE_LEN;

	 if(vw_get_message(message, &messageLength)){
		 for(int i = 0; i < messageLength; i++){
			 if(foundEnd){
				 //We want to clear the buffer even if the end was found
				 continue;
			 }
			 else{
				 if(i == 0){
					pkt->data = String((char)message[i]);
					pkt->len = 1;
				}
				else if(message[i] == PKT_END){
					foundEnd = true;
				}
				 else{
					pkt->data += (char)message[i];
					pkt->len += 1;
				}
			}
		 }
	 }
	 else{
		 return false;
	 }
	 
	 if(pkt->len == 26){
		 if(pkt->data.startsWith(PREAMBLE) && (pkt->data.charAt(PKT_TYPE_POS) - '0') == RF_SEND_LOCATION){
			 return true;
		 }
		 else{
			 return false;
		 }
	 }
	 else{
		 return false;
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
 void pkt_update_geoposition(struct dva_pkt *pkt, struct dva *pos){
	 pos->latitude = atof(pkt->data.substring(PKT_LATITUDE_START_POS, PKT_LATITUDE_END_POS +1).c_str());
	 pos->longitude = atof(pkt->data.substring(PKT_LONGITUDE_START_POS, PKT_LONGITUDE_END_POS + 1).c_str());
 }
 
  /*	parseFloatToString function
 * Returns a custom string of
 * 	len 8
 *---------------------------------
 * @value: the float to parse
 *---------------------------------
 * returns: String value of
 * 		the float
 * ------------------------------*/
 String parseFloatToString(float value){

	 String result = String((int) value);
	 float temp = value - (float)((int)value);
	 
	 result += '.';
	 
	 for(int i = result.length() + 1; i <= PKT_MAX_PRESITION; i++){
		 if(temp == 0.0){
			 result += '0';
		 }
		 else{
			 temp *= 10.0;
			 result += (char)((int) temp + '0');
			 temp = temp - (float)((int)temp);
		 }
	 }

	 return result;
 }
