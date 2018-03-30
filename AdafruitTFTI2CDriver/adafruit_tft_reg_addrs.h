//*******************************************************************
//* File Name       :  AdafruitTFTRegAddrs.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   12/13/2017 9:19:26 PM
//* Modified        :   12/13/2017
//* Target Device   :   ATmega324A
//* Description:
//*     Addresses provided by Adafruit for their capacitive TFT
//* touchscreen.
//*******************************************************************

#ifndef ADAFRUITTFTREGADDRS_H_
#define ADAFRUITTFTREGADDRS_H_

#define FT6206_ADDR             0x38
#define FT6206_G_FT5201ID       0xA8
#define FT6206_REG_NUMTOUCHES   0x02

#define FT6206_NUM_X            0x33
#define FT6206_NUM_Y            0x34

#define FT6206_REG_MODE         0x00
#define FT6206_REG_CALIBRATE    0x02
#define FT6206_REG_WORKMODE     0x00
#define FT6206_REG_FACTORYMODE  0x40
#define FT6206_REG_THRESHHOLD   0x80
#define FT6206_REG_POINTRATE    0x88
#define FT6206_REG_FIRMVERS     0xA6
#define FT6206_REG_CHIPID       0xA3
#define FT6206_REG_VENDID       0xA8

// calibrated for Adafruit 2.8" ctp screen
#define FT6206_DEFAULT_THRESSHOLD 128

#endif /* ADAFRUITTFTREGADDRS_H_ */
