#ifndef _DVA_H
/*	Possible DVA Roles:
 * 		'1' TX
 * 		'2' RX		*/
#define TX_ROLE '1'
#define RX_ROLE '2'

#define ARRAY_MAX_LEN 50

//PKT valid types
#define BLE_CHMODE 1
#define BLE_UPDATELOCATION 2
#define RF_SEND_LOCATION 3

//Buttons ids, used to read data
#define CH_MODE_BUTTON 0
#define NEXT_VICTIM_BUTTON 1
#define RESCUED_VICTIM_BUTTON 2

//Packet structure definition
typedef struct dva_pkts {
	int type;
	boolean initialized;
	String data;
	int len;
} Dva_pkt;

//DVA structure definition
typedef struct DVAs {
	boolean initialized;
	String id;
	long lastUpdated;
	float latitude;
	float longitude;

} Dva;

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

//Course constants
#ifndef _COURSE_CONST
#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7
#define _COURSE_CONST
#endif

#define _DVA_H
#endif
