#include "i2cMultiMaster.h"
#include "tempsensor.h"

#define TPA81_WRITE 	0xD0
#define TPA81_READ	 	0xD1
#define I2C_MASTER_ADDRESS 0xC0

#define SENSOR_NUM_PIXELS	9
uint8_t temp_pixel_values[SENSOR_NUM_PIXELS][2];

int averageTemp;

void initTPA81() {
	I2C_Master_Initialise(I2C_MASTER_ADDRESS);
}

void readTemperatureValues() {
	int temperature_sum = 0; //reset value

	for (int i = 0; i < SENSOR_NUM_PIXELS; i++) {
		uint8_t writeData[2] = {TPA81_WRITE, i+1};
		uint8_t readData[2] = {TPA81_READ, 0x01};
		I2C_Master_Start_Transceiver_With_Data(writeData, 2);
		I2C_Master_Start_Transceiver_With_Data(readData, 2);
		I2C_Master_Get_Data_From_Transceiver(temp_pixel_values[i], 2);
		temperature_sum += temp_pixel_values[i][1];	//The second of the pair contains the temp
	}
	averageTemp = (int) temperature_sum / SENSOR_NUM_PIXELS; //calculate the average
}

int getAverageTemp() {
	return averageTemp;
}
