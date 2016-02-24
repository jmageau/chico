/*! \file main.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Main file for Chico the robot. This file will contain all the tasks created for Chico.
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

/* Serial interface include file. */
#include "usartserial.h"

/* Our drivers */

#include "led.h"
#include "tempsensor.h"
#include "motion.h"
#include "lcd.h"

/* Prototypes */

/* 	\brief
 * 		Show the ambient temperature on the Wifi card LED
 */
static void TaskShowTemperatureLED(void *pvParameters);

/*! \brief Usart for terminal and wifi card.
 */
int usartfd;

/*! \brief Main method.
 *
 *  This method will turn on all serial ports, call initialize methods and schedule tasks.
 */
int main(void) {

	// turn on the serial port for debugging or for other USART reasons.
	usartfd = usartOpen(USART0_ID, 115200, portSERIAL_BUFFER_TX,
	portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)

	usart_print_P(PSTR("\r\n\n\nHello World!\r\n")); // Ok, so we're alive...

	//initLED();
	initTPA81();
	LCDInit();
	motion_init();

	xTaskCreate(TaskShowTemperatureLED, (const portCHAR *)"RedLED" // Main Arduino Mega 2560, Freetronics EtherMega (Red) LED Blink
			,256// Tested 9 free @ 208
			,NULL, 3, NULL); // */

	vTaskStartScheduler();

	usart_print_P(PSTR("\r\n\n\nGoodbye... no space for idle task!\r\n")); // Doh, so we're dead...
}

/*! \brief Task used to show the ambient temperature on the Wifi card LED.
 *
 *  The average temperature is gathered and then displayed on the LED. The color of the LED depends on the temperature.
 *
 *  \param *pvParameters
 */
static void TaskShowTemperatureLED(void *pvParameters) {
	(void) pvParameters;
	;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int taskTime = (250 / portTICK_PERIOD_MS);

	int timer = 0;
	while (1) {
		readTemperatureValues();
		vTaskDelayUntil(&xLastWakeTime, taskTime);

		//TEMP
		char display_top[16] = "";
		char display_bottom[16] = "";

		sprintf(display_bottom, "A:%d L:%d R:%d", getAmbient(), getAverageLeft(),
				getAverageRight());

		sprintf(display_top, "A:%d R:%d L:%d", getAmbient(), getAverageLeft(),
						getAverageRight());
		LCDPrint(display_top, display_bottom);

		if (timer == 0){
			motion_servo_set_pulse_width(MOTION_WHEEL_LEFT, MIN_PULSE_WIDTH_TICKS);
		    motion_servo_start(MOTION_WHEEL_LEFT);
		}
		else if (timer == 4){
			motion_servo_stop(MOTION_WHEEL_LEFT);
		}

		timer = timer + 1;
		vTaskDelayUntil(&xLastWakeTime, taskTime);
	}
}

/*! \brief Task used to show the ambient temperature on the Wifi card LED.
 *
 *  \param xTask
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask,
portCHAR *pcTaskName) {
	while (1)
		;
}
