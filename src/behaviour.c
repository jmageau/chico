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

int state;
int stateTime;
int direction;
bool lastCenterServoDirection;

void updateState();
void updateWheels();
void updateCenterServo();
void updateLCD();
void updateLED();

void testUpdateLCD();

void initBehaviour() {
	initLED();
	initTPA81();
	LCDInit();
	servoInit();
	initTimer();

	state = MOVING_FORWARDS;
	stateTime = 4;

	lastCenterServoDirection = false;
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
    //Do * 100 to get the time in milliseconds
	int taskTime = ((TASK_TICK_TIME * 1000) / portTICK_PERIOD_MS);

	while (1) {
		readTemperatureValues();
		vTaskDelayUntil(&xLastWakeTime, taskTime);

		updateState();

		updateWheels();
		updateCenterServo();
		//updateLCD();
		testUpdateLCD();
		updateLED();

		incrementTimer();
	}
}

void updateState(){
	int time = getTime();
	if (time != 0 && time % 4 == 0 && state != STOPPED){
		state++;
	}
}

void updateWheels(){
	if (state == MOVING_FORWARDS){
		moveWheels(FORWARDS);
	} else if (state == MOVING_BACKWARDS){
		moveWheels(BACKWARDS);
	} else if (state == MOVING_CLOCKWISE){
		moveWheels(CLOCKWISE);
	} else if (state == MOVING_COUNTERCLOCKWISE){
		moveWheels(COUNTERCLOCKWISE);
	} else { // state == STOPPED
		moveWheels(STOP);
	}
}

void updateCenterServo(){
	if (state != STOPPED){
		if (lastCenterServoDirection){
			moveCenterServo(CLOCKWISE);
		} else {
			moveCenterServo(COUNTERCLOCKWISE);
		}
		lastCenterServoDirection = !lastCenterServoDirection;
	} else {
		moveCenterServo(MIDDLE);
	}
}

void updateLCD() {
	char display_top[16] = "";
	char display_bottom[16] = "";

	sprintf(display_bottom, "S:%2.2f D:%2.2f", getAverageSpeed(), getTotalDistance());

	sprintf(display_top, "A:%d R:%d L:%d", getAmbient(), getAverageLeft(),
			getAverageRight());
	LCDPrint(display_top, display_bottom);
}

void testUpdateLCD() {
	char display_top[16] = "";
	char display_bottom[16] = "";

	sprintf(display_bottom, "S:%2.2f D:%2.2f", getAverageSpeed(), getTotalDistance());

	sprintf(display_top, "A:%d R:%d L:%d", getAmbient(), getAverageLeft(),
			getAverageRight());
	LCDPrint(display_top, display_bottom);
}

void updateLED(){
	if (state == MOVING_FORWARDS){
		setColor(false, true, false);
	} else if (state == MOVING_BACKWARDS){
		setColor(true, false, false);
	} else if (state == MOVING_CLOCKWISE || state == MOVING_COUNTERCLOCKWISE){
		setColor(false, false, true);
	} else {
		setColor(true, true, true);
	}
}

#endif
