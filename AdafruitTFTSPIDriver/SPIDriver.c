//*******************************************************************
//* File Name       :  SPIDriver.c
//*
//* Author          :   Bryant Gonzaga
//* Created         :   11/30/2017
//* Modified        :   12/20/2017
//* Target Device   :   ATmega324A
//* Description:
//*     This is a c file that defines functions for hardware SPI
//* initialization and transmissions. As well as for slave mode.
//*******************************************************************

#include "SPIDriver.h"

void spiMasterInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso)
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

void spiSlaveInit(PortRegisters* ss, PortRegisters* sclk, PortRegisters* mosi, PortRegisters* miso)
{
    //setup DDRx register for SPI
    *(ss->DDRx) &= ~ss->mask;
    *(sclk->DDRx) &= ~sclk->mask;
    *(mosi->DDRx) |= mosi->mask;
    *(miso->DDRx) &= ~miso->mask;
    /* Enable SPI */
    SPCR0 = _BV(SPE0);
}

void spiSetClkPrescalar(uint8_t prescalar)
{
}

void spiStartTransmission(PortRegisters* ss)
{
    *(ss->PORTx) &= ~(ss->mask);
}

void spiMasterTransmit(uint8_t cData)
{
    //start transmission
    SPDR0 = cData;
    //Wait for transmission complete
    while (!(SPSR0 & _BV(SPIF0))) {
        //do nothing
    }
}

void spiMasterTransmit16(uint16_t cData)
{
    spiMasterTransmit(cData >> 8);
    spiMasterTransmit(cData);
}

void spiMasterTransmit32(uint32_t cData)
{
    spiMasterTransmit16(cData >> 16);
    spiMasterTransmit16(cData);
}

void spiEndTransmission(PortRegisters* ss)
{
    *(ss->PORTx) |= ss->mask;
}

uint8_t spiSlaveReceive()
{
    while (!(SPSR0 & _BV(SPIF0))) {
        //wait for reception complete
    }
    //return data register
    return SPDR0;
}
