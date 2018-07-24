/********************************************************************
 * File:        adafruit_tft_button.h
 * Author:      Bryant Gonzaga
 * Created:     12/19/2017 12:10:04 PM
 * Modified:    3/30/2018
 *
 * Notes:
 *  To be used along side with adafruit_tft_spi_driver.
 *
 * Description:
 *  This file contains the definition of the two functions for
 * initializing the button and for checking if the button was
 * pressed.
 *
 * How To:
 *  1: Firstly, users should initialize a ts_button_vars struct.
 *  2: Then, call draw_button_tft(), passing it a pointer to the
 *     struct created in 1.
 *  3: The button_contains_point_tft() function can then be used to
 *     detect if a particular button was pressed.
 *******************************************************************/

#include "adafruit_tft_button.h"

void draw_button_tft(ts_button_vars* button, tft_vars* tftVars)
{
    if (button->size < 1) {
        return;
    }
    
    uint8_t textSize = button->size;
    button->w = (button->size * 5) + (button->size * 4);
    button->h = (button->size * 7) + (button->size * 4);
    uint8_t r = button->w / 4;
    
    
    //DRAW OUTLINE
    draw_round_rect_tft(button->x, button->y, button->w, button->h, r, button->outlineColor, tftVars);
    //FILL ROUND RECTANGLE
    fillRoundRect(button->x, button->y, button->w, button->h, r, button->fillColor, tftVars);
    //DRAW BUTTON LABEL
    drawChar(button->x + (button->w - (textSize * 5)) / 2,
             button->y + (button->h - (textSize * 7)) / 2,
             button->label, button->textColor, button->fillColor, textSize, tftVars);
}

bool button_contains_point_tft(int16_t x, int16_t y, ts_button_vars* button)
{
    return (
               (x >= button->x) && (x < (button->x + button->w)) &&
               (y >= button->y) && (y < (button->y + button->h))
           );
}
