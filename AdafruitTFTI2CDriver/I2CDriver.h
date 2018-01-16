//*******************************************************************
//* File Name       :  I2CDriver.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   12/4/2017 6:42:38 PM
//* Modified        :   1/13/2018
//* Target Device   :   ATmega324A
//* Description:
//*     Contains several functions to implement the I2C communication
//* in hardware of the Atmega324A. To implement I2C follow these
//* steps:
//* 1) Start Condition.
//* 2) Begin write by sending address of slave.
//* 3) Send register address to write to.
//* 4) Send data.
//* 5) Stop Condition.
//*******************************************************************

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

/**
 * Sends a start condition.
 * @return true if slave acknowledges start condition.
 */
bool i2cStartCondition();

/**
 * Send address of slave and write bit.
 * @param addr  7-bit slave address that is right aligned
 * @return true if slave acknowledges call.
 */
bool i2cBeginWrite(uint8_t addr);

/**
 * Send address of slave and read bit
 * @param addr  7-bit slave address that is right aligned
 * @return true if slave acknowledges call.
 */
bool i2cBeginRead(uint8_t addr);

/**
 * Transmits a byte of data. Start Condition, Slave Address, and
 * Register Address should have all been sent before sending data.
 * This method is also used to send register address.
 * @param data  Byte of data or 8-bit register address
 * @return true if data was send and acknowledged by slave.
 */
bool i2cTransmit(uint8_t data);

/**
 * Reads a specified number of bytes from the slave.
 * @param dataAddr      Pointer to where the read data is to be
 *                      stored.
 * @param numOfBytes    Number of bytes to be read from slave.
 * @return true if data was read successfully.
 */
bool i2cReadData(uint8_t* dataAddr, uint8_t numOfBytes);

/**
 * Stop Condition is used to end transmission
 */
void i2cStopCondition();

#endif /* I2CDRIVER_H_ */
