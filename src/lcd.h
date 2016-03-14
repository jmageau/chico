/*! \file lcd.h
 *  \authors
 *  	C. Maathuis,
 *  	N. Horton,
 *  	J. Mageau,
 *  	N. Seguin
 *	\brief
 *		LCD driver for Chico
 */

#ifndef LCD
#define LCD

#define LCD_LINE_SIZE 16


/*! \brief Initializes the LCD component by turning it on and clearing it.
 */
void LCDInit();

/*! \brief Prints two char arrays on the LCD, one for the top row and one for the bottom row.
 *     \param top
 *  	Char array being printed on the top row
 *     \param bottom
 *  	Char array being printed on the bottom row
 */
void LCDPrint(char*, char*);

#endif
