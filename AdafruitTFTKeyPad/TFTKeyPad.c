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

TSButtonVars buttonA;
TSButtonVars buttonB;
TSButtonVars buttonC;
TSButtonVars buttonD;
TSButtonVars buttonE;

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
    fillScreenTFT(ILI9341_BLACK, &tftVars);
    
    //INITIALIZE TEXT FORMATING
    tftVars.gfxFont = NULL;
    tftVars.textColor = ILI9341_BLUE;
    tftVars.textBGColor = ILI9341_BLACK;
    tftVars.textSize = 3;
    tftVars.cp437 = false;
    tftVars.wrap = true;
    tftVars.cursor_x = 0;
    tftVars.cursor_y = 0;

    //BUTTON 'A'
    buttonA.fillColor = ILI9341_WHITE;
    buttonA.outlineColor = ILI9341_GREEN;
    buttonA.textColor = ILI9341_BLACK;
    buttonA.label = 'A';
    buttonA.x = 0;
    buttonA.y = 196;
    buttonA.size = 4;

    //BUTTON 'B'
    buttonB.fillColor = ILI9341_WHITE;
    buttonB.outlineColor = ILI9341_GREEN;
    buttonB.textColor = ILI9341_BLACK;
    buttonB.label = 'B';
    buttonB.x = 40;
    buttonB.y = 196;
    buttonB.size = 4;
    
    //BUTTON 'C'
    buttonC.fillColor = ILI9341_WHITE;
    buttonC.outlineColor = ILI9341_GREEN;
    buttonC.textColor = ILI9341_BLACK;
    buttonC.label = 'C';
    buttonC.x = 80;
    buttonC.y = 196;
    buttonC.size = 4;
    
    //BUTTON 'D'
    buttonD.fillColor = ILI9341_WHITE;
    buttonD.outlineColor = ILI9341_GREEN;
    buttonD.textColor = ILI9341_BLACK;
    buttonD.label = 'D';
    buttonD.x = 120;
    buttonD.y = 196;
    buttonD.size = 4;
    
    //BUTTON 'E'
    buttonE.fillColor = ILI9341_WHITE;
    buttonE.outlineColor = ILI9341_GREEN;
    buttonE.textColor = ILI9341_BLACK;
    buttonE.label = 'E';
    buttonE.x = 160;
    buttonE.y = 196;
    buttonE.size = 4;

	setRotationTFT(1, &tftVars);
    drawButtonTFT(&buttonA, &tftVars);
    drawButtonTFT(&buttonB, &tftVars);
    drawButtonTFT(&buttonC, &tftVars);
    drawButtonTFT(&buttonD, &tftVars);
    drawButtonTFT(&buttonE, &tftVars);

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
    if (buttonContainsPointTFT(x, p.y, &buttonA)) {
        write('A', &tftVars);
    } else if (buttonContainsPointTFT(x, p.y, &buttonB)) {
        write('B', &tftVars);
    } else if (buttonContainsPointTFT(x, p.y, &buttonC)) {
        write(buttonC.label, &tftVars);
    } else if (buttonContainsPointTFT(x, p.y, &buttonD)) {
        write(buttonD.label, &tftVars);
    } else if (buttonContainsPointTFT(x, p.y, &buttonE)) {
        write(buttonE.label, &tftVars);
    } else {
        //do nothing
    }
    
    _delay_ms(500);
}
