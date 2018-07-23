/********************************************************************
 * File:        spi_driver.h
 * Author:      Bryant Gonzaga
 * Created:     11/30/2017
 * Modified:    6/11/2018
 *
 * Notes:
 *  This SPI driver was written to work with the ATmega16, ATmega128,
 * and ATmega324. The "system_config.h" file specifies what MCU is to
 * be used and compiler specific code. This file only needs to be 
 * edited slightly to define what MCU is used. Only active low slaves
 * are supported.
 *
 * Description:
 *  This is a header file that declares functions for initializing and
 * using the hardware SPI on the MCU. It provides support for Master
 * and Slave mode of the MCU.
 *******************************************************************/

#ifndef BG_LIB_SPI_DRIVER_H_
#define BG_LIB_SPI_DRIVER_H_

#include "system_config.h"
#include "avr_pin.h"

//=================================================================//
//                SPI Port and Register Definitions                //
//=================================================================//
#define BG_SPI_SCLK_DDR DDRB
#define BG_SPI_MOSI_DDR DDRB
#define BG_SPI_MISO_DDR DDRB

#define BG_SPI_SCLK_PIN PINB
#define BG_SPI_MOSI_PIN PINB
#define BG_SPI_MISO_PIN PINB

#define BG_SPI_SCLK_PORT PORTB
#define BG_SPI_MOSI_PORT PORTB
#define BG_SPI_MISO_PORT PORTB

#if defined(ATMEGA324)
    /* Register Definitions */
    #define  BG_SPI_CR   SPCR0
    #define  BG_SPI_DR   SPDR0
    #define  BG_SPI_SR   SPSR0
    
    /* SPI Status Register Bit Definitions */
    #define SPIF    SPIF0
    #define WCOL    WCOL0
    #define SPI2X   SPI2X0

    /* SPI Control Register Bit Definitions */
    #define SPIE    SPIE0
    #define SPE     SPE0
    #define DORD    DORD0
    #define MSTR    MSTR0
    #define CPOL    CPOL0
    #define CPHA    CPHA0
    #define SPR1    SPR10
    #define SPR0    SPR00
    
    /* SPI Pin Masks */
    #define BG_SPI_MOSI_MASK 0x20
    #define BG_SPI_MISO_MASK 0x40
    #define BG_SPI_SCLK_MASK 0x80
    
#elif defined(ATMEGA128)
    /* Register Definitions */
    #define  BG_SPI_CR   SPCR
    #define  BG_SPI_DR   SPDR
    #define  BG_SPI_SR   SPSR

    /* SPI Pin Masks */
    #define BG_SPI_SCLK_MASK 0x02
    #define BG_SPI_MOSI_MASK 0x04
    #define BG_SPI_MISO_MASK 0x08
    
#elif defined(ATMEGA16)
    /* Register Definitions */
    #define  BG_SPI_CR   SPCR
    #define  BG_SPI_DR   SPDR
    #define  BG_SPI_SR   SPSR

    /* SPI Pin Masks */
    #define BG_SPI_MOSI_MASK 0x20
    #define BG_SPI_MISO_MASK 0x40
    #define BG_SPI_SCLK_MASK 0x80
    
#else
    #error "This SPI Driver is only guaranteed to work with the     \
            ATmega16, ATmega128, and ATmeg324. Please define one in \
            the system_config.h file"
#endif

//=================================================================//
//                         SPI Flags                               //
//=================================================================//
#define BG_SPI_LSB_FIRST        0x01
#define BG_SPI_SCLK_IDLE_HIGH   0x02
#define BG_SPI_SAMPLE_RISING    0x04
#define BG_SPI_SCLK_DIV_4       0x00
#define BG_SPI_SCLK_DIV_2       0x10
#define BG_SPI_SCLK_DIV_16      0x20
#define BG_SPI_SCLK_DIV_8       0x30
#define BG_SPI_SCLK_DIV_64      0x40
#define BG_SPI_SCLK_DIV_32      0x50
#define BG_SPI_SCLK_DIV_128     0x60

//=================================================================//
//                          SPI Functions                          //
//=================================================================//
/**
 * Initializes the SPI hardware to operate in master mode with the
 * desired flags. Initializes the ss, sclk, and mosi pins as outputs
 * and the miso pin as input. It also sets the slave select pin.
 * @param ss    Slave Select. Pointer to AVRPin struct
 * @param flags Set of flags that indicate how to set up SPI
 * @see avr_pin
 * @see SPI Flags
 */
void spi_master_init(avr_pin* ss, uint8_t flags);

/**
 * Initializes the SPI hardware to operate in the Slave mode.
 * Firstly, it initializes the sclk, and mosi pins as inputs and
 * the miso pin as output.
 */
void spi_slave_init(void);

/**
 * Initializes the SPI transaction by driving the slave select pin
 * low.
 * @param ss    avr_pin structure for the slave select pin.
 * @see avr_pin
 */
void spi_select_slave(avr_pin* ss);

/**
 * Ends the SPI transaction by driving the slave select pin high.
 * @param ss    avr_pin structure for the slave select pin.
 * @see avr_pin
 */
void spi_deselect_slave(avr_pin* ss);

/**
 * Transmits n bytes of data through hardware SPI.
 * Will return once data has been transmitted.
 * @param data  pointer to data to be transmitted.
 * @param n     number of bytes to be transmitted.
 */
void spi_master_transmit(uint8_t* data, uint8_t n);

/**
 * Transmits a single byte of data through hardware SPI.
 * Will return once data has been transmitted.
 * @param data  byte of data to be transmitted.
 */
void spi_master_transmit8(uint8_t data);

/**
 * Transmits a two bytes of data through hardware SPI.
 * Will return once data has been transmitted.
 * @param data  16-bit data to be transmitted.
 */
void spi_master_transmit16(uint16_t data);

/**
 * Transmits a four bytes of data through hardware SPI.
 * Will return once data has been transmitted.
 * @param data  32-bit data to be transmitted.
 */
void spi_master_transmit32(uint32_t data);

/**
 * Will read one byte from the SPI data register and return it.
 */
uint8_t spi_master_recieve(void);

/**
 * Used to read data from a master through SPI.
 * @return a byte of data received from master.
 */
uint8_t spi_slave_recieve(void);

#endif /* BG_LIB_SPI_DRIVER_H_ */
