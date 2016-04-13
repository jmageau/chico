/*! \file tempsensor.c
 *  \authors
 *      C. Maathuis,
 *      N. Horton,
 *      J. Mageau,
 *      N. Seguin
 *  \brief
 *      Header file for the temperature sensor driver.
 */

/* Includes */
#include "i2cMultiMaster.h"
#include "tempsensor.h"

/*! \brief Stores all temperature readings from the sensors.
 */
uint8_t register_values[SENSOR_NUM_REGISTERS][2];

/*! \brief Initialization method of the temperature sensor. This set the master address for I2C communications.
 */
void initTPA81() {
	I2C_Master_Initialise(I2C_MASTER_ADDRESS);
}

/*! \brief Will detect and store all the temperatures from the 8 sensors.
 */
void readTemperatureValues() {
	for (int i = 0; i < SENSOR_NUM_REGISTERS; i++) {
		uint8_t writeData[2] = { TPA81_WRITE, i };
		uint8_t readData[2] = { TPA81_READ, 0x01 };
		I2C_Master_Start_Transceiver_With_Data(writeData, 2);
		I2C_Master_Start_Transceiver_With_Data(readData, 2);
		I2C_Master_Get_Data_From_Transceiver(register_values[i], 2);
	}
}

uint8_t *getTemperatureValues() {
	readTemperatureValues();
	static uint8_t tempValues[SENSOR_NUM_PIXELS];
	for (int i = 0; i < SENSOR_NUM_PIXELS; ++i) {
		tempValues[i] = register_values[i + 2][1];
	}
	return tempValues;
}

/*! \brief Returns the ambient temperature.
 */
int getAmbient() {
	return register_values[1][1];
}

/*! \brief Returns average temperature of the 3 left sensors.
 */
int getAverageLeft() {
	return (register_values[2][1] + register_values[3][1]
			+ register_values[4][1]) / 3;
}

/*! \brief Returns average temperature of the 2 middle sensors.
 */
int getAverageMiddle() {
	return (register_values[5][1] + register_values[6][1]) / 2;
}

/*! \brief Returns average temperature of the 3 right sensors.
 */
int getAverageRight() {
	return (register_values[7][1] + register_values[8][1]
			+ register_values[9][1]) / 3;
}
