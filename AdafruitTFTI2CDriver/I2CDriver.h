//*****************************************************************
//* Author  :   Bryant Gonzaga
//* Created :   12/4/2017 6:42:38 PM
//* Name    :   I2CDriver.h
//* Modified:   12/13/2017
//* Description:
//*
//********************************************************************


#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

bool i2cStartCondition();

bool i2cBeginWrite(uint8_t addr);

bool i2cBeginRead(uint8_t addr);

bool i2cTransmit(uint8_t data);

bool i2cReadData(uint8_t* dataAddr, uint8_t numOfBytes);

void i2cStopCondition();

#endif /* I2CDRIVER_H_ */
