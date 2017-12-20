/*
 * AdafruitTFTPaint.c
 *
 * Created: 12/15/2017 2:28:02 PM
 * Author : gonza
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "AdafruitTFTI2CDriver/AdafruitTFTI2CDriver.h"
#include "AdafruitTFTSPIDriver/AdafruitTFTSPIDriver.h"

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3

int oldcolor, currentcolor;
//INITIALIZE VARIABLES FOR TFT
TFTVars tftVars;
TSVars tsVars;

int main(void)
{
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
    initCPTTS(FT6206_DEFAULT_THRESSHOLD);
      
    fillScreen(ILI9341_BLACK, &tftVars);
      
    // make the color selection boxes
    fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED, &tftVars);
    fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW, &tftVars);
    fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN, &tftVars);
    fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN, &tftVars);
    fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE, &tftVars);
    fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA, &tftVars);
      
    // select the current color 'red'
    drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
    currentcolor = ILI9341_RED;
    //SET UP INTERRUPT
    DDRD &= 0xFB;
    EICRA |= 0x02;
    EIMSK |= 0x01;
    sei();
    
    while (1) {
        // Wait for a touch
    }
}

ISR(INT0_vect)
{
    // Retrieve a point
    TS_Point p = getPoint(&tsVars);

    // flip it around to match the screen.
    p.x = 240 - p.x;
    p.y = 320 - p.y;

    if (p.y < BOXSIZE) {
        oldcolor = currentcolor;

        if (p.x < BOXSIZE) {
            currentcolor = ILI9341_RED;
            drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*2) {
            currentcolor = ILI9341_YELLOW;
            drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*3) {
            currentcolor = ILI9341_GREEN;
            drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*4) {
            currentcolor = ILI9341_CYAN;
            drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*5) {
            currentcolor = ILI9341_BLUE;
            drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x <= BOXSIZE*6) {
            currentcolor = ILI9341_MAGENTA;
            drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
        }

        if (oldcolor != currentcolor) {
            if (oldcolor == ILI9341_RED)
            fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED, &tftVars);
            if (oldcolor == ILI9341_YELLOW)
            fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW, &tftVars);
            if (oldcolor == ILI9341_GREEN)
            fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN, &tftVars);
            if (oldcolor == ILI9341_CYAN)
            fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN, &tftVars);
            if (oldcolor == ILI9341_BLUE)
            fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE, &tftVars);
            if (oldcolor == ILI9341_MAGENTA)
            fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA, &tftVars);
        }
    }
         
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tftVars.height)) {
        fillCircle(p.x, p.y, PENRADIUS, currentcolor, &tftVars);
    }
}