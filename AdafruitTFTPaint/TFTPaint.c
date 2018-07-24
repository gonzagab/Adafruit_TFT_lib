/*
 * AdafruitTFTPaint.c
 *
 * Created: 12/15/2017 2:28:02 PM
 * Author : Bryant Gonzaga
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "adafruit_tft_spi_driver.h"
#include "adafruit_tft_i2c_driver.h"

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3

int oldcolor, currentcolor;
//INITIALIZE VARIABLES FOR TFT
tft_vars tftVars;
ts_vars tsVars;

int main(void)
{
    //INITIALIZE PORTS FOR TFT
    avr_pin cs;
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x10;
    tftVars.cs = &cs;

    avr_pin dc;
    dc.DDRx = &DDRB;
    dc.PORTx = &PORTB;
    dc.PINx = &PINB;
    dc.mask = 0x08;
    tftVars.dc = &dc;

    avr_pin rst;
    rst.DDRx = &DDRB;
    rst.PORTx = &PORTB;
    rst.PINx = &PINB;
    rst.mask = 0x10;
    tftVars.rst = &rst;

    avr_pin sclk;
    sclk.DDRx = &DDRB;
    sclk.PORTx = &PORTB;
    sclk.PINx = &PINB;
    sclk.mask = 0x80;
    tftVars.sclk = &sclk;

    avr_pin mosi;
    mosi.DDRx = &DDRB;
    mosi.PORTx = &PORTB;
    mosi.PINx = &PINB;
    mosi.mask = 0x20;
    tftVars.mosi = &mosi;

    avr_pin miso;
    miso.DDRx = &DDRB;
    miso.PORTx = &PORTB;
    miso.PINx = &PINB;
    miso.mask = 0x40;
    tftVars.miso = &miso;

    //INITIALIZE TOUCH SCREEN
    init_tft(&tftVars);
    initCPTTS(FT6206_DEFAULT_THRESSHOLD);
      
    fill_screen_tft(ILI9341_BLACK, &tftVars);
      
    // make the color selection boxes
    fill_rect_tft(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED, &tftVars);
    fill_rect_tft(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW, &tftVars);
    fill_rect_tft(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN, &tftVars);
    fill_rect_tft(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN, &tftVars);
    fill_rect_tft(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE, &tftVars);
    fill_rect_tft(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA, &tftVars);
      
    // select the current color 'red'
    draw_rect_tft(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
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
    ts_point p = getPoint(&tsVars);

    // flip it around to match the screen.
    p.x = 240 - p.x;
    p.y = 320 - p.y;

    if (p.y < BOXSIZE) {
        oldcolor = currentcolor;

        if (p.x < BOXSIZE) {
            currentcolor = ILI9341_RED;
            draw_rect_tft(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*2) {
            currentcolor = ILI9341_YELLOW;
            draw_rect_tft(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*3) {
            currentcolor = ILI9341_GREEN;
            draw_rect_tft(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*4) {
            currentcolor = ILI9341_CYAN;
            draw_rect_tft(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x < BOXSIZE*5) {
            currentcolor = ILI9341_BLUE;
            draw_rect_tft(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
            } else if (p.x <= BOXSIZE*6) {
            currentcolor = ILI9341_MAGENTA;
            draw_rect_tft(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE, &tftVars);
        }

        if (oldcolor != currentcolor) {
            if (oldcolor == ILI9341_RED)
            fill_rect_tft(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED, &tftVars);
            if (oldcolor == ILI9341_YELLOW)
            fill_rect_tft(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW, &tftVars);
            if (oldcolor == ILI9341_GREEN)
            fill_rect_tft(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN, &tftVars);
            if (oldcolor == ILI9341_CYAN)
            fill_rect_tft(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN, &tftVars);
            if (oldcolor == ILI9341_BLUE)
            fill_rect_tft(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE, &tftVars);
            if (oldcolor == ILI9341_MAGENTA)
            fill_rect_tft(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA, &tftVars);
        }
    }
         
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tftVars.height)) {
        fill_circle_tft(p.x, p.y, PENRADIUS, currentcolor, &tftVars);
    }
}