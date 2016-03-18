/*! \file behaviour.h
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

#ifndef SRC_BEHAVIOUR_H_
#define SRC_BEHAVIOUR_H_

#define MOVING_FORWARDS           0
#define MOVING_BACKWARDS          1
#define MOVING_CLOCKWISE          2
#define MOVING_COUNTERCLOCKWISE   3
#define STOPPED                   4
#define STATE_TIME				  1    //in seconds
#define WHEEL_UPDATE_TIME		  0.02 //in seconds
#define WHEEL_SPEED_READ_TIME     0.01 // in seconds
#define LCD_UPDATE_TIME			  0.12 //in seconds
#define CENTER_SERVO_UPDATE_TIME  1    //in seconds

/*! \brief Will initialize all the components and create the starting state of Chico.
 */
void initBehaviour();

/*! \brief Task responsible for moving Chico in a predefined pattern and scanning with the center servo.
 *
 *  Chico will move forwards, backwards, clockwise, then counterclockwise. While moving, the center servo will scan left and right.
 *
 *  \param *pvParameters
 */
void TaskMoveAndScan(void *pvParameters);

#endif /* SRC_BEHAVIOUR_H_ */
