//*******************************************************************
//* File Name       :  AdafruitTFTButton.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   12/19/2017 11:59:15 AM
//* Modified        :   12/20/2017
//* Target Device   :   ATmega324A
//* Description:
//*     Structure for the variables need for a button. Also has two
//* methods needed for button.
//*******************************************************************


#ifndef ADAFRUITTFTBUTTON_H_
#define ADAFRUITTFTBUTTON_H_

#include <stdbool.h>

#include "AdafruitTFTSPIDriver.h"

typedef struct TSButtonVars
{
     int16_t       x, y; //Coordinates of top-left corner
     uint16_t      w, h;
     uint16_t      outlineColor, fillColor, textColor;
     char          label;
} TSButtonVars;
 
 void drawButtonTFT(TSButtonVars* button, TFTVars* tftVars);
 
 bool contains(int16_t x, int16_t y, TSButtonVars* button);

#endif /* ADAFRUITTFTBUTTON_H_ */
