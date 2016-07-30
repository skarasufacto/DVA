/*		dva_buttons file
 *	Defines the main
 *	functions and vars to
 *	interact with the dva
 *	buttons
 *--------------------------*/

#ifndef _BUTTONS_CONST
#define CH_MODE_BUTTON_PIN 8
#define _BUTTONS_CONST
#endif

#ifndef _BUTTONS_IDS
#define CH_MODE_BUTTON 0
#define _BUTTONS_IDS
#endif

/*	buttons_setup function
 * Used to setup the pins that
 * the buttons will be using
 * --------------------------------
 * returns: void
 * ------------------------------*/
void buttons_setup(){
	pinMode(CH_MODE_BUTTON_PIN, INPUT);
}

/*	buttons_read function
 * Reads the actual value that
 * the desired button has
 * ---------------------------------
 * @button_id: The id of the button
 * 		we want to read data from
 * ---------------------------------
 * returns: value of the button
 * 		after successful read
 * -------------------------------*/
int buttons_read(int button_id){
	int res = LOW;
	
	switch(button_id){
		case CH_MODE_BUTTON :
			res = digitalRead(CH_MODE_BUTTON_PIN);
			break;
	}
	
	return res;
}
