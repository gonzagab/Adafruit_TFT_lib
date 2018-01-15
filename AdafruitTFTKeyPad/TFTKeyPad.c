//*******************************************************************
//* AdafruitTFTKeyPad.
//*
//* Created  :  12/19/2017 11:46:11 AM
//* Author   :  Bryant Gonzaga
//* Modified :  12/19/2017
//* Description:
//*     This is a test program that displays 
//*******************************************************************


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "AdafruitTFTI2CDriver/AdafruitTFTI2CDriver.h"
#include "AdafruitTFTSPIDriver/AdafruitTFTSPIDriver.h"
#include "AdafruitTFTSPIDriver/AdafruitTFTButton.h"

//INITIALIZE VARIABLES FOR TFT
TFTVars tftVars;
TSVars tsVars;
TSButtonVars button;

int main(void)
{
    //INITIALIZE PORTS FOR TFT
    AVRPin cs;
    cs.DDRx = &DDRB;
    cs.PORTx = &PORTB;
    cs.PINx = &PINB;
    cs.mask = 0x10;
    tftVars.cs = &cs;

    AVRPin dc;
    dc.DDRx = &DDRB;
    dc.PORTx = &PORTB;
    dc.PINx = &PINB;
    dc.mask = 0x08;
    tftVars.dc = &dc;

    AVRPin rst;
    rst.DDRx = &DDRB;
    rst.PORTx = &PORTB;
    rst.PINx = &PINB;
    rst.mask = 0x10;
    tftVars.rst = &rst;

    AVRPin sclk;
    sclk.DDRx = &DDRB;
    sclk.PORTx = &PORTB;
    sclk.PINx = &PINB;
    sclk.mask = 0x80;
    tftVars.sclk = &sclk;

    AVRPin mosi;
    mosi.DDRx = &DDRB;
    mosi.PORTx = &PORTB;
    mosi.PINx = &PINB;
    mosi.mask = 0x20;
    tftVars.mosi = &mosi;

    AVRPin miso;
    miso.DDRx = &DDRB;
    miso.PORTx = &PORTB;
    miso.PINx = &PINB;
    miso.mask = 0x40;
    tftVars.miso = &miso;

    //INITIALIZE TOUCH SCREEN
    initTFT(&tftVars);
    initCPTTS(FT6206_DEFAULT_THRESSHOLD);
    fillScreen(ILI9341_BLACK, &tftVars);
    
    //BUTTON 'T'
    button.fillColor = ILI9341_WHITE;
    button.outlineColor = ILI9341_GREEN;
    button.textColor = ILI9341_BLACK;
    button.label = 'T';
    button.x = 10;
    button.y = 10;
    button.w = 50;
    button.h = 50;
	
	setRotationTFT(1, &tftVars);
    drawButtonTFT(&button, &tftVars);
    
    //SET UP INTERRUPT
    DDRD &= 0xFB;
    EICRA |= 0x02;
    EIMSK |= 0x01;
    sei();

    while (1) 
    {
    }
}

ISR(INT0_vect)
{
    // Retrieve a point
    TS_Point p = getPoint(&tsVars);
    //flip to match rotation
	uint16_t x = 320 - p.y;
	p.y = p.x;
    if (buttonContainsPointTFT(x, p.y, &button)) {
        tftVars.gfxFont = NULL;
        tftVars.textColor = ILI9341_BLUE;
        tftVars.textBGColor = ILI9341_BLACK;
        tftVars.textSize = 4;
        tftVars.cp437 = false;
        tftVars.wrap = true;
        tftVars.cursor_x = 0;
        tftVars.cursor_y = 100;

        write('H', &tftVars);
        write('E', &tftVars);
        write('L', &tftVars);
        write('L', &tftVars);
        write('O', &tftVars);
        write(' ', &tftVars);
        write('W', &tftVars);
        write('O', &tftVars);
        write('R', &tftVars);
        write('L', &tftVars);
        write('D', &tftVars);
        write('\n', &tftVars);
    }
}
