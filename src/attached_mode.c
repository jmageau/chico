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

#define TEMP_THRESHOLD 30 //In Celsius
#define TRACK_SPEED    20

bool readHeat();
void trackHeat();

int currentCenterServoDirection;
int lastTrackedDirection;

bool targetFound;

//moveWheels
//lcd
//led
void initAttachedMode(){
	currentCenterServoDirection = CLOCKWISE;
	lastTrackedDirection = CLOCKWISE;
	targetFound = false;
}

bool readHeat() {
	return (getAverageLeft() > TEMP_THRESHOLD || getAverageRight() > TEMP_THRESHOLD);
}

void trackHeat() {
	int leftTemp = getAverageLeft();
	int rightTemp = getAverageRight();

	if (leftTemp > rightTemp && (leftTemp - rightTemp) > 2) {
		moveCenterServo(COUNTERCLOCKWISE, TRACK_SPEED * (leftTemp / rightTemp));
		lastTrackedDirection = COUNTERCLOCKWISE;
	} else if (rightTemp > leftTemp && (rightTemp - leftTemp) > 2) {
		moveCenterServo(CLOCKWISE, TRACK_SPEED * (rightTemp / leftTemp));
		lastTrackedDirection = CLOCKWISE;
	} else {
		moveCenterServo(STOP, 0);
	}
}

void updateCenterServoAttachedMode() {
	if (readHeat()) {
		trackHeat();
		targetFound = true;
	} else {
		targetFound = false;
		int currentPulseWidth = motion_servo_get_pulse_width(
				MOTION_SERVO_CENTER);

		if (currentPulseWidth <= MIN_PULSE_WIDTH_TICKS + 100) {
			currentCenterServoDirection = COUNTERCLOCKWISE;
			lastTrackedDirection = COUNTERCLOCKWISE;
		} else if (currentPulseWidth >= MAX_PULSE_WIDTH_TICKS - 100) {
			currentCenterServoDirection = CLOCKWISE;
			lastTrackedDirection = CLOCKWISE;
		} else {
			currentCenterServoDirection = lastTrackedDirection;
		}
		moveCenterServo(currentCenterServoDirection, SCAN_SPEED);
	}
}

void updateWheelsAttachedMode() {
	if (targetFound){
		int currentPulseWidth = motion_servo_get_pulse_width(MOTION_SERVO_CENTER);
		if (abs(currentPulseWidth - INITIAL_PULSE_WIDTH_TICKS) <= 100 ) {
			//TODO: forwards
		} else {
			moveWheels(currentCenterServoDirection, 0.1f);
		}
	} else {
		moveWheels(STOP, 1);
	}
}

