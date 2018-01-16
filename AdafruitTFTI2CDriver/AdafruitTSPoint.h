//*******************************************************************
//* File Name       :  AdafruitTSPoint.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   12/15/2017 2:04:12 PM
//* Modified        :   12/15/2017
//* Target Device   :
//* Description:
//*     A point on the touch screen described in three planes: x, y,
//* z.
//*******************************************************************


#ifndef ADAFRUITTSPOINT_H_
#define ADAFRUITTSPOINT_H_

typedef struct TS_Point
{
    int16_t x;
    int16_t y;
    int16_t z;
} TS_Point;

#endif /* ADAFRUITTSPOINT_H_ */
