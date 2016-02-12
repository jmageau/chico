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

/*! \brief Initialization method of the temperature sensor. This set the master address for I2C communications.
 */
void initTPA81();

/*! \brief Will detect and store all the temperatures from the 8 sensors.
 */
void readTemperatureValues();

/*! \brief Returns the average temperature of the 8 sensors.
 */
int getAmbient();
int getAverageLeft();
int getAverageRight();

#endif
