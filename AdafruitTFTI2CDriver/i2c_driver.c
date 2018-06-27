//*******************************************************************
//* File Name       :  I2CDriver.c
//*
//* Author          :   Bryant Gonzaga
//* Created         :   12/4/2017 6:42:38 PM
//* Modified        :   1/13/2018
//* Target Device   :   ATmega324A
//* Description:
//*     Contains several functions to implement the I2C communication
//* in hardware of the Atmega324A. To implement I2C follow these
//* steps:
//* 1) Start Condition.
//* 2) Begin write by sending address of slave.
//* 3) Send register address to write to.
//* 4) Send data.
//* 5) Stop Condition.
//*******************************************************************

#include "i2c_driver.h"

bool i2c_start_condition(void)
{
    //send a start condition
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT))) {        //wait for start condition to be transmitted    }    //check that start condition has been sent    if ((TWSR & 0xF8) != 0x08) {
        return false;
    }
    return true;
}

bool i2c_begin_write(uint8_t addr)
{
    //address of slave we want to talk to + write bit
    TWDR = (addr << 1) & 0xFE;
    //call that address
    TWCR = _BV(TWINT) | _BV(TWEN);
    
    while (!(TWCR & _BV(TWINT))) {        //wait for acknowledgment to be received    }    //check that SLA + W was sent and acknowledged
    if ((TWSR & 0xF8) != 0x18) {
        return false;
    }
    return true;
}

bool i2c_begin_read(uint8_t addr)
{
    //address of slave we want to talk to + read bit
    TWDR = (addr << 1) | 0x01;
    //call that address
    TWCR = _BV(TWINT) | _BV(TWEN);
    
    while (!(TWCR & _BV(TWINT))) {        //wait for acknowledgment to be received    }    //check that SLA + R was sent and acknowledged
    if ((TWSR & 0xF8) != 0x40) {
        return false;
    }
    return true;
}

bool i2c_transmit(uint8_t data)
{
    //put data in two wire data register
    TWDR = data;
    //need to write a one to clear two wire interrupt bit
    TWCR = _BV(TWINT) | _BV(TWEN); //enable write
    while (!(TWCR & _BV(TWINT))) {
        //wait for data to be written and get acknowledgment signal
    }
    //check status register 
    if ((TWSR & 0xF8) != 0x28) {        return false;    }
    return true;
}

bool i2c_read_data(uint8_t* dataAddr, uint8_t numOfBytes)
{
    for (int i = numOfBytes; i > 0; i--) {
        if (i == 1) {
            //ready to receive last data byte
            TWCR = _BV(TWINT) | _BV(TWEN);
        } else {
            //ready to receive data repeatedly
            TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
        }
            
        while ( !(TWCR & _BV(TWINT)) ) {
            //wait to receive data
        }
        //check status register
        if ( (TWSR & 0xF8) != 0x58 && (TWSR & 0xF8) != 0x50 ) {
            return false;
        }
        //read data
        *(dataAddr++) = TWDR;    
    }
            
    return true;
}

void i2c_stop_condition(void)
{
    //Transmit stop condition
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}
