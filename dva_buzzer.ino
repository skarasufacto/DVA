/*		dva_buzzer file
 *	Defines the vars and
 *	functions used to
 *	make the buzzer play
 *	sounds
 *--------------------------*/

#ifndef _BUZZ_CONST
#define BUZZ_PIN 2
#define _BUZZ_CONST
#endif

/*	buzz_setup function
 * Used to setup the buzzer
 * pin mode
 *---------------------------------
 * returns: void
 * ------------------------------*/
void buzz_setup(){
	pinMode(BUZZ_PIN, OUTPUT);
	digitalWrite(BUZZ_PIN, HIGH);
}

/*	play_beep function
 * Makes a "beep" sound of
 * custom duration and volume
 *---------------------------------
 * @duration: Duration of the 
 * 	played sound
 * --------------------------------
 * returns: void
 * ------------------------------*/
void play_beep(int duration){
	digitalWrite(BUZZ_PIN, LOW);
	delay(duration);
	digitalWrite(BUZZ_PIN, HIGH);
	delay(duration);
	digitalWrite(BUZZ_PIN, LOW);
	delay(duration);
	digitalWrite(BUZZ_PIN, HIGH);
}
