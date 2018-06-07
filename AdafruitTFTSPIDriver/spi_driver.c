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

void spi_master_init(avr_pin* ss, uint8_t flags)
{
    //setup DDRx register for SPI
    BG_SPI_SCLK_DDR |= BG_SPI_SCLK_MASK;
    BG_SPI_MOSI_DDR |= BG_SPI_MOSI_MASK;
    BG_SPI_MISO_DDR |= BG_SPI_MISO_MASK;
    *(ss->DDRx) |= ss->mask;
    
    //set slave select
    *(ss->PORTx) |= ss->mask;
    
	//set up SPI with given flags
    uint8_t spcr_val = 0;
    if (flags & BG_SPI_LSB_FIRST) {
        spcr_val |= _BV(DORD);
    }
    if (flags & BG_SPI_SCLK_IDLE_HIGH) {
        spcr_val |= _BV(CPOL);
        if (flags & BG_SPI_SAMPLE_RISING) {
            spcr_val |= _BV(CPHA);
        }
    } else {
        if ( !(flags & BG_SPI_SAMPLE_RISING) ) {
            spcr_val |= _BV(CPHA);
        }
    }
    
	SPCR = _BV(SPE) | _BV(MSTR) | spcr_val | (flags >> 5);
    SPSR = (flags >> 4);
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
