/*! \file main.c
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Main file for Chico the robot. This file will initialize and schedule the tasks for Chico.
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

#include "behaviour.h"

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
