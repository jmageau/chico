////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////    main.c
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

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

/* serial interface include file. */
#include "usartserial.h"

/* our drivers */

#include "led.h"
#include "tempsensor.h"

/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
//extern xComPortHandle xSerialPort;
static void TaskShowTemperatureLED(void *pvParameters); // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink

/*-----------------------------------------------------------*/

/* Main program loop */
//int main(void) __attribute__((OS_main));
int usartfd;

int main(void) {

	// turn on the serial port for debugging or for other USART reasons.
	usartfd = usartOpen(USART0_ID, 115200, portSERIAL_BUFFER_TX,
			portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)

	usart_print_P(PSTR("\r\n\n\nHello World!\r\n")); // Ok, so we're alive...

	initLED();
	initTPA81();

	xTaskCreate(TaskShowTemperatureLED, (const portCHAR *)"RedLED" // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
			,256// Tested 9 free @ 208
			,NULL, 3, NULL); // */

	vTaskStartScheduler();

	usart_print_P(PSTR("\r\n\n\nGoodbye... no space for idle task!\r\n")); // Doh, so we're dead...
}

/*-----------------------------------------------------------*/

static void TaskShowTemperatureLED(void *pvParameters) {
	(void) pvParameters;
	;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int taskTime = (250 / portTICK_PERIOD_MS);

	while (1) {
		readTemperatureValues();
		vTaskDelayUntil(&xLastWakeTime, taskTime);

		int temp = getAverageTemp();

		if (temp >= 40) { // Red
			setColor(true, false, false);
		} else if (temp < 40 && temp >= 30) { // Green
			setColor(false, true, false);
		}
		else { // Blue
			setColor(false, false, true);
		}

		vTaskDelayUntil(&xLastWakeTime, taskTime);
	}
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t xTask,
portCHAR *pcTaskName) {
	while (1);
}

/*-----------------------------------------------------------*/

