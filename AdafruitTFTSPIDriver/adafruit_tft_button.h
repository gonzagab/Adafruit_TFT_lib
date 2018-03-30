/********************************************************************
 * File:        adafruit_tft_button.h
 * Author:      Bryant Gonzaga
 * Created:     12/19/2017 11:59:15 AM
 * Modified:    3/30/2018
 *
 * Notes:
 *  To be used along side with adafruit_tft_spi_driver.
 *
 * Description:
 *  This file contains a struct, ts_button_vars. This struct has
 * data fields for describing a button. The file also contains two
 * functions for initializing the button and for checking if the
 * button was pressed.
 *
 * How To:
 *  1: Firstly, users should initialize a ts_button_vars struct.
 *  2: Then, call draw_button_tft(), passing it a pointer to the
 *     struct created in 1.
 *  3: The button_contains_point_tft() function can then be used to
 *     detect if a particular button was pressed.
 *******************************************************************/

#ifndef ADAFRUITTFTBUTTON_H_
#define ADAFRUITTFTBUTTON_H_

#include "adafruit_tft_spi_driver.h"

/**
 * A collection of variables needed to create a button.
 */
typedef struct ts_button_vars
{
     int16_t    x;              /**< x-coordinate of button >*/
     int16_t    y;              /**< y-coordinate of button >*/
     uint16_t   w;              /**< Button width. >*/
     uint16_t   h;              /**< Button height >*/
     uint8_t    size;           /** Size of button */
     uint16_t   outlineColor;   /**< Outline of button >*/
     uint16_t   fillColor;      /**< Button color >*/
     uint16_t   textColor;      /**< Color of the button label >*/
     char       label;          /**< Single ASCII character to be
                                     displayed on button >*/
} ts_button_vars;

/**
 * Draws the button specified by the TSButtonVars structure.
 * @param button    Pointer to button variable used to draw button.
 * @param tftVars   Pointer to TFT variables structure.
 */
void draw_button_tft(ts_button_vars* button, tft_vars* tftVars);
 
 /**
  * Is used to check if a point is within a given button
  * @param x        x-position.
  * @param y        y-position.
  * @param button   Pointer to button in question.
  * @return         True if (x, y) are within the button's bounds.
  */
bool button_contains_point_tft(int16_t x, int16_t y, ts_button_vars* button);

#endif /* ADAFRUITTFTBUTTON_H_ */
