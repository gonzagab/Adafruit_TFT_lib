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
void spiMasterInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso);

void spiSlaveInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso);

void spiSetClkPrescalar(uint8_t prescalar);

void spiStartTransmission(PortRegisters* ss);

void spiMasterTransmit(uint8_t cData);

void spiMasterTransmit16(uint16_t cData);

void spiMasterTransmit32(uint32_t cData);

void spiEndTransmission(PortRegisters* ss);

uint8_t spiSlaveReceive();

#endif /* SPIDRIVER_H_ */
