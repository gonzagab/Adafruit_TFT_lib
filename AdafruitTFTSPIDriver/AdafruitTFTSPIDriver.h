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

typedef struct TFTVars
{
    uint16_t width;
    uint16_t height;
    uint16_t cursor_x;
    uint16_t cursor_y;
    uint8_t rotation; 
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
} TFTVars;

//METHODS TO CONTROL TFT
void initTFT(TFTVars* var);
void writeCommandTFT(uint8_t cmd, TFTVars* var);
void setRotation(uint8_t r, TFTVars* var);
void invertDisplay(bool i, TFTVars* var);
void scrollTo(uint16_t y, TFTVars* var);
void drawPixel(int16_t x, int16_t y, uint16_t color, TFTVars* var);
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, TFTVars* var);
void writePixels(uint16_t * colors, uint32_t len, TFTVars* var);
void writeColor(uint16_t color, uint32_t len, TFTVars* var);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
void fillScreen(uint16_t color, TFTVars* var);
//METHODS FOR LINES
void drawVLineTFT(int16_t x, int16_t y, int16_t h, uint16_t color, TFTVars* var);
void drawHLineTFT(int16_t x, int16_t y, int16_t w, uint16_t color, TFTVars* var);
void drawLineTFT(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, TFTVars* var);
//METHODS FOR RECTANGLES
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFTVars* var);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFTVars* var);
//METHODS FOR CIRCLES
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, TFTVars* var);
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color, TFTVars* var);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, TFTVars* var);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color, TFTVars* var);
//METHODS FOR ROUND RECTANGLES
void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color, TFTVars* var);
void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color, TFTVars* var);
//METHODS FOR TRIANGLES
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFTVars* var);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFTVars* var);
//BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, TFTVars* var);
void drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg, TFTVars* var);
//METHODS FOR TEXT
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size, TFTVars* var);
void write(uint8_t c, TFTVars* var);
void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, TFTVars* var);
void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h, TFTVars* var);
void setFont(const GFXfont* f, TFTVars* var);

#endif /* ADAFRUITTFTSPIDRIVER_H_ */
