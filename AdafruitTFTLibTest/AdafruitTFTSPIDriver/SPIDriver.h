//*******************************************************************
//* SPIDriver.h
//* Author:         Bryant Gonzaga
//* Date:           11/30/2017
//* Target Device:  ATmega324A
//* Description:
//*     This is a header file that defines functions for hardware SPI
//* initialization and transmissions. As well as for slave mode.
//*******************************************************************
#ifndef SPIDRIVER_H_
#define SPIDRIVER_H_

#include <avr/io.h>

#include "PortRegisterStruct.h"

//SPI SETUP
void SPIMasterInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso);

void SPISlaveInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso);

void SPISetClkPrescalar(uint8_t prescalar);

void SPIStartTransmission(PortRegisters* ss);

void SPIMasterTransmit(uint8_t cData);

void SPIMasterTransmit16(uint16_t cData);

void SPIMasterTransmit32(uint32_t cData);

void SPIEndTransmission(PortRegisters* ss);

uint8_t SPISlaveReceive();

#endif /* SPIDRIVER_H_ */
