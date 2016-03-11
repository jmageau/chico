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

#ifndef TIMER
#define TIMER

void (*isrCallback)();

bool red;
void initTimer(void (*isr)()) {
	isrCallback = isr;

	TIMSK3 &= ~_BV(TOIE1);
	TCCR3A = 0;
	TCCR3B = 0;

	TCNT3 = 34286;
	TCCR3B |= (1 << CS12);
	TIMSK3 |= (1 << TOIE1);   // enable timer overflow interrupt
}

ISR(TIMER3_OVF_vect) {
	TCNT3 = 34286;
	isrCallback();
}

#endif
