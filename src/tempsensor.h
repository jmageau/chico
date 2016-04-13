/*! \file tempsensor.h
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		Header file for the temperature sensor driver.
 */

#ifndef TEMP_SENSOR
#define TEMP_SENSOR

/*! \brief Slave write address for I2C communications.
 */
#define TPA81_WRITE 	0xD0
/*! \brief Slave read address for I2C communications.
 */
#define TPA81_READ	 	0xD1
/*! \brief Master address for I2C communications.
 */
#define I2C_MASTER_ADDRESS 0xC0

#define SENSOR_NUM_REGISTERS 10

/*! \brief Number of pixels on the temperature sensor
 */
#define SENSOR_NUM_PIXELS	8

/*! \brief Initialization method of the temperature sensor. This set the master address for I2C communications.
 */
void initTPA81();

/*! \brief Will detect and store all the temperatures from the 8 sensors.
 */
void readTemperatureValues();

uint8_t *getTemperatureValues();

/*! \brief Returns the average temperature of the 8 sensors.
 */
int getAmbient();
/*! \brief Returns average temperature of the 3 left sensors.
 */
int getAverageLeft();
/*! \brief Returns average temperature of the 2 middle sensors.
 */
int getAverageMiddle();
/*! \brief Returns average temperature of the 3 right sensors.
 */
int getAverageRight();


#endif
