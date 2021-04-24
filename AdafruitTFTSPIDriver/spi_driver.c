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

void spi_master_init(pin_intrf* ss, uint16_t flags)
{
    // TODO: check mutual exclusivity of flags
    
    
    // Check for Sampling
    if (flags & BG_SPI_SAMPLE_RISING) {
        
    }
    
    // setup DDRx register for SPI
    BG_SPI_SCLK_DDR |= BG_SPI_SCLK_PIN_BM;
    BG_SPI_MOSI_DDR |= BG_SPI_MOSI_PIN_BM;
    BG_SPI_MISO_DDR &= ~BG_SPI_MISO_PIN_BM;
    *(ss->DDRx) |= ss->mask;
    
    // set slave select
    *(ss->PORTx) |= ss->mask;
    
    // TODO: this is a hack and ugly. fix
    #if defined(AVR128DB48)
        *( ((uint8_t*)&BG_SPI_CR) + 1 ) = (BG_SPI_MSTR_BM >> 8);
    #endif
    
    // write to spi control register
	BG_SPI_CR |= BG_SPI_ENABLE_BM | BG_SPI_MSTR_BM | flags;

    // Send a byte to no one to set int flag
    BG_SPI_DR = 0xAA;
    // wait for the dummy data to send
    while ( !(BG_SPI_SR & BG_SPI_INTF_BM) ) {
        //do nothing
    }
}

void spi_slave_init(void)
{
    /* setup DDRx register for SPI */
    BG_SPI_SCLK_DDR &= ~BG_SPI_SCLK_PIN_BM;
    BG_SPI_MOSI_DDR &= ~BG_SPI_MOSI_PIN_BM;
    BG_SPI_MISO_DDR |= BG_SPI_MISO_PIN_BM;
    
    /* Enable SPI */
    BG_SPI_CR = BG_SPI_ENABLE_BM;
}

void spi_select_slave(pin_intrf* ss)
{
    *(ss->PORTx) &= ~(ss->mask);
}

void spi_deselect_slave(pin_intrf* ss)
{
    *(ss->PORTx) |= ss->mask;
}

void spi_master_transmit(uint8_t* data, uint8_t n)
{
    for (n; n > 0; n--) {
        BG_SPI_DR = *(data++);
        //Wait for transmission complete
        while ( !(BG_SPI_SR & BG_SPI_INTF_BM) ) {
            //do nothing
        }
    }
}

void spi_master_transmit8(uint8_t data)
{
    //start transmission
    BG_SPI_DR = data;
    //Wait for transmission complete
    while ( !(BG_SPI_SR & BG_SPI_INTF_BM) ) {
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

uint8_t spi_master_receive(void)
{
    //return data register
    uint8_t temp = BG_SPI_DR;
    return temp;
}

uint8_t spi_slave_receive(void)
{
    while ( !(BG_SPI_SR & BG_SPI_INTF_BM) ) {
        //wait for data
    }
    //return data register
    uint8_t temp = BG_SPI_DR;
    return temp;
}
