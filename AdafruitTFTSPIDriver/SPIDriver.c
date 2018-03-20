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
	SPCR = _BV(SPE) | _BV(MSTR);
	//SPI clock x2
	SPSR = _BV(SPI2X);
}

void spiSlaveInit(AVRPin* ss, AVRPin* sclk, AVRPin* mosi, AVRPin* miso)
{
    //setup DDRx register for SPI
    *(ss->DDRx) &= ~ss->mask;
    *(sclk->DDRx) &= ~sclk->mask;
    *(mosi->DDRx) &= ~mosi->mask;
    *(miso->DDRx) |= miso->mask;
    /* Enable SPI */
    SPCR = _BV(SPE);
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
    SPDR = data;
    //Wait for transmission complete
    while (!(SPSR & _BV(SPIF))) {
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
    while (!(SPSR & _BV(SPIF))) {
        //wait for reception complete
    }
    //return data register
    uint8_t temp = SPDR;
    return temp;
}
