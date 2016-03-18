/*! \file timer.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Timer used to time various method calls for Chico.
 */

/* Includes */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "BitDefinitions.h"

#include <avr/io.h>

#include "timer.h"

#ifndef TIMER
#define TIMER

/*! \brief Amount of times the timer has ticked since being initialized
 */
int timerTickCount;

/*! \brief The method that will be called when there is an interrupt/
 */
void (*isrCallback)();

/*! \brief Initializes the timer.
 */
void initTimer(void (*isr)()) {
	timerTickCount = 0; //start logical timer at 0
	isrCallback = isr; // Set the callback method

	TIMSK3 &= ~_BV(TOIE1); // disable timer overflow interrupts
	TCCR3A = 0;
	TCCR3B = 0;

	TCNT3 = TIMER_PRELOAD;
	TCCR3B |= (1 << CS12);    // set prescaler
	TIMSK3 |= (1 << TOIE1);   // enable timer overflow interrupts
}

int getTime(){
	return timerTickCount;
}

bool isTimerRepeatMultiple(int frequency){
	return timerTickCount % TIMER_FREQUENCY*frequency == 0 && timerTickCount != 0;
}

/*! \brief Method called when there is an interrupt. Will call the stored isrCallback method.
 */
ISR(TIMER3_OVF_vect) {
	timerTickCount++;
	TCNT3 = TIMER_PRELOAD;
	isrCallback();
}

#endif
