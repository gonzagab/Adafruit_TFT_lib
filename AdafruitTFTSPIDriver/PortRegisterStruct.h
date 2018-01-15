//*******************************************************************
//* File Name       :  PortRegisterStruct.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   11/30/2017 5:49:54 PM
//* Modified        :   12/20/2017
//* Target Device   :   ATmega324A
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
    uint8_t* DDRx;
    uint8_t* PORTx;
    uint8_t* PINx;
    uint8_t mask;
} AVRPin;

#endif /* PORTREGISTERSTRUCT_H_ */
