//*******************************************************************
//* SPIDriver.c
//* Author:         Bryant Gonzaga
//* Date:           11/30/2017
//* Target Device:  ATmega324A
//* Description:
//*     This is a c file that defines functions for hardware SPI
//* initialization and transmissions. As well as for slave mode.
//*******************************************************************

#include "SPIDriver.h"

void SPIMasterInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso)
{
    //setup DDRx register for SPI
    *(ss->DDRx) |= ss->mask;
    *(sclk->DDRx) |= sclk->mask;
    *(mosi->DDRx) |= mosi->mask;
    *(miso->DDRx) &= ~miso->mask;
    //slave select high
    *(ss->PORTx) |= ss->mask;
    //mosi and sclk low
    *(mosi->PORTx) &= ~mosi->mask;
    *(sclk->PORTx) &= ~sclk->mask;
	//Enable SPI, Master, MSB first
	SPCR0 = _BV(SPE0) | _BV(MSTR0);
	//SPI clock x2
	SPSR0 = _BV(SPI2X0);
}

/**
TODO: check miso mosi direction correctness
**/
void SPISlaveInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso)
{
    //setup DDRx register for SPI
    *(ss->DDRx) &= ~ss->mask;
    *(sclk->DDRx) &= ~sclk->mask;
    *(mosi->DDRx) |= mosi->mask;
    *(miso->DDRx) &= ~miso->mask;
    /* Enable SPI */
    SPCR0 = _BV(SPE0);
}

void SPISetClkPrescalar(uint8_t prescalar)
{
}

void SPIStartTransmission(PortRegisters* ss)
{
    *(ss->PORTx) &= ~(ss->mask);
}

void SPIMasterTransmit(uint8_t cData)
{
    //start transmission
    SPDR0 = cData;
    //Wait for transmission complete
    while (!(SPSR0 & _BV(SPIF0))) {
        //do nothing
    }
}

void SPIMasterTransmit16(uint16_t cData)
{
    SPIMasterTransmit(cData >> 8);
    SPIMasterTransmit(cData);
}

void SPIMasterTransmit32(uint32_t cData)
{
    SPIMasterTransmit16(cData >> 16);
    SPIMasterTransmit16(cData);
}

void SPIEndTransmission(PortRegisters* ss)
{
    *(ss->PORTx) |= ss->mask;
}

uint8_t SPISlaveReceive()
{
    while (!(SPSR0 & _BV(SPIF0))) {
        //wait for reception complete
    }
    //return data register
    return SPDR0;
}
