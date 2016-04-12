#include "FreeRTOS.h"
#include "BitDefinitions.h"

#include "custom_timer.h"
#include "ping.h"

int ping();
void pulse_out(int duration);
int pulse_in();

void initPing() {
	DDRA |= BIT0;  //Initialize as ouput
	PORTA &= ~BIT0; //Zero out value
}

//returns time in microseconds between ping and echo
int ping() {
	DDRA |= BIT0; //Ensure ouput mode
	PORTA &= ~BIT0; //Set ping to low
	pulse_out(10); //chirp for 10 microseconds
	return pulse_in(10000); //time the echo delay
}

//speedSound determined based on ambient temp (v = 331m/s + 0.6m/s/C * T`)
double getPingDistance(double speedSound) {
	double echoTime = ping() / 1000000; //echo time in seconds
	return speedSound * echoTime; //Both in seconds
}

//send a pulse (high value) for a given amount of time
void pulse_out(unsigned long duration) {
	PORTA |= BIT0; //high
	double tStart = time_in_microseconds();
	while (time_in_microseconds() - tStart <= duration);
	PORTA &= ~BIT0; //low
}

//reads how long the pin reads the given value

int pulse_in(unsigned long maxDelay){

	DDRA &= ~BIT0; //Set to input mode
	unsigned long tStart = time_in_microseconds();

	/* Wait for pulse begin */
	while (PORTA << 0 != 1){
		if (time_in_microseconds() - tStart >= maxDelay){
			return -1; //Error, no echo!
		}
	}

	/* Wait for pulse end */
	// while (PORTA << 0 == 1){
	// 	if (time_in_microseconds() - tStart >= maxDelay){
	// 		return -1; //Error, no echo!
	// 	}
	// }

	return time_in_microseconds() - tStart;
}
