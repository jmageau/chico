/*!	\file lcdDisplay.h
 * \author ProjectTheodore
 * \date January 2016
 * \brief The LCD display module allows the printing of two lines of strings
 * to the Chico™ LCD.
 * \details Chico's LCD display can be activated and used by this module.
 * Simply make a call to "InitLCD" before starting to ensure that the LCD
 * communication is activated. Then, call "printLCD" with two strings of
 * 16 characters or less to print onto the LCD.
 *
 */

#ifndef LCDDISPLAY_H_
#define LCDDISPLAY_H_

/* --Includes-- */
#include <stdlib.h>

/* External functions */

void initLCD (void);
void printLCD (char *, char *);

#endif /* LCDDISPLAY_H_ */
