/*		dva_lcd file
 *	Defines the vars and
 *	functions used to
 *	display information and
 *	data on lcd screen
 *--------------------------*/

//Include and defines
#ifndef _INCLUDED_LCD
#include <LiquidCrystal_I2C.h>
#define _INCLUDED_LCD
#endif

#define BACKLIGHT_PIN 3

//variables
LiquidCrystal_I2C	lcd(0x27,2,1,0,4,5,6,7);

/*	lcd_setup function
 * Used to setup the lcd and
 * it's variables
 *---------------------------------
 * returns: void
 * ------------------------------*/
void lcd_setup(){
	lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
	lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
    lcd.begin(16, 2);
}

/*	lcd_print function
 * Used to print data on the
 * lcd screen; also resets
 * the screen on call
 *---------------------------------
 * @data: characters the lcd
 * 		will print
 * --------------------------------
 * returns: void
 * ------------------------------*/
void lcd_print(char* data){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.println(data);
}
