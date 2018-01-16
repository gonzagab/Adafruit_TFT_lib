//*******************************************************************
//* File Name       :  SPIDriver.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   11/30/2017
//* Modified        :   12/20/2017
//* Target Device   :   ATmega324A
//* Description:
//*     This is a header file that defines functions for hardware SPI
//* initialization and transmissions. As well as for slave mode.
//*******************************************************************
#ifndef SPIDRIVER_H_
#define SPIDRIVER_H_

#include <avr/io.h>

#include "AVRPin.h"

/**
 * Initializes the SPI hardware to operate in the Master mode, to
 * send the most significant bit first, and to have slave clock
 * frequency of SOME Hz.
 * Initializes the ss, sclk, and mosi pins as outputs
 * and the miso pin as input. It also drives the mosi and sclk pins
 * low and the ss pin high.
 * @param ss    Slave Select. Pointer to AVRPin struct
 * @param sclk  Slave Clock. Pointer to AVRPin struct
 * @param mosi  Master Out Slave In. Pointer to AVRPin struct
 * @param miso  Master In Slave Out. Pointer to AVRPin struct
 * @see AVRPin
 */
void spiMasterInit(AVRPin* ss, AVRPin* sclk, AVRPin* mosi, AVRPin* miso);

/**
 * Initializes the SPI hardware to operate in the Slave mode.
 * Firstly, it initializes the ss, sclk, and mosi pins as inputs and
 * the miso pin as output.
 * @param ss    Slave Select. Pointer to AVRPin struct
 * @param sclk  Slave Clock. Pointer to AVRPin struct
 * @param mosi  Master Out Slave In. Pointer to AVRPin struct
 * @param miso  Master In Slave Out. Pointer to AVRPin struct
 * @see AVRPin
 */
void spiSlaveInit(AVRPin* ss, AVRPin* sclk, AVRPin* mosi, AVRPin* miso);

/**
 * Empty for now.
 */
void spiSetClkPrescalar(uint8_t prescalar);

/**
 * Initializes the SPI transaction by driving the slave select pin
 * low.
 * @param ss    AVRPin structure for the slave select pin.
 */
void spiStartTransmission(AVRPin* ss);

/**
 * Transmits a single byte of data through hardware SPI.
 * Will return once data has been transmitted
 * @param data  byte of data to be transmitted.
 */
void spiMasterTransmit(uint8_t data);

/**
 * Transmits a two bytes of data through hardware SPI.
 * Will return once data has been transmitted
 * @param data  16-bit data to be transmitted.
 */
void spiMasterTransmit16(uint16_t data);

/**
 * Transmits a four bytes of data through hardware SPI.
 * Will return once data has been transmitted
 * @param data  32-bit data to be transmitted.
 */
void spiMasterTransmit32(uint32_t data);

/**
 * Ends the SPI transaction by driving the slave select pin high.
 * @param ss    AVRPin structure for the slave select pin.
 */
void spiEndTransmission(AVRPin* ss);

/**
 * Used to read data from a master through SPI.
 * @return a byte of data received from master.
 */
uint8_t spiSlaveReceive();

#endif /* SPIDRIVER_H_ */
