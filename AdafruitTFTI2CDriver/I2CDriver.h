/*
 * I2CDriver.h
 *
 * Created: 12/4/2017 6:42:38 PM
 *  Author: Bryant Gonzaga
 */ 


#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

bool I2CStartCondition();

bool I2CBeginWrite(uint8_t addr);

bool I2CBeginRead(uint8_t addr);

bool I2CTransmitData(uint8_t data);

void I2CEndTransmission();

#endif /* I2CDRIVER_H_ */