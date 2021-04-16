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
#include "pin_intrf.h"
#include "spi_driver_hal_defs.h"
#include "spi_driver_user_flags.h"

//=================================================================//
//                          SPI Functions                          //
//=================================================================//
/**
 * Initializes the SPI hardware to operate in master mode with the
 * desired flags. Initializes the ss, sclk, and mosi pins as outputs
 * and the miso pin as input. It also sets the slave select pin.
 * @param ss    Slave Select. Pointer to AVRPin struct
 * @param flags Set of flags that indicate how to set up SPI
 * @see pin_intrf
 * @see SPI Flags
 */
void spi_master_init(pin_intrf* ss, uint16_t flags);

/**
 * Initializes the SPI hardware to operate in the Slave mode.
 * Firstly, it initializes the sclk, and mosi pins as inputs and
 * the miso pin as output.
 */
void spi_slave_init(void);

/**
 * Initializes the SPI transaction by driving the slave select pin
 * low.
 * @param ss    pin_intrf structure for the slave select pin.
 * @see pin_intrf
 */
void spi_select_slave(pin_intrf* ss);

/**
 * Ends the SPI transaction by driving the slave select pin high.
 * @param ss    pin_intrf structure for the slave select pin.
 * @see pin_intrf
 */
void spi_deselect_slave(pin_intrf* ss);

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
uint8_t spi_master_receive(void);

/**
 * Used to read data from a master through SPI.
 * @return a byte of data received from master.
 */
uint8_t spi_slave_receive(void);

#endif /* BG_LIB_SPI_DRIVER_H_ */
