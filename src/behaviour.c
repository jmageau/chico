/*! \file behaviour.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Main control module for Chico. This module will handle all of Chico's behaviours and states.
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

/*! \brief Amount of times the timer has ticked since being initialized
 */
int timerTickCount;
/*! \brief The current state of Chico.
 */
int state;


/*! \brief Whether or not the wheel speed need to be calculated
 */
bool wheelSpeedUpdated;
/*! \brief Whether or not wheels need to be updated
 */
bool wheelsUpdated;
/*! \brief Whether or not the LCD needs to be updated
 */
bool lcdUpdated;
/*! \brief Whether or not center servo needs to be updated
 */
bool centerServoUpdated;
/*! \brief Current direction the center servo is going in
 */
int centerServoDirection;

/*! \brief Updates the state of Chico and checks if the components need to be updated. Is called at every timer tick (10 ms)
 */
void updateState();
/*! \brief Updates the wheels. The wheels will change direction based on the state.
 */
void updateWheels();
/*! \brief Calculates the wheel speed. The wheel speed needs to be calculated often to maintain accuracy and to keep track of distance.
 */
void updateWheelSpeed();
/*! \brief Updates the center servo. The center servo will scan clockwise and counterclockwise until Chico reaches the end state.
 */
void updateCenterServo();
/*! \brief Updates the LCD. The LCD shows the temperatures mesured by the sensor, the current speed and the total distance travelled.
 */
void updateLCD();
/*! \brief Updates the LED. The LED will change color depending on Chico's state.
 */
void updateLED();
/*! \brief Will be called by the timer's ISR method at every timer tick (10ms).
 */
void timerCallback();

/*! \brief Will initialize all the components and create the starting state of Chico.
 */
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
	wheelsUpdated = false;
	centerServoDirection = CLOCKWISE;
}

/*! \brief Will be called by the timer's ISR method at every timer tick (10ms).
 */
void timerCallback() {
	updateState();
}

/*! \brief Task responsible for moving Chico in a predefined pattern and scanning with the center servo.
 *
 *  Chico will move forwards, backwards, clockwise, then counterclockwise. While moving, the center servo will scan left and right.
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

/*! \brief Updates the state of Chico and checks if the components need to be updated. Is called at every timer tick (10 ms)
 */
void updateState() {
	if (timerTickCount % TIMER_FREQUENCY*STATE_TIME == 0 && timerTickCount != 0) {
		if (state != STOPPED) {
			state++;
		}
	}
	wheelSpeedUpdated = false;

	if (timerTickCount % TIMER_FREQUENCY*WHEEL_UPDATE_TIME == 0){ //0.02
		wheelsUpdated = false;
	}

	if (timerTickCount % TIMER_FREQUENCY*LCD_UPDATE_TIME == 0){
		lcdUpdated = false;
	}

	if (timerTickCount % TIMER_FREQUENCY*CENTER_SERVO_UPDATE_TIME == 0 && timerTickCount != 0){
		centerServoUpdated = false;
	}

	timerTickCount++;
}

/*! \brief Calculates the wheel speed. The wheel speed needs to be calculated often to maintain accuracy and to keep track of distance.
 */
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

/*! \brief Updates the center servo. The center servo will scan clockwise and counterclockwise until Chico reaches the end state.
 */
void updateWheelSpeed(){
	if (!wheelSpeedUpdated){
		getSpeed();
		wheelSpeedUpdated = true;
	}
}

/*! \brief Updates the center servo. The center servo will scan clockwise and counterclockwise until Chico reaches the end state.
 */
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

/*! \brief Updates the LCD. The LCD shows the temperatures mesured by the sensor, the current speed and the total distance travelled.
 */
void updateLCD() {
	if (!lcdUpdated) {
		readTemperatureValues();

		char display_top[LCD_LINE_SIZE] = "";
		char display_bottom[LCD_LINE_SIZE] = "";

		sprintf(display_bottom, "S:%2.2f D:%2.2f", getAverageSpeed(), getTotalDistance());

		sprintf(display_top, "A:%d R:%d L:%d", getAmbient(), getAverageLeft(),
				getAverageRight());
		LCDPrint(display_top, display_bottom);

		lcdUpdated = true;
	}
}

/*! \brief Updates the LED. The LED will change color depending on Chico's state.
 */
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
