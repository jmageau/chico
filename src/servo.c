/*
 * servo.c
 *
 *  Created on: Feb 24, 2016
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

#include "motion.h"
#include "servo.h"

#ifndef SERVO
#define SERVO

double previousSpeed;
double totalDistance;

void servoInit() {
	motion_init();
	previousSpeed = 0;
	totalDistance = 0;
}

void moveWheels(int direction) {
	if (direction == FORWARDS) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, MAX_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, MIN_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_WHEEL_RIGHT);

	} else if (direction == BACKWARDS) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, MIN_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, MAX_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_WHEEL_RIGHT);

	} else if (direction == CLOCKWISE) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, MAX_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, MAX_PULSE_WIDTH_TICKS);
	    motion_servo_start(MOTION_WHEEL_RIGHT);

	}else if (direction == COUNTERCLOCKWISE) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, MIN_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, MIN_PULSE_WIDTH_TICKS);
	    motion_servo_start(MOTION_WHEEL_RIGHT);

	} else if (direction == STOP) {
		motion_servo_stop(MOTION_WHEEL_RIGHT);
		motion_servo_stop(MOTION_WHEEL_LEFT);
	}

}

void moveCenterServo(int direction) {
	if (direction == CLOCKWISE) {
		motion_servo_set_pulse_width(MOTION_SERVO_CENTER,
		MIN_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_SERVO_CENTER);
	} else if (direction == COUNTERCLOCKWISE) {
		motion_servo_set_pulse_width(MOTION_SERVO_CENTER,
		MAX_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_SERVO_CENTER);
	} else if (direction == MIDDLE) {
		motion_servo_set_pulse_width(MOTION_SERVO_CENTER,
		((MAX_PULSE_WIDTH_TICKS - MIN_PULSE_WIDTH_TICKS) / 2) + MIN_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_SERVO_CENTER);
	} else if (direction == STOP) {
		motion_servo_start(MOTION_SERVO_CENTER);
	}
}

//0.0054 is the distance travelled for two fronts (taken from motion module doc)
//0.0000005 is 500ns in seconds
//
//I'm calculating speed by doing v=d/t, while converting the ticks into seconds

double getSpeed(int deviceID){
	uint32_t* wheelTickDelta;

	double wheelSpeed;
	if (motion_enc_read(deviceID, wheelTickDelta) == 1){
		//There was a new speed and we need to do something about it
		wheelSpeed =  0.0054 / ((*wheelTickDelta) * 0.0000005); //distance over time
		totalDistance = totalDistance + 0.0054;
	} else {
		wheelSpeed = previousSpeed;
	}

	previousSpeed = wheelSpeed;
	return wheelSpeed;
}

double getAverageSpeed(){
	return (getSpeed(MOTION_WHEEL_LEFT) + getSpeed(MOTION_WHEEL_RIGHT))/2;
}

double getTotalDistance(){
	return totalDistance;
}

#endif
