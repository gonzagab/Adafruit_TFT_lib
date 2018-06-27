/********************************************************************
 * File:        spi_driver.c
 * Author:      Bryant Gonzaga
 * Created:     11/30/2017
 * Modified:    6/11/2018
 *
 * Notes:
 *  This SPI driver was written to work with the ATmega16, ATmega128,
 * and ATmega324. The "system_config.h" file specifies what MCU is to
 * be used and compiler specific code. This file only needs to be edited
 * slightly to define what MCU is used.
 *
 * Description:
 *  This is a header file that declares functions for initializing and
 * using the hardware SPI on the MCU. It provides support for Master
 * and Slave mode of the MCU.
 *******************************************************************/

#include "spi_driver.h"

void spi_master_init(avr_pin* ss, uint8_t flags)
{
    //setup DDRx register for SPI
    BG_SPI_SCLK_DDR |= BG_SPI_SCLK_MASK;
    BG_SPI_MOSI_DDR |= BG_SPI_MOSI_MASK;
    BG_SPI_MISO_DDR &= ~BG_SPI_MISO_MASK;
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

void spi_slave_init(avr_pin* ss)
{
    //setup DDRx register for SPI
    BG_SPI_SCLK_DDR &= ~BG_SPI_SCLK_MASK;
    BG_SPI_MOSI_DDR &= ~BG_SPI_MOSI_MASK;
    BG_SPI_MISO_DDR |= BG_SPI_MISO_MASK;
    *(ss->DDRx) &= ~ss->mask;
    
    /* Enable SPI */
    SPCR = _BV(SPE);
}

void spi_select_slave(avr_pin* ss)
{
    *(ss->PORTx) &= ~(ss->mask);
}

void spi_deselect_slave(avr_pin* ss)
{
    *(ss->PORTx) |= ss->mask;
}

void spi_master_transmit(uint8_t* data, uint8_t n)
{
    for (n; n > 0; n--) {
        SPDR = *(data++);
        //Wait for transmission complete
        while (!(SPSR & _BV(SPIF))) {
            //do nothing
        }
    }
}

void spi_master_transmit8(uint8_t data)
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
    spi_master_transmit8(data >> 8);
    spi_master_transmit8(data);
}

void spi_master_transmit32(uint32_t data)
{
    spi_master_transmit16(data >> 16);
    spi_master_transmit16(data);
}

uint8_t spi_master_recieve(void)
{
    //return data register
    uint8_t temp = SPDR;
    return temp;
}

uint8_t spi_slave_recieve(void)
{
    while ( !(SPSR & _BV(SPIF)) ) {
        //wait for data
    }
    //return data register
    uint8_t temp = SPDR;
    return temp;
}
