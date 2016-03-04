/*
 * behaviour.h
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

#ifndef SRC_BEHAVIOUR_H_
#define SRC_BEHAVIOUR_H_

#define MOVING_FORWARDS           0
#define MOVING_BACKWARDS          1
#define MOVING_CLOCKWISE          2
#define MOVING_COUNTERCLOCKWISE   3
#define STOPPED                   4
#define TASK_TICK_TIME            0.5

void initBehaviour();

void TaskMoveAndScan(void *pvParameters);

#endif /* SRC_BEHAVIOUR_H_ */
