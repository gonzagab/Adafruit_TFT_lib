/*
 * AdafruitTFTI2CDriver.h
 *
 * Created: 12/4/2017 5:42:04 PM
 *  Author: Bryant Gonzaga
 */ 


#ifndef ADAFRUITTFTI2CDRIVER_H_
#define ADAFRUITTFTI2CDRIVER_H_

#define FT6206_ADDR           0x38
#define FT6206_G_FT5201ID     0xA8
#define FT6206_REG_NUMTOUCHES 0x02

#define FT6206_NUM_X             0x33
#define FT6206_NUM_Y             0x34

#define FT6206_REG_MODE 0x00
#define FT6206_REG_CALIBRATE 0x02
#define FT6206_REG_WORKMODE 0x00
#define FT6206_REG_FACTORYMODE 0x40
#define FT6206_REG_THRESHHOLD 0x80
#define FT6206_REG_POINTRATE 0x88
#define FT6206_REG_FIRMVERS 0xA6
#define FT6206_REG_CHIPID 0xA3
#define FT6206_REG_VENDID 0xA8

// calibrated for Adafruit 2.8" ctp screen
#define FT6206_DEFAULT_THRESSHOLD 128


bool begin(uint8_t thresh = FT6206_DEFAULT_THRESSHOLD);



#endif /* ADAFRUITTFTI2CDRIVER_H_ */