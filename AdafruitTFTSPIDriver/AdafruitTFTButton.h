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
     int16_t    x;
     int16_t    y;
     uint16_t      w, h;
     uint16_t      outlineColor, fillColor, textColor;
     char          label;
} TSButtonVars;

/**
 * Draws the button specified by the TSButtonVars structure.
 * @param button    Pointer to button variable used to draw button.
 * @param tftVars   Pointer to TFT variables structure.
 */
void drawButtonTFT(TSButtonVars* button, TFTVars* tftVars);
 
 /**
  * Is used to check if a point is within a given button
  * @param x        x-position.
  * @param y        y-position.
  * @param button   Pointer to button in question.
  * @return         True if (x, y) are within the button's bounds.
  */
bool buttonContainsPointTFT(int16_t x, int16_t y, TSButtonVars* button);

#endif /* ADAFRUITTFTBUTTON_H_ */
