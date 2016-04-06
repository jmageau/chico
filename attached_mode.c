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

#include "servo.h"
#include "tempsensor.h"

#define TEMP_THRESHOLD 35

#ifndef ATTACHED_MODE
#define ATTACHED_MODE

void updateAttachedMode() {
  if (readHeat()) {
    trackHeat();
  } else {
    moveCenterServo(SCAN, SCAN_SPEED);
  }
}

bool readHeat() {
  uint8_t *tempValues = getTemperatureValues();
  for (int i = 0; i < SENSOR_NUM_PIXELS; ++i) {
    if (tempValues[i] > TEMP_THRESHOLD) {
      return true;
    }
  }
  return false;
}

void trackHeat() {
  uint8_t *tempValues = getTemperatureValues();

}

#endif /* ATTACHED_MODE */
