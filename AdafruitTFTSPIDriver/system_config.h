/********************************************************************
 * File:        system_config.h
 * Author:      Bryant Gonzaga
 * Created:     --/--/2018
 * Modified:    3/30/2018
 *
 * Notes:
 *  This file is to be used with the AdafruitTFTSPIDriver Library.
 *
 * Description:
 *  In order to make the library more versatile this file is used to
 * configure which MCU and which Compiler/IDE users would like to
 * use. At this point the library only supports two microcontrollers
 * and two compilers. The ATmega324 and the ATmega128 are both
 * supported. Also, IAR's Embedded Workbench for Atmel and Atmel
 * Studio 7 are both supported.
 *
 * How To:
 *  1: Choose MCU. Uncomment you choice while leaving the rest
 *     commented out.
 *  2: This is all that needs to be done in this file. Make sure to
 *     include it in your project.
 *******************************************************************/
 
#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

/* Specify Microcontroller */
// #define ATMEGA128
#define ATMEGA324

/* Specify Clock Frequency */
#define SYS_CLK_FREQ 16000000
#define F_CPU SYS_CLK_FREQ

/* Compiler Specific */
#if defined(__ICCAVR__)     // IAR C Compiler
    /* Flash Variables */
    #define FLASH_DECLARE(x) __flash x

    /* Reading From Flash */
    #define READ_FLASH_BYTE(addrs) *( (__flash uint8_t*) addrs )
    #define READ_FLASH_WORD(addrs) *( (__flash uint16_t*) addrs )
   
    /* MCU Include File */
    #ifdef ATMEGA128
        #include <iom128.h>
    #elif defined(ATMEGA324)
        #include <iom324pa.h>    
    #endif

    /* _BV Function */
    #define _BV(bit) (1 << (bit))

    /* Delay Function */
    #define DELAY_MS(mili_secs) __delay_cycles(mili_secs * SYS_CLK_FREQ / 1000)

    /* Interrupts */
    #define ENABLE_INTERRUPTS()       __enable_interrupt()

    /* Include Files Needed */
    #include <intrinsics.h>

#elif defined(__GNUC__)     // GNU Compiler
    /* Flash Variables */
    #define FLASH_DECLARE(x) x __attribute__((__progmem__))
   
    /* Reading From Flash */
    #define READ_FLASH_BYTE(addrs) pgm_read_byte(addrs)
    #define READ_FLASH_WORD(addrs) pgm_read_word(addrs)
    
    /* MCU Include File */
    #include <avr/io.h>
    
    /* _BV Function */
    //included with avr/io.h

    /* Delay Function */
    #define DELAY_MS(mili_secs) _delay_ms(mili_secs)
   
    /* Interrupts */
    #define ENABLE_INTERRUPTS()           sei()

    /* Include Files Needed */
    #include <avr/interrupt.h>
    #include <util/delay.h>
    #include <avr/pgmspace.h>
#endif

/* Compiler Common */
#define READ_FLASH_PTR(addrs) (void*) READ_FLASH_WORD(addrs)

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#endif /* SYSTEM_CONFIG_H_ */
