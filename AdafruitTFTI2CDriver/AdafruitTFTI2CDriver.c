/*
 * AdafruitTFTI2CDriver.c
 *
 * Created: 12/4/2017 5:36:45 PM
 * Author : Bryant Gonzaga
 */

#include "AdafruitTFTI2CDriver.h"

bool begin(uint8_t threshhold)
{
    Wire.begin();

    // change threshhold to be higher/lower
    writeRegister8(FT6206_REG_THRESHHOLD, threshhold);

    if ((readRegister8(FT6206_REG_VENDID) != 17) || (readRegister8(FT6206_REG_CHIPID) != 6)) { return false; }
    /*
    Serial.print("Vend ID: "); Serial.println(readRegister8(FT6206_REG_VENDID));
    Serial.print("Chip ID: "); Serial.println(readRegister8(FT6206_REG_CHIPID));
    Serial.print("Firm V: "); Serial.println(readRegister8(FT6206_REG_FIRMVERS));
    Serial.print("Point Rate Hz: "); Serial.println(readRegister8(FT6206_REG_POINTRATE));
    Serial.print("Thresh: "); Serial.println(readRegister8(FT6206_REG_THRESHHOLD));
    */
    // dump all registers
    /*
    for (int16_t i=0; i<0x20; i++) {
      Serial.print("I2C $"); Serial.print(i, HEX);
      Serial.print(" = 0x"); Serial.println(readRegister8(i), HEX);
    }
    */
    return true;
}

void writeRegister8(uint8_t reg, uint8_t val) {
    // use i2c
    Wire.beginTransmission(FT6206_ADDR);
    Wire.write((byte)reg);
    Wire.write((byte)val);
    Wire.endTransmission();
}
