//********************************************************************
//* AdafruitTFTLibTest.cpp
//*
//* Created     : 12/3/2017 10:12:53 PM
//* Author      : Bryant Gonzaga
//* Description:
//*     This program tests out varies functions of the SPI TFT Library
//********************************************************************

#include <avr/io.h>
#include <stddef.h>

#include "AdafruitTFTSPIDriver/AdafruitTFTSPIDriver.h"

int main(void)
{
    //INITIALIZE VARIABLES FOR TFT
    TFT_vars tftVars;
    //INITIALIZE PORTS FOR TFT
    PortRegisters cs;
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x10;
    tftVars.cs = &cs;

    PortRegisters dc;
    dc.DDRx = &DDRB;
    dc.PORTx = &PORTB;
    dc.PINx = &PINB;
    dc.mask = 0x08;
    tftVars.dc = &dc;

    PortRegisters rst;
    rst.DDRx = &DDRB;
    rst.PORTx = &PORTB;
    rst.PINx = &PINB;
    rst.mask = 0x10;
    tftVars.rst = &rst;

    PortRegisters sclk;
    sclk.DDRx = &DDRB;
    sclk.PORTx = &PORTB;
    sclk.PINx = &PINB;
    sclk.mask = 0x80;
    tftVars.sclk = &sclk;

    PortRegisters mosi;
    mosi.DDRx = &DDRB;
    mosi.PORTx = &PORTB;
    mosi.PINx = &PINB;
    mosi.mask = 0x20;
    tftVars.mosi = &mosi;

    PortRegisters miso;
    miso.DDRx = &DDRB;
    miso.PORTx = &PORTB;
    miso.PINx = &PINB;
    miso.mask = 0x40;
    tftVars.miso = &miso;

    //INITIALIZE TOUCH SCREEN
    initTFT(&tftVars);

	testFillScreen(&tftVars);
	testLines(ILI9341_BLUE, &tftVars);
	testRects(ILI9341_YELLOW, &tftVars);
	testFastLines(ILI9341_WHITE, ILI9341_BLUE, &tftVars);
	testFilledRects(ILI9341_BLUE, ILI9341_RED, &tftVars);
	testCircles(20, ILI9341_BLUE, &tftVars);
	testFilledCircles(20, ILI9341_BLUE, &tftVars);
	testTriangles(&tftVars);
	testFilledTriangles(&tftVars);
	testRoundRects(&tftVars);
	testFilledRoundRects(&tftVars);
	testText(&tftVars);
	
	
    while (1) {
		for (uint8_t rotation = 0; rotation < 4; rotation++) {
			setRotation(rotation, &tftVars);
			testText(&tftVars);
			_delay_ms(5000);
		}
    }
}

void testFillScreen(TFT_vars* tftVars)
{
    fillScreen(ILI9341_BLACK       , tftVars);
    fillScreen(ILI9341_NAVY        , tftVars);
    fillScreen(ILI9341_DARKGREEN   , tftVars);
    fillScreen(ILI9341_DARKCYAN    , tftVars);
    fillScreen(ILI9341_MAROON      , tftVars);
    fillScreen(ILI9341_PURPLE      , tftVars);
    fillScreen(ILI9341_OLIVE       , tftVars);
    fillScreen(ILI9341_LIGHTGREY   , tftVars);
    fillScreen(ILI9341_DARKGREY    , tftVars);
    fillScreen(ILI9341_BLUE        , tftVars);
    fillScreen(ILI9341_GREEN       , tftVars);
    fillScreen(ILI9341_CYAN        , tftVars);
    fillScreen(ILI9341_RED         , tftVars);
    fillScreen(ILI9341_MAGENTA     , tftVars);
    fillScreen(ILI9341_YELLOW      , tftVars);
    fillScreen(ILI9341_WHITE       , tftVars);
    fillScreen(ILI9341_ORANGE      , tftVars);
    fillScreen(ILI9341_GREENYELLOW , tftVars);
    fillScreen(ILI9341_PINK        , tftVars);
}

void testLines(uint16_t color, TFT_vars* tftVars)
{
    int x1;
    int y1;
    int x2;
    int y2;
    int w = tftVars->width;
    int h = tftVars->height;

    fillScreen(ILI9341_BLACK, tftVars);
    x1 = y1 = 0;
    y2 = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }

    fillScreen(ILI9341_BLACK, tftVars);
    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }
    x2 = 0;
    for (y2=0; y2<h; y2+=6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }

    fillScreen(ILI9341_BLACK, tftVars);
    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    for (x2 = 0; x2 < w; x2 += 6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }

    fillScreen(ILI9341_BLACK, tftVars);
    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;

    for (x2 = 0; x2 < w; x2 += 6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }

    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) {
        drawLine(x1, y1, x2, y2, color, tftVars);
    }
}

void testRects(uint16_t color, TFT_vars* tftVars)
{
	int           n, i, i2,
	cx = tftVars->width  / 2,
	cy = tftVars->height / 2;

	fillScreen(ILI9341_BLACK, tftVars);
	if (tftVars->width < tftVars->height) {
		n = tftVars->width;
	} else {
		n = tftVars->height;
	}

	for(i = 2; i < n; i += 6) {
		i2 = i / 2;
		drawRect(cx-i2, cy-i2, i, i, color, tftVars);
	}
}

void testFastLines(uint16_t color1, uint16_t color2, TFT_vars* tftVars)
{
	int  x, y, w = tftVars->width, h = tftVars->height;

	fillScreen(ILI9341_BLACK, tftVars);
	
	for(y=0; y<h; y+=5) drawFastHLine(0, y, w, color1, tftVars);
	for(x=0; x<w; x+=5) drawFastVLine(x, 0, h, color2, tftVars);
}

void testFilledRects(uint16_t color1, uint16_t color2, TFT_vars* tftVars)
{
	int n, i, i2,
	
	cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fillScreen(ILI9341_BLACK, tftVars);
	
	if (tftVars->width < tftVars->height) {
		n = tftVars->width;
	} else {
		n = tftVars->height;
	}	
	
	for (i = n; i > 0; i -= 6) {
		i2    = i / 2;
			
		fillRect(cx-i2, cy-i2, i, i, color1, tftVars);
		drawRect(cx-i2, cy-i2, i, i, color2,tftVars);
	}
}

void testCircles(uint8_t radius, uint16_t color, TFT_vars* tftVars)
{
	int x, y, r2 = radius * 2,
	w = tftVars->width + radius,
	h = tftVars->height + radius;
	
	fillScreen(ILI9341_BLACK, tftVars);

	for (x = 0; x < w; x += r2) {
		for(y = 0; y < h; y += r2) {
			drawCircle(x, y, radius, color, tftVars);
		}
	}
}

void testFilledCircles(uint8_t radius, uint16_t color, TFT_vars* tftVars)
{
	int x, y, w = tftVars->width, h = tftVars->height, r2 = radius * 2;

	fillScreen(ILI9341_BLACK, tftVars);
	
	for(x=radius; x<w; x+=r2) {
		for(y=radius; y<h; y+=r2) {
			fillCircle(x, y, radius, color, tftVars);
		}
	}
}

void testTriangles(TFT_vars* tftVars)
{
	int n, i, cx = tftVars->width / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fillScreen(ILI9341_BLACK, tftVars);
	
	if (cx < cy) {
		n = cx;
	} else {
		n = cy;
	}

	for(i = 0; i < n; i += 5) {
		drawTriangle(
		cx    , cy - i, // peak
		cx - i, cy + i, // bottom left
		cx + i, cy + i, // bottom right
		color565(i, i, i), tftVars);
	}
}

void testFilledTriangles(TFT_vars* tftVars)
{
	int i, cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fillScreen(ILI9341_BLACK, tftVars);
	
	if (cx < cy) {
		i = cx;
	} else {
		i = cy;
	}
	for (i; i > 10; i -= 5) {
		fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		color565(0, i*10, i*10), tftVars);

		drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		color565(i*10, i*10, 0), tftVars);
	}
}

void testRoundRects(TFT_vars* tftVars)
{
	int w, i, i2,
	cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fillScreen(ILI9341_BLACK, tftVars);
	
	if (tftVars->width < tftVars->height) {
		w = tftVars->width;
	} else {
		w = tftVars->height;
	}
	for(i=0; i<w; i+=6) {
		i2 = i / 2;
		drawRoundRect(cx-i2, cy-i2, i, i, i/8, color565(i, 0, 0), tftVars);
	}
}

void testFilledRoundRects(TFT_vars* tftVars)
{
	int i, i2,
	cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fillScreen(ILI9341_BLACK, tftVars);
	
	if (tftVars->width < tftVars->height) {
		i = tftVars->width;
	} else {
		i = tftVars->height;
	}
	for(i; i>20; i-=6) {
		i2 = i / 2;
		fillRoundRect(cx-i2, cy-i2, i, i, i/8, color565(0, i, 0), tftVars);
	}
}

void testText(TFT_vars* tftVars)
{
    fillScreen(ILI9341_BLACK, tftVars);

	tftVars->gfxFont = NULL;
    tftVars->textColor = ILI9341_BLUE;
	tftVars->textBGColor = ILI9341_BLACK;
    tftVars->textSize = 4;
	tftVars->cp437 = false;
	tftVars->wrap = true;
	tftVars->cursor_x = 0;
	tftVars->cursor_y = 0;
	
	write('H', tftVars);
	write('E', tftVars);
	write('L', tftVars);
	write('L', tftVars);
	write('O', tftVars);
	write(' ', tftVars);
	write('W', tftVars);
	write('O', tftVars);
	write('R', tftVars);
	write('L', tftVars);
	write('D', tftVars);
	write('\n', tftVars);
	
	tftVars->textSize = 2;

	write('C', tftVars);
	write('O', tftVars);
	write('N', tftVars);
	write('G', tftVars);
	write('R', tftVars);
	write('A', tftVars);
	write('T', tftVars);
	write('U', tftVars);
	write('L', tftVars);
	write('A', tftVars);
	write('T', tftVars);
	write('I', tftVars);
	write('O', tftVars);
	write('N', tftVars);
	write('S', tftVars);
	write('\n', tftVars);
	
	tftVars->textSize = 1;
	write('T', tftVars);
	write('h', tftVars);
	write('i', tftVars);
	write('s', tftVars);
	write(' ', tftVars);
	write('T', tftVars);
	write('F', tftVars);
	write('T', tftVars);
	write(' ', tftVars);
	write('W', tftVars);
	write('o', tftVars);
	write('r', tftVars);
	write('k', tftVars);
	write('s', tftVars);
}
