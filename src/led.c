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

void initLED() {
	DDRH |= BIT3; // red
	DDRE |= BIT3; // green
	DDRE |= BIT5; // blue
}

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
