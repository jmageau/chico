/*! \file behaviour.h
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Main control module for Chico. This module will handle all of Chico's behaviours and states.
 */

#ifndef SRC_BEHAVIOUR_H_
#define SRC_BEHAVIOUR_H_

#define MOVING_FORWARDS           0
#define MOVING_BACKWARDS          1
#define MOVING_CLOCKWISE          2
#define MOVING_COUNTERCLOCKWISE   3
#define STOPPED                   4
#define ATTACHED                  5
#define SEARCHING                 6
#define PANICKING                 7

#define SCAN_SPEED                50

int currentState;

#endif /* SRC_BEHAVIOUR_H_ */
