/*! \file led.h
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Header file for the Wifi card LED driver.
 */

/* Includes */
#include <stdbool.h>

#ifndef LED
#define LED

/*! \brief Initialization method that turns on the ports to be used.
 */
void initLED();

/*! \brief Sets the color of the LED. Takes three booleans that correspond to RGB.
 *
 *  \param red Turn on/off red LED
 *  \param green Turn on/off green LED
 *  \param blue Turn on/off blue LED
 */
void setColor(bool red, bool green, bool blue);

#endif
