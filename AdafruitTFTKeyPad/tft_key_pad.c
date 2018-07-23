/********************************************************************
 * File:        tft_key_pad.c
 * Author:      Bryant Gonzaga
 * Created:     12/19/2017 11:46:11 AM
 * Modified:    3/30/2018
 *
 * Description:
 *  Prints 16 buttons onto the TFT and when a button is pressed the
 * the label is printed out.
 *******************************************************************/

#include "adafruit_tft_spi_driver.h"
#include "adafruit_tft_i2c_driver.h"
#include "adafruit_tft_button.h"

//INITIALIZE VARIABLES FOR TFT
tft_vars tftVars;
ts_vars tsVars;

ts_button_vars buttons[16];

int main(void)
{
    //INITIALIZE PORTS FOR TFT
    avr_pin cs;
    cs.DDRx = (uint8_t*) &DDRB;
    cs.PORTx = (uint8_t*) &PORTB;
    cs.PINx = (uint8_t*) &PINB;
    cs.mask = 0x01;
    tftVars.cs = &cs;

    avr_pin sclk;
    sclk.DDRx = (uint8_t*) &DDRB;
    sclk.PORTx = (uint8_t*) &PORTB;
    sclk.PINx = (uint8_t*) &PINB;
    sclk.mask = 0x02;
    tftVars.sclk = &sclk;

    avr_pin mosi;
    mosi.DDRx = (uint8_t*) &DDRB;
    mosi.PORTx = (uint8_t*) &PORTB;
    mosi.PINx = (uint8_t*) &PINB;
    mosi.mask = 0x04;
    tftVars.mosi = &mosi;

    avr_pin miso;
    miso.DDRx = (uint8_t*) &DDRB;
    miso.PORTx = (uint8_t*) &PORTB;
    miso.PINx = (uint8_t*) &PINB;
    miso.mask = 0x08;
    tftVars.miso = &miso;
    
    avr_pin rst;
    rst.DDRx = (uint8_t*) &DDRB;
    rst.PORTx = (uint8_t*) &PORTB;
    rst.PINx = (uint8_t*) &PINB;
    rst.mask = 0x10;
    tftVars.rst = &rst;
    
    avr_pin dc;
    dc.DDRx = (uint8_t*) &DDRE;
    dc.PORTx = (uint8_t*) &PORTE;
    dc.PINx = (uint8_t*) &PINE;
    dc.mask = 0x01;
    tftVars.dc = &dc;

    //INITIALIZE TOUCH SCREEN
    init_tft(&tftVars);
    initCPTTS(FT6206_DEFAULT_THRESSHOLD);
    fillScreenTFT(ILI9341_BLACK, &tftVars);
    
    //INITIALIZE TEXT FORMATING
    tftVars.gfxFont = NULL;
    tftVars.textColor = ILI9341_BLUE;
    tftVars.textBGColor = ILI9341_BLACK;
    tftVars.text_size = 3;
    tftVars.cp437 = false;
    tftVars.wrap = true;
    tftVars.cursor_x = 0;
    tftVars.cursor_y = 0;
    
    /* Set Screen Orientation */
    setRotationTFT(1, &tftVars);
    
    /* Create Buttons */
    for (uint8_t i = 0; i < 16; i++) {
        buttons[i].fillColor = ILI9341_WHITE;
        buttons[i].outlineColor = ILI9341_GREEN;
        buttons[i].textColor = ILI9341_BLACK;
        if (i > 9) {
            buttons[i].label = i + 55;
        } else {
            buttons[i].label = i + 48;
        }
        if (i > 7) {
            buttons[i].y = 196;
        } else {
            buttons[i].y = 150;
        }
        buttons[i].x = (i % 8) * 40;
        buttons[i].size = 4;
        draw_button_tft(&buttons[i], &tftVars);
    }

    /* Init Interrrupts */
    DDRE &= 0xEF;
    EICRB = 0x02;
    EIMSK = 0x10;
    ENABLE_INTERRUPTS();

    while (1) {
    }
}


#if defined(__ICCAVR__)         // IAR C Compiler
#pragma vector = INT4_vect
__interrupt void  int4_handler(void)
#elif defined(__GNUC__)         // GNU Compiler
ISR(int_vect)
#endif
{
    // Retrieve a point
    ts_point p = getPoint(&tsVars);
    //flip to match rotation
	uint16_t x = 320 - p.y;
	p.y = p.x;
    
    for (uint8_t i = 0; i < 16; i++) {
        if (button_contains_point_tft(x, p.y, &buttons[i])) {
            write(buttons[i].label, &tftVars);
            break;
        }
    }
  
    DELAY_MS( (uint32_t) 100);
}
