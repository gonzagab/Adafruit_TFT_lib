/********************************************************************
 * File:        avr_pin.h
 * Author:      Bryant Gonzaga
 * Created:     11/30/2017 5:49:54 PM
 * Modified:    3/9/2020
 *
 * Notes:
 *  This file is expected to be used to describe the Port structure
 * that can be used for microcontoller pins. It tries to accomidate
 * for avr architecture along with arm architectures.
 *
 * Description:
 *  Defines a structure with pointers to the three registers
 * associated with a Port on an microcontroller. The structure also 
 * contains a bit mask for a specific pin.
 *******************************************************************/

#ifndef BG_LIB_PIN_INTRF_H_
#define BG_LIB_PIN_INTRF_H_

#include <stdint.h>

#if defined(ATMEGA324) || defined(AVR128DB48)
    typedef uint8_t sys_reg;
#elif defined(__SAML21J18B__ )
    typedef uint32_t sys_reg;
#endif

typedef struct
{
    sys_reg* DDRx;  /**< Pointer to Data Direction Register */
    sys_reg* PORTx; /**< Pointer to PORT Register */
    sys_reg* PINx;  /**< Pointer to PIN Register */
    sys_reg mask;   /**< n-bit mask. 1 in position of desired pin */
} pin_intrf;

#endif /* BG_LIB_PIN_INTRF_H_ */
