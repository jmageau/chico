/*! \file servo.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Module responsible for preparing commands for the servos. Uses the motion module.
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
#include "timer.h"
#include "tempsensor.h"

#ifndef SERVO
#define SERVO

/*! \brief Speed of the right wheel
 */
double wheelSpeedRight;
/*! \brief Speed of the left wheel
 */
double wheelSpeedLeft;

/*! \brief The speed of the right wheel at the previous clock tick
 */
double previousSpeedRight;
/*! \brief The speed of the left wheel at the previous clock tick
 */
double previousSpeedLeft;
/*! \brief Total distance for the left and right wheels.
 */
double totalDistance[2];

/*! \brief Initializes the motion module and sets all speeds/distances to zero.
 */
void servoInit() {
	motion_init();
	wheelSpeedRight = 0;
	wheelSpeedLeft = 0;
	previousSpeedRight = 0;
	previousSpeedLeft = 0;
	totalDistance[0] = 0;
	totalDistance[1] = 0;
}

/*! \brief Calculates the pulse width necessary to move a servo at a certain power level.
 *
 * \param direction
 * 		Direction of the servo. 1 if going clockwise, 0 if going counterclockwise
 * 	\param power
 * 		Percentage of the power the wheel should be turning at. If power is 50%, then the wheel should turn 50% slower
 */
int getPulseWidth(bool direction, double power){
	if(direction){
		return (int)(INITIAL_PULSE_WIDTH_TICKS + (MAX_PULSE_WIDTH_TICKS - INITIAL_PULSE_WIDTH_TICKS) * power);
	}
	return (int)(INITIAL_PULSE_WIDTH_TICKS - (INITIAL_PULSE_WIDTH_TICKS - MIN_PULSE_WIDTH_TICKS) * power);
}

/*! \brief Sends move commands to the wheels depending on the direction.
 *
 * \param direction
 * 	The direction the wheels need to move in.
 */
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

/*! \brief Sends move commands to the center servo depending on the direction
 *
 * \param direction
 * 	The direction the center servo needs to move in.
 */
void moveCenterServo(int direction, int speed) {
	int currentPulseWidth = motion_servo_get_pulse_width(MOTION_SERVO_CENTER);
	if (direction == CLOCKWISE) {
		motion_servo_set_pulse_width(MOTION_SERVO_CENTER,
		currentPulseWidth - speed);
		motion_servo_start(MOTION_SERVO_CENTER);
	} else if (direction == COUNTERCLOCKWISE) {
		motion_servo_set_pulse_width(MOTION_SERVO_CENTER,
		currentPulseWidth + speed);
		motion_servo_start(MOTION_SERVO_CENTER);
	} else if (direction == MIDDLE) {
		motion_servo_set_pulse_width(MOTION_SERVO_CENTER,
		((MAX_PULSE_WIDTH_TICKS - MIN_PULSE_WIDTH_TICKS) / 2) + MIN_PULSE_WIDTH_TICKS);
		motion_servo_start(MOTION_SERVO_CENTER);
	} else if (direction == STOP) {
		motion_servo_stop(MOTION_SERVO_CENTER);
	}
}

/*! \brief Get the speed of a specific wheel
 *
 * \param deviceID
 * 		What wheel's speed to calculate
 * \param previousSpeed
 * 		The previous speed of the wheel
 *
 */
double getSpeedById(int deviceID, double previousSpeed){
	uint32_t* wheelTickDelta;
	double wheelSpeed;

	if (motion_enc_read(deviceID, wheelTickDelta) == 1){
		//There was a new speed and we need to do something about it
		//0.0054 = distance of 1 capture event, 500ns = period of servos
		wheelSpeed = CAPTURE_EVENT_DISTANCE / ((*wheelTickDelta) * SERVO_PERIOD); //distance over time
	} else {
		wheelSpeed = previousSpeed;
	}
	totalDistance[deviceID] = totalDistance[deviceID] + wheelSpeed * 1/TIMER_FREQUENCY;

	return wheelSpeed;
}


/*! \brief Calculates the speeds of the wheels
 */
void getSpeed(){
	wheelSpeedLeft  = getSpeedById(MOTION_WHEEL_LEFT, previousSpeedLeft);
	wheelSpeedRight = getSpeedById(MOTION_WHEEL_RIGHT, previousSpeedRight);
	previousSpeedLeft = wheelSpeedLeft;
	previousSpeedRight = wheelSpeedRight;
}

/*! \brief Returns the average speed of the wheels
 */
double getAverageSpeed(){
	return (wheelSpeedLeft + wheelSpeedRight)/2;
}

/*! \brief Returns the total distance traveled by the wheels
 */
double getTotalDistance(){
	return (totalDistance[0] + totalDistance[1])/2;
}

#endif
