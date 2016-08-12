/*		dva_gps file
 *	Defines the vars and
 *	functions used to
 *	interact with the gps
 * 	module
 *--------------------------*/

#ifnodef _SOFTWARE_SERIAL_INCLUDED
#include <SoftwareSerial.h>
#define _SOFTWARE_SERIAL_INCLUDED
#endif

#ifnodef _GPS_CONST
#define GPS_TX_PIN 6
#define GPS_RX_PIN 7
#define _GPS_CONST
#endif
