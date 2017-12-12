/*
 * AdafruitTFTI2CDriver.c
 *
 * Created: 12/4/2017 5:36:45 PM
 * Author : Bryant Gonzaga
 */

#include "AdafruitTFTI2CDriver.h"

bool begin(uint8_t threshhold)
{
    if (!I2CStartCondition()) {
        return false;
    }        
    
    if (!I2CBeginWrite()) {
        return false;
    }

    // change threshhold to be higher/lower
    writeRegister8(FT6206_REG_THRESHHOLD, threshhold);

    if ((readRegister8(FT6206_REG_VENDID) != 17) || (readRegister8(FT6206_REG_CHIPID) != 6)) {
        return false;
    }

    return true;
}

void writeRegister8(uint8_t reg, uint8_t val) {
    // use i2c
    Wire.I2CBeginWrite(FT6206_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}
