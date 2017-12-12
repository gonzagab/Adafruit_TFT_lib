/*
 * I2CDriver.c
 *
 * Created: 12/4/2017 6:42:58 PM
 *  Author: gonza
 */ 

#include "I2CDriver.h"

bool I2CStartCondition()
{
    //send a start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {        //wait for start condition to be transmitted    }    //check that start condition has been sent    if ((TWSR & 0xF8) != 0x08) {
        return false;
    }
    return true;
}

bool I2CBeginWrite(uint8_t addr)
{
    //address of slave we want to talk to + write bit
    TWDR = (addr << 1) & 0xFE;
    //call that address
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT))) {        //wait for acknowledgment to be received    }    //check that SLA + W was sent and acknowledged
    if ((TWSR & 0xF8) != 0x18) {
        return false;
    }
    return true;
}

bool I2CBeginRead(uint8_t addr)
{
    //address of slave we want to talk to + read bit
    TWDR = (addr << 1) | 0x01;
    //call that address
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    while (!(TWCR & (1 << TWINT))) {        //wait for acknowledgment to be received    }    //check that SLA + R was sent and acknowledged
    if ((TWSR & 0xF8) != 0x40) {
        return false;
    }
    
    return true;
}

bool I2CTransmitData(uint8_t regAddr, uint8_t data)
{
    //put data in two wire data register
    TWDR = data;
    //need to write a one to clear two wire interrupt bit
    TWCR = (1 << TWINT) | (1 << TWEN); //enable write
     
    while (!(TWCR & (1 << TWINT))) {
        //wait for data to be written and get acknowledgment signal
    }
    
    //check status register 
    if ((TWSR & 0xF8) != 0x28) {        return false;    }
    
    return true;
}

void I2CEndTransmission()
{
    //Transmit stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
