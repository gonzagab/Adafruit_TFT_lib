/*
 * I2CDriver.h
 *
 * Created: 12/4/2017 6:42:38 PM
 *  Author: gonza
 */ 


#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <avr/io.h>

void initI2C();

void beginTransmission(uint8_t addr);

#endif /* I2CDRIVER_H_ */