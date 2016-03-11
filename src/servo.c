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
#include "behaviour.h"

#ifndef SERVO
#define SERVO

double wheelSpeedRight;
double wheelSpeedLeft;

double previousSpeed;
double totalDistance[2];

void servoInit() {
	motion_init();
	wheelSpeedRight = 0;
	wheelSpeedLeft = 0;
	previousSpeed = 0;
	totalDistance[0] = 0;
	totalDistance[1] = 0;
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
		wheelSpeedRight = 0;
		wheelSpeedLeft = 0;
		previousSpeed = 0;
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

double getSpeedById(int deviceID){
	uint32_t* wheelTickDelta;

	if (motion_enc_read(deviceID, wheelTickDelta) == 1){
		//There was a new speed and we need to do something about it
		//0.0054 = distance of 1 capture event, 500ms = period of servos
		wheelSpeed =  0.0054 / ((*wheelTickDelta) * 0.0000005); //distance over time
		totalDistance[deviceID] = totalDistance[deviceID] + wheelSpeed * 1/TIMER_FREQUENCY;
	} else {
		wheelSpeed = previousSpeed;
	}

	previousSpeed = wheelSpeed;
	return wheelSpeed;
}

void getSpeed(){
	wheelSpeedLeft  = getSpeedById(MOTION_WHEEL_LEFT);
	wheelSpeedRight = getSpeedById(MOTION_WHEEL_RIGHT);
}

double getAverageSpeed(){
	return (wheelSpeedLeft + wheelSpeedRight)/2;
}

double getTotalDistance(){
	return (totalDistance[0] + totalDistance[1])/2;
}

#endif
