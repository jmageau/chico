#include "FreeRTOS.h"
#include "BitDefinitions.h"

#include "custom_timer.h"
#include "ping.h"
#include <stdbool.h>
#include <util/delay.h>

#include "led.h"

#define CHIRP_TIME_MICROSECONDS		10
#define LISTEN_TIME_MICROSECONDS	20000

double distance;

int ping();
void pulse_out(unsigned long duration);
double pulse_in(unsigned long maxDelay);

void initPing() {
	distance = 0;
	DDRA |= BIT0;  //Initialize as ouput
	PORTA &= ~BIT0; //Zero out value
}

//returns time in microseconds between ping and echo
int ping() {
	DDRA |= BIT0; //Ensure ouput mode
	PORTA &= ~BIT0; //Set ping to low
	pulse_out(CHIRP_TIME_MICROSECONDS); //chirp for 10 microseconds
	return pulse_in(LISTEN_TIME_MICROSECONDS); //time the echo delay
}

//speedSound determined based on ambient temp (v = 331cm/s + 0.6m/s/C * T`)
void readDistance(double speedSound) {
	//double echoTime = 100000 / 1000000; //convert echo time to seconds
	//distance = 331 *ping()/(double) 1000000; //Both in seconds
	distance = ping()/29/2; //Both in seconds

}

double getDistance() {
	return distance;
}

//send a pulse (high value) for a given amount of time
void pulse_out(unsigned long duration) {
	_delay_us(2);
	PORTA |= BIT0; //high
	_delay_us(5);
	PORTA &= ~BIT0; //low
}

//reads how long the pin reads the given value

double pulse_in(unsigned long maxDelay) {

	DDRA &= ~BIT0; //Set to input mode

	unsigned long tStart = time_in_microseconds(); //start timer to test for timeout

	/* Wait for pulse begin */
//	while (PORTA << 0 != 1){
//		if (time_in_microseconds() - tStart >= maxDelay){
//			setColor(true,false,false);
//			return 1000; //Error, no signal!
//		}
//	}
	unsigned long count = 0;

	while (!bit_is_set(PINA, PA0)) {
		_delay_us(5);
		count++;
		if (count > 200000) {
			return -1;
		}
	}

	tStart = time_in_microseconds(); //reset time, we only want to measure the high signal

	/* Wait for pulse end */
	loop_until_bit_is_clear(PINA, PA0);

	return time_in_microseconds() - tStart;
}
