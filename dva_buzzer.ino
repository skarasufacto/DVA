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
}

/*	play_beep function
 * Makes a "beep" sound of
 * custom duration and volume
 *---------------------------------
 * @volume: Volume of the sound.
 * 	has to be set in a range of
 * 	0-255
 * @duration: Duration of the 
 * 	played sound
 * --------------------------------
 * returns: void
 * ------------------------------*/
void play_beep(int volume, int duration){
	analogWrite(BUZZ_PIN, volume);
	delay(duration);
	analogWrite(BUZZ_PIN, 0);
}
