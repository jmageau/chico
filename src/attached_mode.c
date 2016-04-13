
#include "BitDefinitions.h"
#include "servo.h"
#include "tempsensor.h"
#include "motion.h"
#include "ping.h"
#include "behaviour.h"
#include "custom_timer.h"

#include "led.h"

#define TEMP_THRESHOLD 31 //In Celsius

bool readHeat();
void trackHeat();

int currentCenterServoDirection;
int lastTrackedDirection;

unsigned long initialAttachTime;
unsigned long currentTime;

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
	currentTime = time_in_milliseconds();

	if (currentState == SEARCHING) {
		moveCenterServo(SCAN, SCAN_SPEED);

		if (readHeat()) {
			currentState = ATTACHED;
		} else if (currentTime - initialAttachTime >= 1000000) { // change to if maybe
			currentState = PANICKING;
			setColor(true,false,false);
		}
	} else if (currentState == ATTACHED) {
		if (readHeat()) {
			int leftTemperature = getAverageLeft();
			int rightTemperature = getAverageRight();
			int middleTemperature = getAverageMiddle();
			if(leftTemperature > rightTemperature && leftTemperature > middleTemperature + 2) {
				moveCenterServo(COUNTERCLOCKWISE, SCAN_SPEED);
			} else if(rightTemperature > leftTemperature && rightTemperature > middleTemperature + 2) {
				moveCenterServo(COUNTERCLOCKWISE, SCAN_SPEED);
			} else {
				// nothing
			}
		} else {
			currentState = SEARCHING;
		}
	} else if (currentState == PANICKING) {
		moveCenterServo(MIDDLE, SCAN_SPEED);
	}
}

void updateWheelsAttachedMode() {
	if (currentState == PANICKING) {
		moveWheels(COUNTERCLOCKWISE, 1);
		if (currentTime - initialAttachTime >= 60000) {
			initAttachedMode();
		}
	} else if (currentState == SEARCHING) {
		//TODO:
		moveWheels(STOP, 0);
	} else if (currentState == ATTACHED) {
		int centerServoAngle = motion_servo_get_pulse_width(MOTION_SERVO_CENTER)
				- INITIAL_PULSE_WIDTH_TICKS;
		if (abs(centerServoAngle) < 100) {
			if (getDistance() > 100) {
				moveWheels(FORWARDS, 1);
			} else if (getDistance() < 80) {
				//moveWheels(BACKWARDS, 1);
			}
		} else {
			if (centerServoAngle > 0) {
				moveWheels(COUNTERCLOCKWISE, 1);
			} else {
				moveWheels(CLOCKWISE, 1);
			}
		}
	}
}
