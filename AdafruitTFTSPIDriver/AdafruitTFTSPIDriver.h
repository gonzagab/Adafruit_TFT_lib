//*****************************************************************
//* Author:         Bryant Gonzaga
//* Modified:       11/30/2017
//* Name:           AdafruitTFTSPIDriver.h
//* Description:
//*
//********************************************************************

#ifndef ADAFRUITTFTSPIDRIVER_H_
#define ADAFRUITTFTSPIDRIVER_H_

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdbool.h>

#include "CommandDefinitions.h"
#include "PortRegisterStruct.h"
#include "ColorDefinitions.h"
#include "SPIDriver.h"
#include "GFXFont.h"
#include "Font5x7.h"

#define TFT_WIDTH   240
#define TFT_HEIGHT  320

typedef struct TFT_vars
{
    int16_t width;
    int16_t height; // Display w/h as modified by current rotation

    int16_t cursor_x;
    int16_t cursor_y;
    uint8_t rotation; 
    //variables for text
    uint8_t textSize;
    uint16_t textColor;
    uint16_t textBGColor;    
    bool wrap;      // If set, 'wrap' text at right edge of display
    bool cp437;    // If set, use correct CP437 char set (default is off)
    GFXfont *gfxFont;
    
    PortRegisters* cs;
    PortRegisters* dc;
    PortRegisters* rst;
    PortRegisters* mosi;
    PortRegisters* miso;
    PortRegisters* sclk;
} TFT_vars;

////////////// FUNCTIONS //////////////
void initTFT(TFT_vars* var);
void writeCommand(uint8_t cmd, TFT_vars* var);
void setRotation(uint8_t r, TFT_vars* var);
void invertDisplay(bool i, TFT_vars* var);
void scrollTo(uint16_t y, TFT_vars* var);
void drawPixel(int16_t x, int16_t y, uint16_t color, TFT_vars* var);

// Transaction API not used by GFX
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, TFT_vars* var);
void writePixels(uint16_t * colors, uint32_t len, TFT_vars* var);
void writeColor(uint16_t color, uint32_t len, TFT_vars* var);

// Recommended Non-Transaction
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color, TFT_vars* var);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color, TFT_vars* var);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFT_vars* var);

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, TFT_vars* var);
void fillScreen(uint16_t color, TFT_vars* var);
void setCursor(int16_t x, int16_t y, TFT_vars* var);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFT_vars* var);

// These exist only with Adafruit_GFX
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, TFT_vars* var);
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color, TFT_vars* var);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, TFT_vars* var);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color, TFT_vars* var);
void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color, TFT_vars* var);
void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color, TFT_vars* var);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFT_vars* var);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFT_vars* var);
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, TFT_vars* var);

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, TFT_vars* var);
void drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg, TFT_vars* var);

//ADD ONE BY ONE
/*
void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t ask[], int16_t w, int16_t h);
void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
*/


// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size, TFT_vars* var);
//void cp437(bool x = true, TFT_vars* var);
void setFont(const GFXfont* f, TFT_vars* var);
void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h, TFT_vars* var);
//void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void write(uint8_t c, TFT_vars* var);
void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, TFT_vars* var);


//STUFF IDK WHAT TO DO WITH
/*
using Adafruit_GFX::drawRGBBitmap; // Check base class first
void drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h);
uint8_t readcommand8(uint8_t reg, uint8_t index = 0);
uint8_t spiRead(void);
*/

#endif /* ADAFRUITTFTSPIDRIVER_H_ */
