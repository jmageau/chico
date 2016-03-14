/*! \file lcd.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		LCD driver for Chico
 */

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "usartSerial.h"

#include "BitDefinitions.h"

#ifndef LCD
#define LCD

#define LCD_ADDRESS USART1_ID
#define LCD_COMMAND_ON 0x0C
#define LCD_COMMAND_CLEAR 0x01
#define LCD_COMMAND_TOP 0x80
#define LCD_COMMAND_BOTTOM 0xC0

/*! \brief Sends a command to the LCD component.
 *
 *  \param command
 *  	The command being sent to the LCD (ex: clear, on, top, bottom)
 */
void sendLCDCommand(int command);

/*! \brief Initializes the LCD component by turning it on and clearing it.
 */
void LCDInit() {
  usartOpen(LCD_ADDRESS, 9600, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
  DDRD |= BIT3;
  PORTD |= BIT3;
  sendLCDCommand(LCD_COMMAND_ON);
  sendLCDCommand(LCD_COMMAND_CLEAR);
}

/*! \brief Prints two char arrays on the LCD, one for the top row and one for the bottom row.
 *     \param top
 *  	Char array being printed on the top row
 *     \param bottom
 *  	Char array being printed on the bottom row
 */
void LCDPrint(char* top, char* bottom) {
  sendLCDCommand(LCD_COMMAND_CLEAR);
  sendLCDCommand(LCD_COMMAND_TOP);
  usart_fprint(LCD_ADDRESS, (uint8_t*) top);
  sendLCDCommand(LCD_COMMAND_BOTTOM);
  usart_fprint(LCD_ADDRESS, (uint8_t*) bottom);
}

/*! \brief Sends a command to the LCD component.
 *
 *  \param command
 *  	The command being sent to the LCD (ex: clear, on, top, bottom)
 */
void sendLCDCommand(int command) {
  usartWrite(LCD_ADDRESS, 0xFE);
  usartWrite(LCD_ADDRESS, command);
}

#endif
