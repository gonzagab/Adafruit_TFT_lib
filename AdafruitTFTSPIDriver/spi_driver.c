/********************************************************************
 * File:        spi_driver.c
 * Author:      Bryant Gonzaga
 * Created:     11/30/2017
 * Modified:    3/29/2018
 *
 * Notes:
 *  This SPI driver was written to work with the ATmega128 and
 * ATmega324. The "system_config.h" file specifies what MCU is to be
 * used and compiler specific code. This file only needs to be edited
 * slightly to define what MCU is used.
 *
 * Description:
 *  This is the C file that defines functions for initializing and
 * using the hardware SPI on the MCU. It provides support for Master
 * and Slave mode of the MCU.
 *******************************************************************/

#include "spi_driver.h"

void spi_master_init(avr_pin* ss, avr_pin* sclk, avr_pin* mosi, avr_pin* miso)
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

void spi_slave_init(avr_pin* ss, avr_pin* sclk, avr_pin* mosi, avr_pin* miso)
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

void spi_start_transmission(avr_pin* ss)
{
    *(ss->PORTx) &= ~(ss->mask);
}

void spi_master_transmit(uint8_t data)
{
    //start transmission
    SPDR = data;
    //Wait for transmission complete
    while (!(SPSR & _BV(SPIF))) {
        //do nothing
    }
}

void spi_master_transmit16(uint16_t data)
{
    spi_master_transmit(data >> 8);
    spi_master_transmit(data);
}

void spi_master_transmit32(uint32_t data)
{
    spi_master_transmit16(data >> 16);
    spi_master_transmit16(data);
}

void spi_end_transmission(avr_pin* ss)
{
    *(ss->PORTx) |= ss->mask;
}

uint8_t spi_slave_recieve(void)
{
    while (!(SPSR & _BV(SPIF))) {
        //wait for reception complete
    }
    //return data register
    uint8_t temp = SPDR;
    return temp;
}
