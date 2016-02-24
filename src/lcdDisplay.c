/*!	\file lcdDisplay.c
 * \author ProjectTheodore
 * \date January 2016
 * \brief The LCD display module allows the printing of two lines of strings
 * to the Chico™ LCD.
 * \details Chico's LCD display can be activated and used by this module.
 * Simply make a call to "InitLCD" before starting to ensure that the LCD
 * communication is activated. Then, call "printLCD" with two strings of
 * 16 characters or less to print onto the LCD.
 */

/* --Includes-- */

#include <stdio.h>

#include "FreeRTOS.h"
#include "usartSerial.h"

#include "lcdDisplay.h"

#define LCD_ADDRESS USART1_ID // The USART ID of the LCD for communication.
#define SEND_COMMAND 0xFE // Hex needed to notify LCD of command incoming.
#define CLEAR_LCD 0x01 // Command: clear the LCD.
#define FIRST_LINE 0x80 // Command: go to first line's beginning character.
#define SECOND_LINE 0xC0 // Command: go to second line's beginning character.
#define TURN_ON_DISPLAY 0x0C // Command: turn on display.

/* Function Declarations */

void lcdCommand (int);

/*!\brief Initialize the LCD for communication, called before any printing
 * can be done.
 *
 *\details This function must be called once before trying to print on the LCD,
 * or else the LCD will not receive commands sent to it. In technical detail,
 * to be able to communicate with and print on the LCD, we have to set the
 * Data Register DDRD, and then set PortD pin 3 as output compare, since the
 * LCD is connected to pin 3 on Chico™.
 *
 */
void initLCD (void) {
	// Open the USART port for writing.
	usartOpen(LCD_ADDRESS, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
	// Set the data register for the LCD.
	DDRD |= _BV(DDD3);
	// Point at pin 3, where the LCD is connected.
	PORTD |= _BV(PORTD3);
	// Turn on the display.
	lcdCommand(TURN_ON_DISPLAY);
	// Clear the LCD, in case it has any previous content.
	lcdCommand(CLEAR_LCD);
}

/*!\brief Print to the LCD on both lines.
 *
 *\details Clears the LCD, then prints the two C strings given. Each string
 * should be equal or less than 16 characters to fit on the LCD. We print
 * str_top on the first line of the LCD, and str_bottom on the second line.
 *
 * @param str_top C string, 16 characters or less, to print on the first line
 * of the LCD.
 * @param str_bottom C string, 16 characters or less, to print on the bottom
 * line of the LCD.
 */
void printLCD (char *str_top, char *str_bottom) {
	lcdCommand(CLEAR_LCD); // Start by clearing the LCD.
	lcdCommand(FIRST_LINE); // Go to the first line.
	// Print out the string for the top:
	usart_fprint(LCD_ADDRESS, (uint8_t *) str_top);
	lcdCommand(SECOND_LINE); // Go to the second line.
	// Print out the string for the bottom:
	usart_fprint(LCD_ADDRESS, (uint8_t *) str_bottom);
}

/* Local functions */

/*!\brief Send a command to the LCD.
 *
 *\details Send a given command to the LCD. First, we tell the LCD that we are
 * sending a command, and then we send it. This allows us to clear the LCD,
 * move the cursor, turn it on and off, and more.
 *
 * @param command Integer of the command to be performed.
 */
void lcdCommand (int command) {
	// Notify the LCD that we are going to send a command.
	usartWrite(LCD_ADDRESS, SEND_COMMAND);
	// Send the given command.
	usartWrite(LCD_ADDRESS, command);
}
