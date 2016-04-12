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
#include "attached_mode.h"

int currentState = ATTACHED;

void initModules();
void TaskCenterServo(void *pvParameters);
void TaskThermoSensor(void *pvParameters);
void TaskWheelSpeed(void *pvParameters);
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

	usart_print_P(PSTR("\r\n\n\nGoodbye... no space for idle task!\r\n")); // Doh, so we're dead...
}

void createTasks() {
	xTaskCreate(TaskCenterServo, (const portCHAR *)"", 120, NULL, 3, NULL);
	xTaskCreate(TaskThermoSensor, (const portCHAR *)"", 120, NULL, 3, NULL);
	xTaskCreate(TaskWheelSpeed, (const portCHAR *)"", 128, NULL, 3, NULL);
	xTaskCreate(TaskLCD, (const portCHAR *)"", 128, NULL, 3, NULL);
	xTaskCreate(TaskLED, (const portCHAR *)"", 128, NULL, 3, NULL);
	xTaskCreate(TaskWIFI, (const portCHAR *)"", 636, NULL, 1, NULL);
	xTaskCreate(TaskWheels, (const portCHAR *)"", 128, NULL, 3, NULL);
	//wheels
	//web/wifi
	//sonar
}

void initModules() {
	initWifi();
	initLED();
	initTPA81();
	LCDInit();
	servoInit();

	initAttachedMode();
}

void initWifi() {
	taskENABLE_INTERRUPTS()
	;
	int terminalUSART = usartOpen(USART_0, BAUD_RATE_115200,
	portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
	int wifiUSART = usartOpen(USART_2, BAUD_RATE_9600, portSERIAL_BUFFER_TX,
	portSERIAL_BUFFER_RX);

	gs_initialize_module(wifiUSART, BAUD_RATE_9600, terminalUSART,
			BAUD_RATE_115200);
	gs_set_wireless_ssid("RICO");
	gs_activate_wireless_connection();

	//set up web page
	configure_web_page("Chico", "Control", HTML_RADIO_BUTTON);
	add_element_choice('0', "FORWARDS");
	add_element_choice('1', "BACKWARDS");
	add_element_choice('2', "CLOCKWISE");
	add_element_choice('3', "COUNTERCLOCKWISE");
	add_element_choice('4', "ATTACHED");
	start_web_server();
}

void TaskCenterServo(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 20;

	moveCenterServo(MIDDLE, 0);

	while (1) {
		if (currentState == ATTACHED){
			updateCenterServoAttachedMode();
		} else {
			moveCenterServo(SCAN, SCAN_SPEED);
		}
		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

void TaskThermoSensor(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 20;

	while (1) {
		readTemperatureValues();
		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

void TaskWheelSpeed(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 10;

	while (1) {
		getSpeed();
		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

void TaskLCD(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 100;

	while (1) {
		//TODO
		char display_top[LCD_LINE_SIZE] = "";
		char display_bottom[LCD_LINE_SIZE] = "";

		uint8_t *tempValues = getTemperatureValues();

		sprintf(display_bottom, "S:%2.2f D:%2.2f", getAverageSpeed(),
				getTotalDistance());

		//sprintf(display_top, "P:%d,%d,%d,%d,%d",tempValues[0],tempValues[1],tempValues[2],tempValues[3],tempValues[4]);

		sprintf(display_top, "A:%d R:%d L:%d", getAmbient(), getAverageLeft(),
				getAverageRight());

		//sprintf(display_bottom, "%d,%d,%dA:%dD:%d",tempValues[5],tempValues[6],tempValues[7],getAmbient(),99);

		LCDPrint(display_top, display_bottom);

		vTaskDelay(period / portTICK_PERIOD_MS);
	}

}

void TaskLED(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 50;

	while (1) {
		if (currentState == ATTACHED){
			setColor(false, false, false);
		} else if (currentState == MOVING_FORWARDS){
			setColor(false, true, false);
		} else if (currentState == MOVING_BACKWARDS){
			setColor(true, false, false);
		} else if (currentState == MOVING_CLOCKWISE){
			setColor(false, false, true);
		} else if (currentState == MOVING_COUNTERCLOCKWISE){
			setColor(false, false, true);
		} else if (currentState == STOPPED){
			setColor(true, true, true);
		} else {
			setColor(false, false, false);
		}

		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

void TaskWIFI(void *pvParameters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 500;

	vTaskDelay(6000 / portTICK_PERIOD_MS);
	while (1) {
		process_client_request();
		char clientRequest = get_next_client_response();

		if (clientRequest == '0'){
			currentState = MOVING_FORWARDS;
		} else if (clientRequest == '1') {
			currentState = MOVING_BACKWARDS;
		} else if (clientRequest == '2') {
			currentState = MOVING_CLOCKWISE;
		} else if (clientRequest == '3') {
			currentState = MOVING_COUNTERCLOCKWISE;
		} else if (clientRequest == '4') {
			currentState = ATTACHED;
		}

		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}

void TaskWheels(void *pvParamaters) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	int period = 20;

	while (1) {
		if (currentState == ATTACHED) {
			//TODO:
		} else if (currentState == MOVING_FORWARDS) {
			moveWheels(FORWARDS, 1);
		} else if (currentState == MOVING_BACKWARDS) {
			moveWheels(BACKWARDS, 1);
		} else if (currentState == MOVING_CLOCKWISE) {
			moveWheels(CLOCKWISE, 1);
		} else if (currentState == MOVING_COUNTERCLOCKWISE) {
			moveWheels(COUNTERCLOCKWISE, 1);
		} else {
			moveWheels(STOP, 0);
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
