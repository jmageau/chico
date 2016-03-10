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

void initTimer(void (*isr)()) {

	isrCallback = isr;

	TIMSK1 &= ~_BV(TOIE1);
	TCCR1A = 0;
	TCCR1B = 0;

	TCNT1 = 57723;            // preload timer 65536-16MHz/256/8Hz
	TCCR1B |= (1 << CS12);    // 256 prescaler
	TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
}

ISR(TIMER1_OVF_vect) {
	TCNT1 = 57723;
	isrCallback();
}

#endif
