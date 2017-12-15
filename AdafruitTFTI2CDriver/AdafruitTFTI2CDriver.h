//*****************************************************************
//* Author  :   Bryant Gonzaga
//* Created :   12/4/2017 5:42:04 PM
//* Name    :   AdafruitTFTI2CDriver.h
//* Modified:   12/15/2017
//* Description:
//*
//********************************************************************


#ifndef ADAFRUITTFTI2CDRIVER_H_
#define ADAFRUITTFTI2CDRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#include "AdafruitTFTRegAddrs.h"
#include "AdafruitTSPoint.h"

typedef struct TSVars 
{
    uint8_t touches;
    uint16_t touchX[2], touchY[2], touchID[2];
} TSVars;

bool begin(uint8_t thresh = FT6206_DEFAULT_THRESSHOLD);

void writeRegister8(uint8_t reg, uint8_t val);

uint8_t readRegister8(uint8_t reg);

void readData(uint16_t* x, uint16_t* y, TSVars* var);

bool touched();

TS_Point getPoint(void);

#endif /* ADAFRUITTFTI2CDRIVER_H_ */
