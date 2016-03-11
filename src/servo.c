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

double previousSpeedRight;
double previousSpeedLeft;

double totalDistance[2];

void servoInit() {
	motion_init();
	wheelSpeedRight = 0;
	wheelSpeedLeft = 0;
	previousSpeedRight = 0;
	previousSpeedLeft = 0;
	totalDistance[0] = 0;
	totalDistance[1] = 0;
}

int getPulseWidth(bool direction, double power){
	int PULSE_DELTA = MAX_PULSE_WIDTH_TICKS - MIN_PULSE_WIDTH_TICKS;

	if(direction){
		return (int)(MIN_PULSE_WIDTH_TICKS + (PULSE_DELTA / 2) + (PULSE_DELTA / 2) * power);
	}
	return (int)(MIN_PULSE_WIDTH_TICKS + (PULSE_DELTA / 2) - (PULSE_DELTA / 2) * power);
}

void moveWheels(int direction) {
	float leftPowerRatio;
	float rightPowerRatio;

	//normalize speed
	if (previousSpeedRight > previousSpeedLeft && previousSpeedRight != 0) {
		leftPowerRatio = 1;
		rightPowerRatio = previousSpeedLeft / previousSpeedRight;
	} else if (previousSpeedLeft > previousSpeedRight && previousSpeedLeft != 0) {
		leftPowerRatio = previousSpeedRight / previousSpeedLeft;
		rightPowerRatio = 1;
	}
	else {
		leftPowerRatio = 1;
		rightPowerRatio = 1;
	}

	if (direction == FORWARDS) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, getPulseWidth(1, leftPowerRatio));
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, getPulseWidth(0, rightPowerRatio));
		motion_servo_start(MOTION_WHEEL_RIGHT);

	} else if (direction == BACKWARDS) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, getPulseWidth(0, leftPowerRatio));
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, getPulseWidth(1, rightPowerRatio));
		motion_servo_start(MOTION_WHEEL_RIGHT);

	} else if (direction == CLOCKWISE) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, getPulseWidth(1, leftPowerRatio));
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, getPulseWidth(1, rightPowerRatio));
	    motion_servo_start(MOTION_WHEEL_RIGHT);

	}else if (direction == COUNTERCLOCKWISE) {

		motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, getPulseWidth(0, leftPowerRatio));
		motion_servo_start(MOTION_WHEEL_LEFT);

		motion_servo_set_pulse_width(MOTION_WHEEL_RIGHT, getPulseWidth(0, rightPowerRatio));
	    motion_servo_start(MOTION_WHEEL_RIGHT);

	} else if (direction == STOP) {
		motion_servo_stop(MOTION_WHEEL_RIGHT);
		motion_servo_stop(MOTION_WHEEL_LEFT);
		previousSpeedRight = 0;
		previousSpeedLeft  = 0;
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

double getSpeedById(int deviceID, double previousSpeed){
	uint32_t* wheelTickDelta;
	double wheelSpeed;

	if (motion_enc_read(deviceID, wheelTickDelta) == 1){
		//There was a new speed and we need to do something about it
		//0.0054 = distance of 1 capture event, 500ns = period of servos
		wheelSpeed =  0.0054 / ((*wheelTickDelta) * 0.0000005); //distance over time
		totalDistance[deviceID] = totalDistance[deviceID] + wheelSpeed * 1/TIMER_FREQUENCY;
	} else {
		wheelSpeed = previousSpeed;
	}

	return wheelSpeed;
}

void getSpeed(){
	wheelSpeedLeft  = getSpeedById(MOTION_WHEEL_LEFT, previousSpeedLeft);
	wheelSpeedRight = getSpeedById(MOTION_WHEEL_RIGHT, previousSpeedRight);
	previousSpeedLeft = wheelSpeedLeft;
	previousSpeedRight = wheelSpeedRight;
}

double getAverageSpeed(){
	return (wheelSpeedLeft + wheelSpeedRight)/2;
}

double getTotalDistance(){
	return (totalDistance[0] + totalDistance[1])/2;
}

#endif
