//*******************************************************************
//* SPIDriver.c
//* Author:         Bryant Gonzaga
//* Date:           11/30/2017
//* Target Device:  ATmega324A
//* Description:
//*     This is a c file that defines functions for hardware SPI
//* initialization and transmissions. As well as for slave mode.
//*******************************************************************

#include <avr/io.h>

#include "SPIDriver.h"

void SPIMasterInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso)
{
    //setup DDRx register for SPI
    *(ss->DDRx) |= ss->mask;
    *(sclk->DDRx) |= sclk->mask;
    *(mosi->DDRx) |= mosi->mask;
    *(miso->DDRx) &= ~miso->mask;
    //Enable SPI, Master, set clock rate system clock/16
    SPCR0 = (1 << SPE0) | (1 << MSTR0) | (1 << SPR00);
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
    SPCR0 = (1 << SPE0);
}

void SPISetClkPrescalar(uint8_t prescalar)
{
}

void SPIMasterTransmit(uint8_t cData)
{
    //start transmission
    SPDR0 = cData;
    //Wait for transmission complete
    while (!(SPSR0 & (1<<SPIF0))) {
        //do nothing
    }
}

void SPIMasterTransmit16(uint16_t cData)
{
    SPIMasterTransmit(cData);
    SPIMasterTransmit(cData >> 8);
}

void SPIMasterTransmit32(uint32_t cData)
{
    SPIMasterTransmit16(cData);
    SPIMasterTransmit16(cData >> 16);
}

uint8_t SPISlaveReceive()
{
    //wait for reception complete
    while (!(SPSR0 & (1 << SPIF0))) {

    }
    //return data register
    return SPDR0;
}
