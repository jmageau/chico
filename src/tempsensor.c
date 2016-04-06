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


/*! \brief Stores all temperature readings from the sensors.
 */
uint8_t pixel_values[SENSOR_NUM_PIXELS][2];

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
		I2C_Master_Get_Data_From_Transceiver(pixel_values[i], 2);
		temperature_sum += pixel_values[i][1];	//The second of the pair contains the temp
        if (i > 1 && i <= 1 + SENSOR_NUM_PIXELS/2){ //the first value is command register, the second value is ambient
            //left value
            temperature_left_sum += pixel_values[i][1];
        } else{
            //right value
            temperature_right_sum += pixel_values[i][1];
        }
	}
	averageTemp = (int) temperature_sum / SENSOR_NUM_PIXELS; //calculate the average
    averageLeftTemp = (int) temperature_left_sum / (SENSOR_NUM_PIXELS/2); //calculate left average
    averageRightTemp = (int) temperature_right_sum / (SENSOR_NUM_PIXELS/2); //calculate right average
}

uint8_t *getTemperatureValues() {
    readTemperatureValues();
    static uint8_t tempValues[SENSOR_NUM_PIXELS];
    for (int i = 0; i < SENSOR_NUM_PIXELS; ++i) {
        tempValues[i] = pixel_values[i][1];
    }
    return tempValues;
}

/*! \brief Returns the ambient temperature.
 */
int getAmbient() {
	return averageTemp;
}

/*! \brief Returns average temperature of the left sensors.
 */
int getAverageLeft(){
    return averageLeftTemp;
}

/*! \brief Returns average temperature of the right sensors.
 */
int getAverageRight(){
    return averageRightTemp;
}
