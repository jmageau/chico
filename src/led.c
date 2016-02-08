/*! \file led.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Wifi card LED driver. This allows the color of the LED on the wifi card to be changed.
 */

/* Includes */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <avr/io.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "BitDefinitions.h"

#ifndef LED
#define LED

/*! \brief Initialization method that turns on the ports to be used.
 */
void initLED() {
	DDRH |= BIT3; // red
	DDRE |= BIT3; // green
	DDRE |= BIT5; // blue
}

/*! \brief Sets the color of the LED. Takes three booleans that correspond to RGB.
 *
 *  \param red Turn on/off red LED
 *  \param green Turn on/off green LED
 *  \param blue Turn on/off blue LED
 */
void setColor(bool red, bool green, bool blue) {

	if (red) {
		PORTH &= ~BIT3;
	} else {
		PORTH |= BIT3;
	}
	if (green) {
		PORTE &= ~BIT3;
	} else {
		PORTE |= BIT3;
	}
	if (blue) {
		PORTE &= ~BIT5;
	} else {
		PORTE |= BIT5;
	}
}

#endif
