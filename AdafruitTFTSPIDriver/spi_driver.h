/********************************************************************
 * File:        spi_driver.h
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
 *  This is a header file that declares functions for initializing and
 * using the hardware SPI on the MCU. It provides support for Master
 * and Slave mode of the MCU.
 *******************************************************************/

#ifndef BG_LIB_SPI_DRIVER_H_
#define BG_LIB_SPI_DRIVER_H_

#include "system_config.h"
#include "avr_pin.h"

#if defined(ATMEGA324)
    #define  SPCR   SPCR0
    #define  SPDR   SPDR0
    #define  SPSR   SPSR0
    
    /* SPI Status Register - SPSR */
    #define SPIF    SPIF0
    #define WCOL    WCOL0
    #define SPI2X   SPI2X0

    /* SPI Control Register - SPCR */
    #define SPIE    SPIE0
    #define SPE     SPE0
    #define DORD    DORD0
    #define MSTR    MSTR0
    #define CPOL    CPOL0
    #define CPHA    CPHA0
    #define SPR1    SPR10
    #define SPR0    SPR00
#elif defined(ATMEGA128)
    // Names are already set
#else
    #warning "This SPI Driver is only guaranteed to work with the \
              ATmeg324 and ATmega128. Please define one in the    \
              system_config.h file"
#endif

/**
 * Initializes the SPI hardware to operate in the Master mode, to
 * send the most significant bit first, and to have slave clock
 * frequency at half the system clock.
 * Initializes the ss, sclk, and mosi pins as outputs
 * and the miso pin as input. It also drives the mosi and sclk pins
 * low and the ss pin high.
 * @param ss    Slave Select. Pointer to AVRPin struct
 * @param sclk  Slave Clock. Pointer to AVRPin struct
 * @param mosi  Master Out Slave In. Pointer to AVRPin struct
 * @param miso  Master In Slave Out. Pointer to AVRPin struct
 * @see AVRPin
 */
void spi_master_init(avr_pin* ss, avr_pin* sclk, avr_pin* mosi, avr_pin* miso);

/**
 * Initializes the SPI hardware to operate in the Slave mode.
 * Firstly, it initializes the ss, sclk, and mosi pins as inputs and
 * the miso pin as output.
 * @param ss    Slave Select. Pointer to AVRPin struct
 * @param sclk  Slave Clock. Pointer to AVRPin struct
 * @param mosi  Master Out Slave In. Pointer to AVRPin struct
 * @param miso  Master In Slave Out. Pointer to AVRPin struct
 * @see AVRPin
 */
void spi_slave_init(avr_pin* ss, avr_pin* sclk, avr_pin* mosi, avr_pin* miso);

/**
 * Empty for now.
 */
void spiSetClkPrescalar(uint8_t prescalar);

/**
 * Initializes the SPI transaction by driving the slave select pin
 * low.
 * @param ss    AVRPin structure for the slave select pin.
 */
void spi_start_transmission(avr_pin* ss);

/**
 * Transmits a single byte of data through hardware SPI.
 * Will return once data has been transmitted
 * @param data  byte of data to be transmitted.
 */
void spi_master_transmit(uint8_t data);

/**
 * Transmits a two bytes of data through hardware SPI.
 * Will return once data has been transmitted
 * @param data  16-bit data to be transmitted.
 */
void spi_master_transmit16(uint16_t data);

/**
 * Transmits a four bytes of data through hardware SPI.
 * Will return once data has been transmitted
 * @param data  32-bit data to be transmitted.
 */
void spi_master_transmit32(uint32_t data);

/**
 * Ends the SPI transaction by driving the slave select pin high.
 * @param ss    AVRPin structure for the slave select pin.
 */
void spi_end_transmission(avr_pin* ss);

/**
 * Used to read data from a master through SPI.
 * @return a byte of data received from master.
 */
uint8_t spi_slave_recieve(void);

#endif /* BG_LIB_SPI_DRIVER_H_ */
