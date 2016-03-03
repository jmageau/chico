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

#ifndef TIMER
#define TIMER

int count;

void initTimer(){
	count = 0;
}

int getTime(){
	return count;
}

void incrementTimer(){
	count = count + 1;
}


#endif
