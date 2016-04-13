/*! \file main.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Main file for Chico the robot. This file will initialize and schedule the tasks for Chico.
 */

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "wireless_interface.h"
#include "usart_serial.h"

#include "behaviour.h"

#include "led.h"
#include "tempsensor.h"
#include "lcd.h"
#include "servo.h"

#include "custom_timer.h"
#include "attached_mode.h"
#include "ping.h"

char clientRequest = ' ';

void initModules();
void TaskCenterServo(void *pvParameters);
void TaskLCD(void *pvParameters);
void TaskLED(void *pvParameters);
void TaskWIFI(void *pvParameters);
void TaskWheels(void *pvParameters);
void createTasks();
void initWifi();

/*! \brief Main method.
 *
 *  This method will turn on all serial ports, call initialize methods and schedule tasks.
 */
int main(void) {
	usart_print_P(PSTR("\r\n\n\nHello World!\r\n")); // Ok, so we're alive...

	initModules();

	createTasks();
	vTaskStartScheduler();

	setColor(true, false, true);
	usart_print_P(PSTR("\r\n\n\nGoodbye... no space for idle task!\r\n")); // Doh, so we're dead...
}

void createTasks() {
	xTaskCreate(TaskCenterServo, (const portCHAR *)"", 100, NULL, 3, NULL);
	xTaskCreate(TaskLCD, (const portCHAR *)"", 180, NULL, 3, NULL);
	xTaskCreate(TaskWIFI, (const portCHAR *)"", 800, NULL, 3, NULL);
	xTaskCreate(TaskWheels, (const portCHAR *)"", 128, NULL, 4, NULL);
}

void initModules() {
	initWifi();
	initLED();
	initTPA81();
	LCDInit();
	servoInit();

	initialize_module_timer0();
	initPing();

	initAttachedMode();
}

void initWifi() {
	portENABLE_INTERRUPTS()
	;
	int terminalUSART = usartOpen(USART_0, BAUD_RATE_115200,
	portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
	int wifiUSART = usartOpen(USART_2, BAUD_RATE_9600, portSERIAL_BUFFER_TX,
	portSERIAL_BUFFER_RX);

	gs_initialize_module(USART_2, BAUD_RATE_9600, USART_0, BAUD_RATE_115200);
	gs_set_wireless_ssid("RICO");
	gs_activate_wireless_connection();

	//set up web page
	configure_web_page("Chico", "Control", HTML_DROPDOWN_LIST);
	add_element_choice('0', "FORWARDS");
	add_element_choice('1', "BACKWARDS");
	add_element_choice('2', "CLOCKWISE");
	add_element_choice('3', "COUNTERCLOCKWISE");
	add_element_choice('4', "ATTACHED");
	add_element_choice('5', "STOP");
	start_web_server();
}

void TaskCenterServo(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 20;

	moveCenterServo(MIDDLE, 0);

	while (1) {
		readTemperatureValues();
		readDistance();
		if (currentState == ATTACHED || currentState == SEARCHING
				|| currentState == PANICKING) {
			updateCenterServoAttachedMode();
		} else {
			moveCenterServo(SCAN, SCAN_SPEED);
		}
		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

void TaskLCD(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 200;

	while (1) {
		char display_top[LCD_LINE_SIZE] = "";
		char display_bottom[LCD_LINE_SIZE] = "";

		uint8_t *tempValues = getTemperatureValues();

		if (currentState == ATTACHED || currentState == SEARCHING
				|| currentState == PANICKING || currentState == STOPPED) {
			sprintf(display_top, "%d,%d,%d,%d,%d", tempValues[0], tempValues[1],
					tempValues[2], tempValues[3], tempValues[4]);

			sprintf(display_bottom, "%d,%d,%d,%dD%3.0f", tempValues[5],
					tempValues[6], tempValues[7], getAmbient(), getDistance());
		} else {
			sprintf(display_top, "State:%d", currentState);

			sprintf(display_bottom, "D:%2.2f,S:%2.2f", getTotalDistance(),
					getAverageSpeed());
		}
		LCDPrint(display_top, display_bottom);

		vTaskDelay(period / portTICK_PERIOD_MS);
	}

}

void TaskWIFI(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 500;

	vTaskDelay(6000 / portTICK_PERIOD_MS);

	while (1) {
		clientRequest = process_client_request();

		if (clientRequest != ' ' && clientRequest != '\0') {
			if (clientRequest == '0') {
				setColor(false, true, false); // Green
				currentState = MOVING_FORWARDS;
			} else if (clientRequest == '1') {
				setColor(true, false, false); //red
				currentState = MOVING_BACKWARDS;
			} else if (clientRequest == '2') {
				setColor(false, false, true); // Blue
				currentState = MOVING_CLOCKWISE;
			} else if (clientRequest == '3') {
				setColor(false, false, true); // Blue
				currentState = MOVING_COUNTERCLOCKWISE;
			} else if (clientRequest == '4') {
				//currentState = ATTACHED;
				setColor(false, false, false);
				initAttachedMode();
			} else if (clientRequest == '5') {
				setColor(true, true, true);  //White
				currentState = STOPPED;
			}
		}

		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

void TaskWheels(void *pvParamaters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 100;

	while (1) {
		getSpeed();

		if (currentState == ATTACHED || currentState == PANICKING
				|| currentState == SEARCHING) {
			updateWheelsAttachedMode();
		} else if (currentState == MOVING_FORWARDS) {
			moveWheels(FORWARDS, 1);
		} else if (currentState == MOVING_BACKWARDS) {
			moveWheels(BACKWARDS, 1);
		} else if (currentState == MOVING_CLOCKWISE) {
			moveWheels(CLOCKWISE, 1);
		} else if (currentState == MOVING_COUNTERCLOCKWISE) {
			moveWheels(COUNTERCLOCKWISE, 1);
		} else if (currentState == STOPPED) {
			moveWheels(STOP, 1);
		}

		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

/*! \brief Application Stack Overflow hook
 *
 *  \param xTask
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask,
portCHAR *pcTaskName) {
	while (1)
		;
}
