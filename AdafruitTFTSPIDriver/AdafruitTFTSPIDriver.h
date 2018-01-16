//*******************************************************************
//* File Name       :  AdafruitTFTSPIDriver.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   11/27/2017
//* Modified        :   12/20/2017
//* Target Device   :   ATmega324A
//* Description:
//*     Contains all the methods used to communicate with the
//* Adafruit touch screen. This file specifically contains
//* methods to send data or commands to the touch screen. All methods
//* use SPI to send data.
//* Notes:
//*     Enable (or disable) Code Page 437-compatible char set.
//* There was an error in glcdfont.c for the longest time -- one character
//* (#176, the 'light shade' block) was missing -- this threw off the index
//* of every character that followed it.  But a TON of code has been written
//* with the erroneous character indices.  By default, the library uses the
//* original 'wrong' behavior and old sketches will still work.  Pass 'true'
//* to this function to use correct CP437 character values in your code.
//*******************************************************************
#ifndef ADAFRUITTFTSPIDRIVER_H_
#define ADAFRUITTFTSPIDRIVER_H_

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdbool.h>

#include "AdafruitTFTCommands.h"
#include "PortRegisterStruct.h"
#include "AdafruitTFTColors.h"
#include "SPIDriver.h"
#include "GFXFont.h"
#include "Font5x7.h"

#define TFT_WIDTH   240
#define TFT_HEIGHT  320

/**
 * Contains all the variables associated with the touchscreen.
 */
typedef struct TFTVars
{
    uint16_t width;     /**< Screen width according to rotation. >*/
    uint16_t height;    /**< Screen height according to rotation. >*/
    uint16_t cursor_x;  /**< Cursor x-position used in some methods
                             to specify where to display. >*/
    uint16_t cursor_y;  /**< Cursor y-position used in some methods
                             to specify where to display. >*/
    uint8_t rotation;   /**< Current orientation of the screen >*/
    uint8_t textSize;   /**< Multiplier for the font >*/
    uint16_t textColor; /**< Color of text >*/
    uint16_t textBGColor;
    bool wrap;          /**< If set, 'wrap' text at right edge of
                             display >*/
    bool cp437;         /**< If set, use correct CP437 char set
                             (default is off) >*/
    GFXfont *gfxFont;   /**< Pointer to a GFXfont structure for
                             custom fonts. >*/  
    AVRPin* cs;         /**< Chip Select. >*/
    AVRPin* dc;         /**< Data or Command. Low indicates command
                             and high indicates data. >*/
    AVRPin* rst;        /**< Reset. TFT has a active low reset. >*/
    AVRPin* mosi;       /**< Master Out Slave In SPI Pin. >*/
    AVRPin* miso;       /**< Master In Slave Out SPI Pin. >*/
    AVRPin* sclk;       /**< Slave Clock SPI Pin. >*/
} TFTVars;

/**
 * Initializes the SPI hardware with SUCH PARAMETERS. As well as
 * initiating the touchscreen.
 * @param var   Pointer to TFTVars structure that contains the
 *              current variables used for the touchscreen.
 */
void initTFT(TFTVars* var);

/**
 * Sends a command to the touchscreen through SPI.
 * @param cmd   8-bit command
 * @param var   pointer to TFTVars structure.
 */
void writeCommandTFT(uint8_t cmd, TFTVars* var);

/**
 * Sets the rotation of the screen. The screen can have four
 * different rotations. Updates the width and height according to
 * rotation.
 * Rotation 0 has 0 for x and y at the bottom left corner. Where x
 * is height and y is width. Each subsequent rotation turns the axes
 * clock wise. Where rotation 1 has the 0 at the upper left corner
 * and the x-axis as width and y-axis as height.
 * @param r     rotation can be from 0 to 3.
 * @param var   pointer to TFTVars structure
 */
void setRotationTFT(uint8_t r, TFTVars* var);

/**
 * Invert display according to the boolean value of <code> i </code>
 * @param i     boolean value; true to invert display.
 * @param var   pointer to TFTVars structure.
 */
void invertDisplay(bool i, TFTVars* var);

void scrollTo(uint16_t y, TFTVars* var);

/**
 * Colors a specified pixel to the specified color
 * @param x     x location of pixel
 * @param y     y location of pixel
 * @param color color of pixel
 * @param var   pointer to TFTVars structure
 */
void drawPixel(int16_t x, int16_t y, uint16_t color, TFTVars* var);
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, TFTVars* var);
void writePixels(uint16_t * colors, uint32_t len, TFTVars* var);
void writeColor(uint16_t color, uint32_t len, TFTVars* var);

/**
 * Pass 8-bit (each) r, g, and b and get back 16-bit packed color.
 * @param r 8-bit r.
 * @param g 8-bit g.
 * @param b 8-bit b.
 * @return 16-bit packed color
 */
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

/**
 * Fills the whole screen with the specified color
 * @param color Color the screen is to be filled with.
 * @param var   Pointer to TFTVars structure.
 */
void fillScreenTFT(uint16_t color, TFTVars* var);

/**
 * Draws a vertical line on the screen of length <code> h </code>
 * starting form ( <code> x </code>, <code> y </code>).
 * @param x     x starting position.
 * @param y     y starting position.
 * @param h     Height of the line.
 * @param color Color of line.
 * @param var   Pointer to TFTVars structure.
 */
void drawVLineTFT(int16_t x, int16_t y, int16_t h, uint16_t color, TFTVars* var);

/**
 * Draws a horizontal line on the screen of length <code> w </code>
 * starting form ( <code> x </code>, <code> y </code>).
 * @param x     x starting position.
 * @param y     y starting position.
 * @param w     Width of the line.
 * @param color Color of line.
 * @param var   Pointer to TFTVars structure.
 */
void drawHLineTFT(int16_t x, int16_t y, int16_t w, uint16_t color, TFTVars* var);

/**
 * Draws a line on the screen of from (x0,  y0) to (x1, y1)
 * @param x0    x starting position.
 * @param y0    y starting position.
 * @param x1    x ending position.
 * @param y1    y ending position.
 * @param color Color of line.
 * @param var   Pointer to TFTVars structure.
 */
void drawLineTFT(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, TFTVars* var);

/**
 * Draws a filled rectangle with dimensions <code> w </code> and
 * <code> h </code>.
 * @param x     x starting position
 * @param y     y starting position
 * @param w     Width of rectangle
 * @param h     Height of rectangle
 * @param color Color of rectangle
 * @param var   Pointer to TFTVars structure
 */
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFTVars* var);

/**
 * Draws a hollow rectangle with dimensions <code> w </code> and
 * <code> h </code>.
 * @param x     x starting position
 * @param y     y starting position
 * @param w     Width of rectangle
 * @param h     Height of rectangle
 * @param color Color of rectangle
 * @param var   Pointer to TFTVars structure
 */
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFTVars* var);

/**
 * Draws a hollow circle with radius <code> r </code>.
 * @param x0    x center position.
 * @param y0    y center position.
 * @param r     Circle radius.
 * @param color Circle color
 * @param var   Pointer to TFTVars structure
 */
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, TFTVars* var);

/**
 * Draws a filled circle with radius <code> r </code>
 * @param x     x center position.
 * @param y     y center position.
 * @param r     Circle radius.
 * @param color Circle color
 * @param var   Pointer to TFTVars structure
 */
void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color, TFTVars* var);

/**
 * Draws a hollow rectangle with dimensions <code> w </code> and
 * <code> h </code> and rounded corners.
 * @param x         x starting position
 * @param y         y starting position
 * @param w         Width of rectangle
 * @param h         Height of rectangle
 * @param radius    How rounded out corners should be
 * @param color     Color of rectangle
 * @param var       Pointer to TFTVars structure
 */
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color, TFTVars* var);

/**
 * Helper method to help with drawing circles. Used for round
 * rectangles.
 */
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color, TFTVars* var);

/**
 * Draws a filled rectangle with dimensions <code> w </code> and
 * <code> h </code> and rounded corners.
 * @param x         x starting position
 * @param y         y starting position
 * @param w         Width of rectangle
 * @param h         Height of rectangle
 * @param radius    How rounded out corners should be
 * @param color     Color of rectangle
 * @param var       Pointer to TFTVars structure
 */
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color, TFTVars* var);

/**
 * Helper method to help with drawing circles. Used for round
 * rectangles.
 */
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color, TFTVars* var);

/**
 * Draws a hollow triangle with the three specified corners.
 * @param x0    x-position of corner 0.
 * @param y0    y-position of corner 0.
 * @param x1    x-position of corner 1.
 * @param y1    y-position of corner 1.
 * @param x2    x-position of corner 2.
 * @param y2    y-position of corner 2.
 * @param color Color of triangle.
 * @param var   Pointer to TFTVars structure.
 */
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFTVars* var);

/**
 * Draws a filled triangle with the three specified corners.
 * @param x0    x-position of corner 0.
 * @param y0    y-position of corner 0.
 * @param x1    x-position of corner 1.
 * @param y1    y-position of corner 1.
 * @param x2    x-position of corner 2.
 * @param y2    y-position of corner 2.
 * @param color Color of triangle.
 * @param var   Pointer to TFTVars structure.
 */
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFTVars* var);

/**
 * Draws the specified character onto the screen
 * @param x     x coordinate for where to place character.
 * @param y     y coordinate for where to place character.
 * @param c     Character to be place. ASCII char.
 * @param color Color of character.
 * @param bg    Background color for character.
 * @param size  Size of character to be displayed on screen.
 * @param var   Pointer to TFTVars data structure.
 */
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size, TFTVars* var);

/**
 * Draws the specified character onto the screen. This method uses
 * the values stored in the TFTVars structure to format the character
 * accordingly. Users should pass a pointer that points to a
 * structure with already set values for <code> cursor_x </code>,
 * <code> cursor_y </code>, <code> textSize </code>, <code> textColor
 * </code>, and <code> textBGColor </code>.
 * @param c     Character to be place. ASCII char.
 * @param var   Pointer to TFTVars data structure.
 */
void write(uint8_t c, TFTVars* var);

/**
 * Used by both the PROGMEM- and RAM-resident
 * getTextBounds() functions.
 */
void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, TFTVars* var);

/** 
 * Pass string and a cursor position, returns UL corner and W, H.
 */
void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h, TFTVars* var);

/**
 * 
 */
void setFont(const GFXfont* f, TFTVars* var);

/**
 * Draw a PROGMEM-resident 1-bit image at the specified (x,y)
 * position, using the specified foreground color (unset bits are
 * transparent).
 */
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, TFTVars* var);

/**
 * Draw a PROGMEM-resident 1-bit image at the specified (x,y)
 * position, using the specified foreground (for set bits) and
 * background (unset bits) colors.
 */
void drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg, TFTVars* var);

#endif /* ADAFRUITTFTSPIDRIVER_H_ */
