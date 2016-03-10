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

#include "led.h"

#ifndef TIMER
#define TIMER

bool red;
void (*isrCallback)();

bool red;
void initTimer(void (*isr)()) {
	isrCallback = isr;
	red = true;

	TIMSK3 &= ~_BV(TOIE1);
	TCCR3A = 0;
	TCCR3B = 0;

	TCNT3 = 30000;            // preload timer 65536-16MHz/256/8Hz
	TCCR3B |= (1 << CS12);    // 256 prescaler
	TIMSK3 |= (1 << TOIE1);   // enable timer overflow interrupt
}

ISR(TIMER3_OVF_vect) {
	TCNT3 = 30000;            // preload timer 65536-16MHz/256/2Hz
	isrCallback();
}

#endif
