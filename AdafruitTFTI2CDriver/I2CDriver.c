/*
 * I2CDriver.c
 *
 * Created: 12/4/2017 6:42:58 PM
 *  Author: gonza
 */ 

void initI2C()
{
    
}

void beginTransmission(uint8_t addr)
{
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
}
