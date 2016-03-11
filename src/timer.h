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

#include <avr/io.h>

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#define TIMER_PRELOAD 34286

void initTimer();
int getTime();
void incrementTimer();

#endif /* SRC_TIMER_H_ */
