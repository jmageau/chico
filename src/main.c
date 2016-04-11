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

/*! \brief Usart for terminal and wifi card.
 */
int usartfd;

/*! \brief Main method.
 *
 *  This method will turn on all serial ports, call initialize methods and schedule tasks.
 */
int main(void) {

	taskENABLE_INTERRUPTS();
	int terminalUSART = usartOpen(USART_0, BAUD_RATE_115200,
			portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
	int wifiUSART = usartOpen(USART_2, BAUD_RATE_9600, portSERIAL_BUFFER_TX,
			portSERIAL_BUFFER_RX);
	gs_initialize_module(wifiUSART, BAUD_RATE_9600, terminalUSART,
			BAUD_RATE_115200);
	gs_set_wireless_ssid("RICO");
	gs_activate_wireless_connection();
	//set up web page
	configure_web_page("Chico", "Control",
			HTML_RADIO_BUTTON);
	add_element_choice('0', "FORWARDS");
	add_element_choice('1', "BACKWARDS");
	add_element_choice('2', "CLOCKWISE");
	add_element_choice('3', "COUNTERCLOCKWISE");
	start_web_server();

	usart_print_P(PSTR("\r\n\n\nHello World!\r\n")); // Ok, so we're alive...

	initBehaviour();

	TaskMoveAndScan();

	usart_print_P(PSTR("\r\n\n\nGoodbye... no space for idle task!\r\n")); // Doh, so we're dead...
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
