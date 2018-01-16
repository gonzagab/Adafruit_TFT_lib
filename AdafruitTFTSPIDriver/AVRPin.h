//*******************************************************************
//* File Name       :  PortRegisterStruct.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   11/30/2017 5:49:54 PM
//* Modified        :   12/20/2017
//* Target Device   :
//* Description:
//*     Defines a structure with pointers to the three registers
//* associated with a PORT. and it also has a bit mask for a specific
//* pin.
//*******************************************************************
#ifndef PORTREGISTERSTRUCT_H_
#define PORTREGISTERSTRUCT_H_

#include <stdint.h>

typedef struct AVRPin
{
    uint8_t* DDRx;  /**< Pointer to Data Direction Register >*/
    uint8_t* PORTx; /**< Pointer to PORT Register >*/
    uint8_t* PINx;  /**< Pointer to PIN Register >*/
    uint8_t mask;   /**< 8-bit mask. 1 in position of desired pin >*/
} AVRPin;

#endif /* PORTREGISTERSTRUCT_H_ */
