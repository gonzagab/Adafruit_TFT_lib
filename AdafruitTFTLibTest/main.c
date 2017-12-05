/*
 * AdafruitTFTLibTest.cpp
 *
 * Created: 12/3/2017 10:12:53 PM
 * Author : Bryant Gonzaga
 */

#include <avr/io.h>
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
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x08;
    tftVars.dc = &dc;

    PortRegisters rst;
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x10;
    tftVars.rst = &rst;

    PortRegisters sclk;
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x80;
    tftVars.sclk = &sclk;

    PortRegisters mosi;
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x20;
    tftVars.mosi = &mosi;

    PortRegisters miso;
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x40;
    tftVars.miso = &miso;

    //INITIALIZE TOUCH SCREEN
    initTFT(&tftVars);

    while (1) {
        testFillScreen(&tftVars);
    }
}

void testFillScreen(TFT_vars* tftVars)
{
    fillScreen(ILI9341_BLACK, tftVars);
    _delay_ms(1000);

    fillScreen(ILI9341_RED, tftVars);
    _delay_ms(1000);
    fillScreen(ILI9341_GREEN, tftVars);
    _delay_ms(1000);

    fillScreen(ILI9341_BLUE, tftVars);
    _delay_ms(1000);

    fillScreen(ILI9341_BLACK, tftVars);
    _delay_ms(1000);
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

void testText(TFT_vars* tftVars)
{
    fillScreen(ILI9341_BLACK, tftVars);

    setCursor(0, 0, tftVars);
    tftVars->textColor = ILI9341_WHITE;
    tftVars->textSize = 1;
    println("Hello World!");
    
    tftVars->textColor = ILI9341_YELLOW;
    tftVars->textSize = 2;
    println(1234.56);
    
    tftVars->textColor = ILI9341_RED;
    tftVars->textSize - 3;
    //println(0xDEADBEEF, HEX);
    
    println();
    
    tftVars->textColor = ILI9341_GREEN;
    tftVars->textSize = 5;
    println("Group");
    
    tftVars->textSize = 2;
    println("I implore thee,");
    
    tftVars->textSize = 1;
    println("my foonting turlingdromes.");
    println("And hooptiously drangle me");
    println("with crinkly bindlewurdles,");
    println("Or I will rend thee");
    println("in the gobberwarts");
    println("with my blurglecruncheon,");
    println("see if I don't!");
}
