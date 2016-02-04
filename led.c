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

#ifndef LED
#define LED

static void init() {
	DDRH |= _BV(DDH3); // red
	DDRE |= _BV(DDE3); // green
	DDRE |= _BV(DDE5); // blue
}

void setColor(bool red, bool green, bool blue) {

	if (red) {
		PORTH &= ~_BV(PORTH3);
	} else {
		PORTH |= _BV(PORTH3);
	}
	if (green) {
		PORTE &= ~_BV(PORTE3);
	} else {
		PORTE |= _BV(PORTE3);
	}
	if (blue) {
		PORTE &= ~_BV(PORTE5);
	} else {
		PORTE |=  _BV(PORTE5);
	}
}

#endif