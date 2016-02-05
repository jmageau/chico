#include "i2cMultiMaster.h"
#include "tempsensor.h"

#define TPA81_WRITE 	0xD0
#define TPA81_READ	 	0xD1

#define SENSOR_NUM_PIXELS	8
int temp_pixel_values[SENSOR_NUM_PIXELS][2];

int averageTemp;

void initTPA81() {
	I2C_Master_Initialise(0xC0);
}

void readTemperatureValues() {
	averageTemp = 0; //reset value
	for (int i = 0; i < SENSOR_NUM_PIXELS; i++) {
		I2C_Master_Start_Transceiver_With_Data({TPA81_WRITE, i}, 2);
		I2C_Master_Start_Transceiver_With_Data({TPA81_READ, 0x01}, 2);
		I2C_Master_Get_Data_From_Tranceiver(temp_pixel_values[i], 2);
		averageTemp += temp_pixel_values[i][1];	//The second of the pair contains the temp
	}
	averageTemp = (int) averageTemp / SENSOR_NUM_PIXELS; //calculate the average
}

int getAverageTemp() {
	return averageTemp;
}

