/*********************************************************************
 * File: spi_driver_hal_defs.h
 *
 * Created: 4/13/2021 10:42:38 AM
 * Author: Bryant Gonzaga
 *
 * Supported MCUs: AVR128DB48, ATMEGA324, ATMEGA128
 * Description:
 *  This file defines macros to be used for interfacing with the
 *  on-chip SPI hardware module of the supported microcontrollers.
 *  Each macro takes the format of BG_SPI_*.
 *  
 *  The first set of macros are to define Ports/Pins that will be used
 *  for the SPI signals (MOSI, MISO, SCLK) SS/CS is not defined. Chip
 *  Select is user defined using the pin_intrf structure. For now only
 *  default pin configurations are supported. In mcus like the
 *  AVR128DB48 SPI pins can be mapped to different I/O pins; this file
 *  only defines the default port/pins SPI mappings.
 *
 *  The next set of macros are to define the Control, Status, and Data
 *  registers of the SPI module along with their associated bit masks.
 *  Since some microcontrollers have multiple control registers we
 *  assume that they are contiguous (like in the AVR128DB48 CNRTLA and
 *  CNTRLB registers). This way we only define one control register -
 *  BG_SPI_CR. The BG_SPI_*_BM are defined with this assumption in
 *  mind. Control bits in a control register, other than the least
 *  significant control register, are shifted accordingly. If an SPI
 *  module only has one control register this shifting is not
 *  necessary.
 *
 *  spi_driver_user_flags.h depends on these bit masks to further
 *  abstract the hardware to user friendly language.
 ********************************************************************/ 

#ifndef BG_LIB_SPI_DRIVER_HAL_DEFS_H_
#define BG_LIB_SPI_DRIVER_HAL_DEFS_H_

//=================================================================//
//                SPI Ports and Pin Masks                          //
//=================================================================//
#if defined(AVR128DB48)
    /* Data Direction Registers */
    #define BG_SPI_SCLK_DDR     PORTA_DIR
    #define BG_SPI_MOSI_DDR     PORTA_DIR
    #define BG_SPI_MISO_DDR     PORTA_DIR
    
    /* Data Input Registers */
    #define BG_SPI_SCLK_PIN     PORTA_IN
    #define BG_SPI_MOSI_PIN     PORTA_IN
    #define BG_SPI_MISO_PIN     PORTA_IN

    /* Data Output Registers */
    #define BG_SPI_SCLK_PORT    PORTA_OUT
    #define BG_SPI_MOSI_PORT    PORTA_OUT
    #define BG_SPI_MISO_PORT    PORTA_OUT
    
    /* SPI Pin Masks */
    #define BG_SPI_MOSI_PIN_BM  0x10
    #define BG_SPI_MISO_PIN_BM  0x20
    #define BG_SPI_SCLK_PIN_BM  0x40
    
#elif defined(ATMEGA324) || defined(ATMEGA16) || defined(ATMEGA128)
    /* Data Direction Registers */
    #define BG_SPI_SCLK_DDR     DDRB
    #define BG_SPI_MOSI_DDR     DDRB
    #define BG_SPI_MISO_DDR     DDRB

    /* Data Input Registers */
    #define BG_SPI_SCLK_PIN     PINB
    #define BG_SPI_MOSI_PIN     PINB
    #define BG_SPI_MISO_PIN     PINB

    /* Data Output Registers */
    #define BG_SPI_SCLK_PORT    PORTB
    #define BG_SPI_MOSI_PORT    PORTB
    #define BG_SPI_MISO_PORT    PORTB
    
    /* SPI Pin Masks */
    #if defined(ATMEGA324) || defined(ATMEGA16)
        #define BG_SPI_MOSI_PIN_BM 0x20
        #define BG_SPI_MISO_PIN_BM 0x40
        #define BG_SPI_SCLK_PIN_BM 0x80
    #elif defined(ATMEGA128)
        #define BG_SPI_MOSI_PIN_BM 0x02
        #define BG_SPI_MISO_PIN_BM 0x04
        #define BG_SPI_SCLK_PIN_BM 0x08
    #endif  
#endif

//=================================================================//
//              Control Registers and Bit Masks                    //
//=================================================================//
#if defined(AVR128DB48)
    /* Register Definitions */
    #define  BG_SPI_CR          *((uint16_t*) (&SPI0_CTRLA))
    #define  BG_SPI_DR          SPI0_DATA
    #define  BG_SPI_SR          SPI0_INTFLAGS

    /* SPI Control Register A Bit Masks */
    #define BG_SPI_ENABLE_BM    SPI_ENABLE_bm
    #define BG_SPI_MSTR_BM      (SPI_MASTER_bm | BG_SPI_SSD_BM)
    #define BG_SPI_DORD_BM      SPI_DORD_bm
    
    /* SPI Control Register B Bit Masks */
    #define BG_SPI_SSD_BM       (SPI_SSD_bm << 0x08)
    #define BG_SPI_CPOL_BM      (SPI_MODE1_bm << 0x08)
    #define BG_SPI_CPHA_BM      (SPI_MODE0_bm << 0x08)
    
    /* SPI Clock Prescalar Group Configurations */
    #define BG_SPI_SCLK_DIV_2_GC    (SPI_CLK2X_bm | SPI_PRESC_DIV4_gc)
    #define BG_SPI_SCLK_DIV_4_GC    (SPI_PRESC_DIV4_gc)
    #define BG_SPI_SCLK_DIV_8_GC    (SPI_CLK2X_bm | SPI_PRESC_DIV16_gc)
    #define BG_SPI_SCLK_DIV_16_GC   (SPI_PRESC_DIV16_gc)
    #define BG_SPI_SCLK_DIV_32_GC   (SPI_CLK2X_bm | SPI_PRESC_DIV64_gc)
    #define BG_SPI_SCLK_DIV_64_GC   (SPI_PRESC_DIV64_gc)
    #define BG_SPI_SCLK_DIV_128_GC  (SPI_PRESC_DIV128_gc)

    /* SPI Status Register Bit Definitions */
    #define BG_SPI_INTF_BM      SPI_IF_bm
    #define WCOL                SPI_WRCOL_bm
    #define SPI2X               SPI_CLK2X_bm
    
#elif defined(ATMEGA324)
    /* Register Definitions */
    #define  BG_SPI_CR          SPCR0
    #define  BG_SPI_DR          SPDR0
    #define  BG_SPI_SR          SPSR0
    
    // TODO: check if  these are actually bit masks or positions
     /* SPI Control Register Bit Definitions */
     #define BG_SPI_ENABLE_BM   SPIE0
     #define BG_SPI_MSTR_BM     MSTR0
     #define BG_SPI_DORD_BM     DORD0
     
     #define BG_SPI_CPOL_BM     CPOL0
     #define BG_SPI_CPHA_BM     CPHA0
     
     /* SPI Clock Prescalar Group Configurations */
     #define BG_SPI_SCLK_DIV_2_GC   (SPI2X0)
     #define BG_SPI_SCLK_DIV_4_GC   (0x00)
     #define BG_SPI_SCLK_DIV_8_GC   (SPI2X0 | SPR00)
     #define BG_SPI_SCLK_DIV_16_GC  (SPR00)
     #define BG_SPI_SCLK_DIV_32_GC  (SPI2X0 | SPR10)
     #define BG_SPI_SCLK_DIV_64_GC  (SPR10)
     #define BG_SPI_SCLK_DIV_128_GC (SPR10 | SPR00)
 
    /* SPI Status Register Bit Masks */
    #define BG_SPI_INTF_BM      SPIF0
    #define WCOL                WCOL0
    #define SPI2X               SPI2X0

#elif defined(ATMEGA128) || defined(ATMEGA16)
    /* Register Definitions */
    #define  BG_SPI_CR      SPCR
    #define  BG_SPI_DR      SPDR
    #define  BG_SPI_SR      SPSR
    // TODO: finish support for atmega128 and atmega16
#else
    #error "This SPI Driver is only guaranteed to work with the     \
    ATmega16, ATmega128, and ATmeg324. Please define one in \
    the system_config.h file"
#endif

#endif /* BG_LIB_SPI_DRIVER_HAL_DEFS_H_ */
