//*******************************************************************
//* File Name       :  AdafruitTFTI2CDriver.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   12/4/2017 5:42:04 PM
//* Modified        :   12/15/2017
//* Target Device   :   ATmega324A
//* Description:
//*     Contains functions to interact with Adafruit's capacitive TFT
//* touchscreen through I2C. Also contains TSVars structure for
//* variables needed about touch.
//*******************************************************************

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

/**
 * Initialize capacitive touchscreen with specified threshold
 * @param thresh    Threshold for capacitive touchscreen
 * @return  True if touchscreen is initialized properly.
 */
bool initCPTTS(uint8_t thresh);

/**
 * Using I2C, this function writes to the specified register, a byte
 * of data, on the touchscreen.
 * @param reg   Register Address on the touchscreen.
 * @param val   Value to be written to that register.
 * @return  True if data is properly transferred to register.
 */
bool writeRegister8(uint8_t reg, uint8_t val);

/**
 * Reads a byte from the specified register on the touchscreen.
 * @param reg   Address of register to be read
 * @return  A byte of data.
 */
uint8_t readRegister8(uint8_t reg);

/**
 * Reads all data from the touchscreen.
 */
void readData(uint16_t* x, uint16_t* y, TSVars* var);

/**
 * @return  True if the screen was touched. False otherwise.
 */
bool touched();

/**
 *  @return The last touched point in form of TS_Point structure.
 */
TS_Point getPoint(TSVars* var);

#endif /* ADAFRUITTFTI2CDRIVER_H_ */
