//*****************************************************************
//* Author  :   Bryant Gonzaga
//* Created :   12/4/2017 5:36:45 PM
//* Name    :   AdafruitTFTI2CDriver.c
//* Modified:   12/15/2017
//* Description:
//*
//********************************************************************

#include "AdafruitTFTI2CDriver.h"

bool initCPTTS(uint8_t threshhold)
{
    // change threshhold to be higher/lower
    writeRegister8(FT6206_REG_THRESHHOLD, threshhold);

    if ((readRegister8(FT6206_REG_VENDID) != 17) ||
        (readRegister8(FT6206_REG_CHIPID) != 6)) {
        return false;
    }

    return true;
}

bool writeRegister8(uint8_t reg, uint8_t val)
{
    //START CONDITION
    if (!i2cStartCondition()) {
        return false;
    }
    //CALL SPECIFIC SLAVE
    if (!i2cBeginWrite(FT6206_ADDR)) {
        return false;
    }
    //WRITE TO SPECIFIC REGISTER
    if (!i2cTransmit(reg)) {
        return false;  
    }
    //WRITE DATA TO REGISTER
    if (!i2cTransmit(val)) {
        return false;
    }
    //STOP CONDITION
    i2cStopCondition();
    return true;
}

uint8_t readRegister8(uint8_t reg)
{
    uint8_t data;
    //START CONDITION
    if (!i2cStartCondition()) {
        return false;
    }
    //SEND SLAVE ADDRESS FOR WRITE
    if (!i2cBeginWrite(FT6206_ADDR)) {
        return false;
    }
    //SEND REGISTER ADDRESS TO BE READ
    if (!i2cTransmit(reg)) {
        return false;
    }
    //SEND STOP CONDITION
    i2cStopCondition();
    //START CONDITION
    if (!i2cStartCondition()) {
        return false;
    }
    //SEND SLAVE ADDRESS FOR READ
    if (!i2cBeginRead(FT6206_ADDR)) {
        return false;
    }
    //READ DATA
    if (!i2cReadData(&data, 1)) {
        return false;
    }
    //SEND STOP CONDITION
    i2cStopCondition();

    return data;
}

void readData(uint16_t* x, uint16_t* y, TSVars* var)
{
    uint8_t i2cdat[16];
    
    //START CONDITION
    i2cStartCondition();
    //SEND SLAVE ADDRESS FOR WRITE
    i2cBeginWrite(FT6206_ADDR);
    //SEND 0 FOR REGISTER
    i2cTransmit(0);
    //STOP CONDITION
    i2cStopCondition();
    
    //START CONDITION
    i2cStartCondition();
    //SEND SLAVE ADDRESS FOR READ
    i2cBeginRead(FT6206_ADDR);
    //READ FROM REGISTERS
    i2cReadData(i2cdat, 16);
    //STOP CONDITION
    i2cStopCondition();

    var->touches = i2cdat[2];

    if (var->touches > 2) {
        var->touches = 0;
        *x = *y = 0;
    }
    if (var->touches == 0) {
        *x = *y = 0;
        return;
    }
    for (uint8_t i = 0; i < 2; i++) {
        var->touchX[i] = i2cdat[0x03 + i*6] & 0x0F;
        var->touchX[i] <<= 8;
        var->touchX[i] |= i2cdat[0x04 + i*6]; 
        var->touchY[i] = i2cdat[0x05 + i*6] & 0x0F;
        var->touchY[i] <<= 8;
        var->touchY[i] |= i2cdat[0x06 + i*6];
        var->touchID[i] = i2cdat[0x05 + i*6] >> 4;
    }

    *x = var->touchX[0]; *y = var->touchY[0];
}

bool touched()
{
    uint8_t n = readRegister8(FT6206_REG_NUMTOUCHES);
    if ((n == 1) || (n == 2)) {
        return true;
    }        
    return false;
}

TS_Point getPoint(TSVars* var)
{
    uint16_t x, y;
    uint8_t z;
    readData(&x, &y, var);
    TS_Point point;
    point.x = x;
    point.y = y;
    point.z = 1;
    return point;
}
