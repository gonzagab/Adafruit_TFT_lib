/********************************************************************
 * File:        spi_driver_user_flags.h
 * Author:      Bryant Gonzaga
 * Created:     4/13/2021 11:53:27 PM
 * Modified:    4/15/2021
 *
 * Description:
 *  This header file defines flags for users to specify their desired
 *  SPI module operation.
 *******************************************************************/ 

#ifndef BG_LIB_SPI_DRIVER_USER_FLAGS_H_
#define BG_LIB_SPI_DRIVER_USER_FLAGS_H_

//=================================================================//
//                         SPI Flags                               //
//=================================================================//
#if defined(AVR128DB48)
    #define BG_SPI_SAMPLE_RISING    0x8000
    #define BG_SPI_SAMPLE_FALLING   0x4000

#elif defined(ATMEGA324) || defined(ATMEGA128)
     #define BG_SPI_SAMPLE_RISING    0x04
     #define BG_SPI_SAMPLE_FALLING   0x00
     
#endif
    
    #define BG_SPI_SCLK_IDLE_LOW    0x00
    #define BG_SPI_SCLK_IDLE_HIGH   BG_SPI_CPOL_BM
    #define BG_SPI_SAMPLE_LEADING   0x00
    #define BG_SPI_SAMPLE_TRAILING  BG_SPI_CPOL_BM

    #define BG_SPI_LSB_FIRST        BG_SPI_DORD_BM
    #define BG_SPI_MSB_FIRST        0x00

    #define BG_SPI_SCLK_DIV_2       BG_SPI_SCLK_DIV_2_GC
    #define BG_SPI_SCLK_DIV_4       BG_SPI_SCLK_DIV_4_GC
    #define BG_SPI_SCLK_DIV_8       BG_SPI_SCLK_DIV_8_GC
    #define BG_SPI_SCLK_DIV_16      BG_SPI_SCLK_DIV_16_GC
    #define BG_SPI_SCLK_DIV_32      BG_SPI_SCLK_DIV_32_GC
    #define BG_SPI_SCLK_DIV_64      BG_SPI_SCLK_DIV_64_GC
    #define BG_SPI_SCLK_DIV_128     BG_SPI_SCLK_DIV_128_GC

#endif /* BG_LIB_SPI_DRIVER_USER_FLAGS_H_ */
