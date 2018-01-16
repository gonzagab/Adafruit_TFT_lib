//*******************************************************************
//* File Name       :  AdafruitTFTButton.c
//*
//* Author          :   Bryant Gonzaga
//* Created         :   12/19/2017 12:10:04 PM
//* Modified        :   12/20/2017
//* Target Device   :   
//* Description:
//*     Structure for the variables need for a button. Also has two
//* methods needed for button.
//*******************************************************************

#include "AdafruitTFTButton.h"

void drawButtonTFT(TSButtonVars* button, TFTVars* tftVars)
{
    if (button->w < 7 || button->h < 7) {
        return;
    }
    uint8_t r;
    uint8_t textSize;
    if (button->w < button->h) {
        r = button->w / 4;
        textSize = button->w / 7;
    } else {
        r = button->h / 4;
        textSize = button->h/7;
    }
    //DRAW OUTLINE
    drawRoundRect(button->x, button->y, button->w, button->h, r, button->outlineColor, tftVars);
    //FILL ROUND RECTANGLE
    fillRoundRect(button->x, button->y, button->w, button->h, r, button->fillColor, tftVars);
    //DRAW BUTTON LABEL
    drawChar(button->x + (button->w - (textSize * 5)) / 2,
             button->y + (button->h - (textSize * 7)) / 2,
             button->label, button->textColor, button->fillColor, textSize, tftVars);
}

bool buttonContainsPointTFT(int16_t x, int16_t y, TSButtonVars* button)
{
    return (
               (x >= button->x) && (x < (button->x + button->w)) &&
               (y >= button->y) && (y < (button->y + button->h))
           );
}
