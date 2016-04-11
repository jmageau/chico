#include "FreeRTOS.h"
#include "BitDefinitions.h"

#include "custom_timer.h"

void initPing(){
	DDRA &= ~BIT0;
}

//returns time in microseconds between ping and echo
int ping(){
	DDRA &= ~BIT0; //Set ping to low
	pulse_out(10);
	return pulse_in();
}

//speedSound determined based on ambient temp (v = 331m/s + 0.6m/s/C * T`)
double getDistance(double speedSound){
	double echoTime = ping() / 1000000; //echo time in seconds
	return speedSound * echoTime; //Both in seconds
}

//send a pulse (high value) for a given amount of time
void pulse_out(int duration){
	DDRA |= BIT0; //high
	sleep(duration);
	DDRA &= ~BIT0; //low
}

//reads how long the pin reads the given value
int pulse_in(){	​
	/* Wait for pulse begin */
	while(getPortAPin(0) != 1);
	​
	unsigned long tStart = time_in_microseconds();
	​
	/* Wait for pulse end */
	while(getPortAPin(0) == 1);
	​	​
	return time_in_microseconds() - tStart;
}