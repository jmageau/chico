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

#ifndef SRC_ATTACHED_MODE_H_
#define SRC_ATTACHED_MODE_H_

void updateAttachedMode();

#endif /* SRC_ATTACHED_MODE_H_ */
