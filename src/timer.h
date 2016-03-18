/*! \file timer.h
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Timer used to time various method calls for Chico.
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

#include <avr/io.h>

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

/*! \brief The value preloaded into TCNT. Calculated based on a 10ms tick rate.
 */
#define TIMER_PRELOAD    64911 //2^16 - 16MHz/256 prescaler/100Hz
#define TIMER_FREQUENCY  100 //1/x seconds

/*! \brief Initializes the timer.
 */
void initTimer(void (*isr)());
int getTime();
bool isTimerRepeatMultiple(int frequency);

#endif /* SRC_TIMER_H_ */
