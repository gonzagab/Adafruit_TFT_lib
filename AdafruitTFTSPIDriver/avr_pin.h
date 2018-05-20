/********************************************************************
 * File:        avr_pin.h
 * Author:      Bryant Gonzaga
 * Created:     11/30/2017 5:49:54 PM
 * Modified:    3/29/2018
 *
 * Notes:
 *  This file is expected to be used to describe the Port structure
 * of the ATmega324 and ATmega128.
 *
 * Description:
 *  Defines a structure with pointers to the three registers
 * associated with a Port on an Atmel chip. The structure also 
 * contains a bit mask for a specific pin.
 *******************************************************************/

#ifndef BG_LIB_AVR_PIN_H_
#define BG_LIB_AVR_PIN_H_

#include <stdint.h>

typedef struct
{
    uint8_t* DDRx;  /**< Pointer to Data Direction Register */
    uint8_t* PORTx; /**< Pointer to PORT Register */
    uint8_t* PINx;  /**< Pointer to PIN Register */
    uint8_t mask;   /**< 8-bit mask. 1 in position of desired pin */
} avr_pin;

#endif /* BG_LIB_AVR_PIN_H_ */
