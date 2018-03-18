//*******************************************************************
//* File Name       :  SPIDriver.c
//*
//* Author          :   Bryant Gonzaga
//* Created         :   11/30/2017
//* Modified        :   1/13/2018
//* Target Device   :   ATmega324A
//* Description:
//*     This is a c file that defines functions for hardware SPI
//* initialization and transmissions. As well as for slave mode.
//*******************************************************************

#include "SPIDriver.h"

void spiMasterInit(AVRPin* ss, AVRPin* sclk, AVRPin* mosi, AVRPin* miso)
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

void spiSlaveInit(AVRPin* ss, AVRPin* sclk, AVRPin* mosi, AVRPin* miso)
{
    //setup DDRx register for SPI
    *(ss->DDRx) &= ~ss->mask;
    *(sclk->DDRx) &= ~sclk->mask;
    *(mosi->DDRx) &= ~mosi->mask;
    *(miso->DDRx) |= miso->mask;
    /* Enable SPI */
    SPCR0 = _BV(SPE0);
}

void spiSetClkPrescalar(uint8_t prescalar)
{
}

void spiStartTransmission(AVRPin* ss)
{
    *(ss->PORTx) &= ~(ss->mask);
}

void spiMasterTransmit(uint8_t data)
{
    //start transmission
    SPDR0 = data;
    //Wait for transmission complete
    while (!(SPSR0 & _BV(SPIF0))) {
        //do nothing
    }
}

void spiMasterTransmit16(uint16_t data)
{
    spiMasterTransmit(data >> 8);
    spiMasterTransmit(data);
}

void spiMasterTransmit32(uint32_t data)
{
    spiMasterTransmit16(data >> 16);
    spiMasterTransmit16(data);
}

void spiEndTransmission(AVRPin* ss)
{
    *(ss->PORTx) |= ss->mask;
}

uint8_t spiSlaveReceive(void)
{
    while (!(SPSR0 & _BV(SPIF0))) {
        //wait for reception complete
    }
    //return data register
    uint8_t temp = SPDR0;
    return temp;
}
