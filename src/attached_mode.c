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
#include "servo.h"
#include "tempsensor.h"
#include "behaviour.h"
#include "custom_timer.h"
#define TEMP_THRESHOLD 50 //In Celsius
#define TRACK_SPEED    20
bool readHeat();
void trackHeat();
int currentCenterServoDirection;
int lastTrackedDirection;
unsigned long initialAttachTime;
unsigned long currentTime;
//moveWheels
//lcd
//led
void initAttachedMode() {
	currentState = SEARCHING;
	currentCenterServoDirection = CLOCKWISE;
	lastTrackedDirection = currentCenterServoDirection;
	initialAttachTime = time_in_milliseconds();
}

bool readHeat() {
	return (getAverageLeft() > TEMP_THRESHOLD
			|| getAverageRight() > TEMP_THRESHOLD);
}
void updateCenterServoAttachedMode() {
	if (currentState == SEARCHING) {
		moveCenterServo(SCAN, SCAN_SPEED);

		if (readHeat()) {
			currentState = ATTACHED;
		} else if (currentTime - initialAttachTime >= 100000) { // change to if maybe
			currentState = PANICKING;
		}
	} else if (currentState == ATTACHED) {
		//TODO
	} else if (currentState == PANICKING) {
		moveCenterServo(MIDDLE, SCAN_SPEED);
	}
	currentTime = time_in_milliseconds();
}

void updateWheelsAttachedMode() {
	if (currentState == PANICKING) {
		moveWheels(COUNTERCLOCKWISE, 0.1f);
		if (currentTime - initialAttachTime >= 60000) {
			initAttachedMode();
		}
	} else if (currentState == SEARCHING) {
		//TODO:
	} else if (currentState == ATTACHED) {
		//TODO:
	}
}
