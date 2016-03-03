/*
 * servo.h
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

#ifndef SRC_SERVO_H_
#define SRC_SERVO_H_

#define FORWARDS       ((uint8_t) 0)
#define BACKWARDS       ((uint8_t) 1)
#define COUNTERCLOCKWISE   ((uint8_t) 2)
#define CLOCKWISE   ((uint8_t) 3)
#define STOP ((uint8_t) 4)
#define MIDDLE ((uint8_t) 5)

void servoInit();
void moveWheels(int direction);
void moveCenterServo(int direction);
double getSpeed(int deviceID);
double getAverageSpeed();
double getTotalDistance();

#endif /* SRC_SERVO_H_ */
