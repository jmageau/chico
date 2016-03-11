/*
 * behaviour.c
 *
 *  Created on: Mar 3, 2016
 *      Author: Nick
 */

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

#include "led.h"
#include "tempsensor.h"
#include "motion.h"
#include "lcd.h"
#include "servo.h"
#include "timer.h"
#include "behaviour.h"

#ifndef BEHAVIOUR
#define BEHAVIOUR

int timerTickCount;
int state;
int direction;

bool wheelSpeedUpdated;
bool wheelsUpdated;
bool lcdUpdated;
bool centerServoUpdated;
int centerServoDirection;
int centerServoUpdateTime;

void updateState();
void updateWheels();
void updateWheelSpeed();
void updateCenterServo();
void updateLCD();
void updateLED();

void timerCallback();

void initBehaviour() {
	initLED();
	initTPA81();
	LCDInit();
	servoInit();
	initTimer(timerCallback);

	state = MOVING_FORWARDS;
	timerTickCount = 0;

	wheelSpeedUpdated = false;
	lcdUpdated = false;
	centerServoUpdated = false;
	centerServoUpdateTime = 1;
	centerServoDirection = CLOCKWISE;

	wheelsUpdated = false;
}

void timerCallback() {
	updateState();
}

/*! \brief Task used to show the ambient temperature on the Wifi card LED.
 *
 *  The average temperature is gathered and then displayed on the LED. The color of the LED depends on the temperature.
 *
 *  \param *pvParameters
 */
void TaskMoveAndScan(void *pvParameters) {
	(void) pvParameters;

	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	while (1) {
		updateWheelSpeed();
		updateWheels();
		updateCenterServo();
		updateLCD();
		updateLED();
	}
}

void updateState() {
	if (timerTickCount % TIMER_FREQUENCY*STATE_TIME == 0 && timerTickCount != 0) {
		if (state != STOPPED) {
			state++;
		}
	}
	wheelSpeedUpdated = false;
	if (timerTickCount % 2 == 0){
		wheelsUpdated = false;
	}

	if (timerTickCount % 12 == 0){
		lcdUpdated = false;
	}

	if (timerTickCount % TIMER_FREQUENCY*centerServoUpdateTime == 0 && timerTickCount != 0){
		centerServoUpdated = false;
	}


	timerTickCount++;
}

void updateWheels() {
	if (!wheelsUpdated){
		if (state == MOVING_FORWARDS) {
				moveWheels(FORWARDS);
			} else if (state == MOVING_BACKWARDS) {
				moveWheels(BACKWARDS);
			} else if (state == MOVING_CLOCKWISE) {
				moveWheels(CLOCKWISE);
			} else if (state == MOVING_COUNTERCLOCKWISE) {
				moveWheels(COUNTERCLOCKWISE);
			} else { // state == STOPPED
				moveWheels(STOP);
			}
		wheelsUpdated = true;
	}
}

void updateWheelSpeed(){
	if (!wheelSpeedUpdated){
		getSpeed();
		wheelSpeedUpdated = true;
	}
}

void updateCenterServo() {
	if (state != STOPPED) {
		if (!centerServoUpdated) {
			if (centerServoDirection == CLOCKWISE){
				centerServoDirection = COUNTERCLOCKWISE;
			} else {
				centerServoDirection = CLOCKWISE;
			}
			moveCenterServo(centerServoDirection);
			centerServoUpdated = true;
		}
	} else {
		moveCenterServo(MIDDLE);
	}
}

void updateLCD() {
	if (!lcdUpdated) {
		readTemperatureValues();

		char display_top[16] = "";
		char display_bottom[16] = "";

		sprintf(display_bottom, "S:%2.2f D:%2.2f", getAverageSpeed(), getTotalDistance());

		sprintf(display_top, "A:%d R:%d L:%d", getAmbient(), getAverageLeft(),
				getAverageRight());
		LCDPrint(display_top, display_bottom);

		lcdUpdated = true;
	}
}

void updateLED() {
	if (state == MOVING_FORWARDS) {
		setColor(false, true, false);
	} else if (state == MOVING_BACKWARDS) {
		setColor(true, false, false);
	} else if (state == MOVING_CLOCKWISE || state == MOVING_COUNTERCLOCKWISE) {
		setColor(false, false, true);
	} else {
		setColor(true, true, true);
	}
}

#endif
