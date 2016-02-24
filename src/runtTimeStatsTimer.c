/*----------------------------------------
 * runtTimeStatsTimer.c
 *
 *  Created on: Feb 12, 2015
 *      Author: garbez
 *
 *  Description:  To support the run time stats,
 *  a timer must be configured to update a counter
 *  for periods shorter than the 1 ms tick period
 *  of the OS.
 *
 *  This module will uses timer 3 configured in
 *  normal mode with a 16 micro-second increment.
 *  Thus the counter after incremented will not overflow
 *  before 1.05 sec.   Run time stats must be recorded
 *  before then.
 *
 *  Configure Timer 3 nomral mode with control register
 *
 *  TCCR3A: 0x00,   No Output compare mode enabled (Chan A-B7:6, Chan B-5:4, Chan C-3:2),
 *                                               WGM1:0 Normal mode (Bits 1:0) set to 00
 *  TCCR3B: 0b00000100
 *                No Input Capture enabled B7:6 = 00
 *                WGM3:2 Normal mode = 00
 *                System clock for the AtMega 2560, given by  = 16MHz
 *                Clock prescaler:  One of four prescalers are available.  Selecting 256
 *                                  gives a time base for run time stats = 0.0625 microSec * 256 = 16 microSec // Enables the clock
 *                B2:0=100, for 256 prescaler
 * TCCR3C: 0x00  no force output compare for channels Chan A B7,  Chan B, B6,  Chan C B5
 * TCNT3:  TCNT counter register for Timer 3
 * OCR3A, OCR3B, OCR3C:  Output Compare Registers
 * ICR3, Input Capture register
 * TIMSK3:  Interrupt mask register:   Bit 0 is the overflow interrupt enable
 * TIFR3: Timer Interrupt Flag Register Bit 0 is the overflow flag
 *
 *---------------------------------------*/
#include <avr/io.h>  // To get hardware register defines

/*--------------------------------------------------
 * Function:  portCONFIGURE_TIMER_FOR_RUNT_TIME_STATS
 *
 * Description: defined as a function rather than a MACRO - does not require changing the FreeRTOSConfig.h file.
 *
 * This function configures timer 3 with 16 microsecond increments.  TCNT3 can be used as the counter value
 * to return (see portGET_RUN_TIME_COUNTER_VALUE() function).
 *-----------------------------------------------------------*/

void xconfigTimerForRunTimeStats()
{
	TCCR1A = 0;  // No OC channel enabled, WGM1:0==00, normal mode
	TCCR1B = 0;  // Sets prescaler to 000 - disables the timer
	TCNT1 = 0; // set timer count register to 0
	TCCR1B = 0b00000100; // Input capture disabled, WGM3:2=00(normal mode), B2:0=100, for 256 prescaler
}

/*--------------------------------------------------
 * Function:  portGET_RUN_TIME_COUNTER_VALUE
 *
 * Description: Defined as an inline function rather than a MACRO - does not require changing the FreeRTOSConfig.h file.
 *
 * This function configures timer 3 with 16 microsecond increments.  TCNT3 can be used as the counter value
 * to return (see portGET_RUN_TIME_COUNTER_VALUE() function).
 *-----------------------------------------------------------*/
inline uint16_t portGET_RUN_TIME_COUNTER_VALUE()  { return TCNT1; }
