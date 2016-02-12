/*! \file tempsensor.h
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Header file for the temperature sensor driver.
 */

/* Includes */
#include "i2cMultiMaster.h"
#include "tempsensor.h"

/* Defines */

/*! \brief Slave write address for I2C communications.
 */
#define TPA81_WRITE 	0xD0
/*! \brief Slave read address for I2C communications.
 */
#define TPA81_READ	 	0xD1
/*! \brief Master address for I2C communications.
 */
#define I2C_MASTER_ADDRESS 0xC0

/*! \brief Number of pixels on the temperature sensor
 */
#define SENSOR_NUM_PIXELS	8

/*! \brief Stores all temperature readings from the sensors.
 */
uint8_t temp_pixel_values[SENSOR_NUM_PIXELS][2];

/*! \brief Average temperature of the 8 sensors
 */
int averageTemp;
int averageLeftTemp;
int averageRightTemp;

/*! \brief Initialization method of the temperature sensor. This set the master address for I2C communications.
 */
void initTPA81() {
	I2C_Master_Initialise(I2C_MASTER_ADDRESS);
}

/*! \brief Will detect and store all the temperatures from the 8 sensors.
 */
void readTemperatureValues() {
	int temperature_sum = 0;
    int temperature_left_sum = 0;
    int temperature_right_sum = 0;

	for (int i = 0; i < SENSOR_NUM_PIXELS; i++) {
		uint8_t writeData[2] = { TPA81_WRITE, i + 2 };
		uint8_t readData[2] = { TPA81_READ, 0x01 };
		I2C_Master_Start_Transceiver_With_Data(writeData, 2);
		I2C_Master_Start_Transceiver_With_Data(readData, 2);
		I2C_Master_Get_Data_From_Transceiver(temp_pixel_values[i], 2);
		temperature_sum += temp_pixel_values[i][1];	//The second of the pair contains the temp
        if (i > 1 && i <= 1 + SENSOR_NUM_PIXELS/2){ //the first value is ambient
            //left value
            temperature_left_sum += temp_pixel_values[i][1];
        } else{
            //right value
            temperature_right_sum += temp_pixel_values[i][1];
        }
	}
	averageTemp = (int) temperature_sum / SENSOR_NUM_PIXELS; //calculate the average
    averageLeftTemp = (int) temperature_left_sum / (SENSOR_NUM_PIXELS/2); //calculate left average
    averageRightTemp = (int) temperature_right_sum / (SENSOR_NUM_PIXELS/2); //calculate right average
}

/*! \brief Returns the average temperature of the 8 sensors.
 */
int getAmbient() {
	return averageTemp;
}

int getAverageLeft(){
    return averageLeftTemp;
}

int getAverageRight(){
    return averageRightTemp;
}
