/*! \file servo.h
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

#ifndef SRC_SERVO_H_
#define SRC_SERVO_H_

#define FORWARDS           ((uint8_t) 0)
#define BACKWARDS          ((uint8_t) 1)
#define COUNTERCLOCKWISE   ((uint8_t) 2)
#define CLOCKWISE          ((uint8_t) 3)
#define STOP               ((uint8_t) 4)
#define MIDDLE             ((uint8_t) 5)

#define CAPTURE_EVENT_DISTANCE  0.0054
#define SERVO_PERIOD            0.0000005

/*! \brief Initializes the motion module and sets all speeds/distances to zero.
 */
void servoInit();
/*! \brief Sends move commands to the wheels depending on the direction.
 *
 * \param direction
 * 	The direction the wheels need to move in.
 */
void moveWheels(int direction);
/*! \brief Sends move commands to the center servo depending on the direction
 *
 * \param direction
 * 	The direction the center servo needs to move in.
 */
void moveCenterServo(int direction);

/*! \brief Calculates the speeds of the wheels
 */
void getSpeed();
/*! \brief Returns the average speed of the wheels
 */
double getAverageSpeed();
/*! \brief Returns the total distance traveled by the wheels
 */
double getTotalDistance();

#endif /* SRC_SERVO_H_ */
