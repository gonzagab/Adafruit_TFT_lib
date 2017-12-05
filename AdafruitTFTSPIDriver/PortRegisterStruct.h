//*******************************************************************
//* PortRegisterStruct.h
//*
//* Created:    11/30/2017 5:49:54 PM
//* Author:     Bryant Gonzaga
//* Description:
//*******************************************************************

#ifndef PORTREGISTERSTRUCT_H_
#define PORTREGISTERSTRUCT_H_

#include <stdint.h>

typedef struct PortRegisters
{
    uint8_t* DDRx;
    uint8_t* PORTx;
    uint8_t* PINx;
    uint8_t mask;
} PortRegisters;

#endif /* PORTREGISTERSTRUCT_H_ */
