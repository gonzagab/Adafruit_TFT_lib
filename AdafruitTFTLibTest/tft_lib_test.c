/********************************************************************
 * File:        tft_lib_test.c
 * Author:      Bryant Gonzaga
 * Created:     12/3/2017 10:12:53 PM
 * Modified:    3/30/2018
 *
 * Description:
 *  This program tests out varies functions of the SPI TFT Library
 *******************************************************************/

#include "adafruit_tft_spi_driver.h"

/* Function Prototypes */
void testFillScreen(tft_vars* tftVars);
void testLines(uint16_t color, tft_vars* tftVars);
void testRects(uint16_t color, tft_vars* tftVars);
void testFastLines(uint16_t color1, uint16_t color2, tft_vars* tftVars);
void testFilledRects(uint16_t color1, uint16_t color2, tft_vars* tftVars);
void testCircles(uint8_t radius, uint16_t color, tft_vars* tftVars);
void testFilledCircles(uint8_t radius, uint16_t color, tft_vars* tftVars);
void testTriangles(tft_vars* tftVars);
void testFilledTriangles(tft_vars* tftVars);
void testRoundRects(tft_vars* tftVars);
void testFilledRoundRects(tft_vars* tftVars);
void testText(tft_vars* tftVars);

int main(void)
{
    //INITIALIZE VARIABLES FOR TFT
    tft_vars tftVars;
    //INITIALIZE PORTS FOR TFT
    pin_intrf rst = {
	    .DDRx = (uint8_t*) &PORTA_DIR,
		.PORTx = (uint8_t*) &PORTA_OUT,
		.PINx = (uint8_t*) &PORTA_IN,
		.mask = 0x04
	};
	
    tftVars.rst = &rst;
    
    pin_intrf dc;
    dc.DDRx = (uint8_t*) &PORTA_DIR;
    dc.PORTx = (uint8_t*) &PORTA_OUT;
    dc.PINx = (uint8_t*) &PORTA_IN;
    dc.mask = 0x08;
    tftVars.dc = &dc;
    
    pin_intrf cs;
    cs.DDRx = (uint8_t*) &PORTF_DIR;
    cs.PORTx = (uint8_t*) &PORTF_OUT;
    cs.PINx = (uint8_t*) &PORTF_IN;
    cs.mask = 0x08;
    tftVars.cs = &cs;

    //INITIALIZE TOUCH SCREEN
    init_tft(&tftVars);

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
			set_rotation_tft(rotation, &tftVars);
			testText(&tftVars);
			DELAY_MS( (uint32_t) 5000 );
		}
    }
}

void testFillScreen(tft_vars* tftVars)
{
    fill_screen_tft(ILI9341_BLACK       , tftVars);
    fill_screen_tft(ILI9341_NAVY        , tftVars);
    fill_screen_tft(ILI9341_DARKGREEN   , tftVars);
    fill_screen_tft(ILI9341_DARKCYAN    , tftVars);
    fill_screen_tft(ILI9341_MAROON      , tftVars);
    fill_screen_tft(ILI9341_PURPLE      , tftVars);
    fill_screen_tft(ILI9341_OLIVE       , tftVars);
    fill_screen_tft(ILI9341_LIGHTGREY   , tftVars);
    fill_screen_tft(ILI9341_DARKGREY    , tftVars);
    fill_screen_tft(ILI9341_BLUE        , tftVars);
    fill_screen_tft(ILI9341_GREEN       , tftVars);
    fill_screen_tft(ILI9341_CYAN        , tftVars);
    fill_screen_tft(ILI9341_RED         , tftVars);
    fill_screen_tft(ILI9341_MAGENTA     , tftVars);
    fill_screen_tft(ILI9341_YELLOW      , tftVars);
    fill_screen_tft(ILI9341_WHITE       , tftVars);
    fill_screen_tft(ILI9341_ORANGE      , tftVars);
    fill_screen_tft(ILI9341_GREENYELLOW , tftVars);
    fill_screen_tft(ILI9341_PINK        , tftVars);
}

void testLines(uint16_t color, tft_vars* tftVars)
{
    int x1;
    int y1;
    int x2;
    int y2;
    int w = tftVars->width;
    int h = tftVars->height;

    fill_screen_tft(ILI9341_BLACK, tftVars);
    x1 = y1 = 0;
    y2 = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }

    fill_screen_tft(ILI9341_BLACK, tftVars);
    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }
    x2 = 0;
    for (y2=0; y2<h; y2+=6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }

    fill_screen_tft(ILI9341_BLACK, tftVars);
    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    for (x2 = 0; x2 < w; x2 += 6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }

    fill_screen_tft(ILI9341_BLACK, tftVars);
    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;

    for (x2 = 0; x2 < w; x2 += 6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }

    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) {
        draw_line_tft(x1, y1, x2, y2, color, tftVars);
    }
}

void testRects(uint16_t color, tft_vars* tftVars)
{
	int           n, i, i2,
	cx = tftVars->width  / 2,
	cy = tftVars->height / 2;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	if (tftVars->width < tftVars->height) {
		n = tftVars->width;
	} else {
		n = tftVars->height;
	}

	for(i = 2; i < n; i += 6) {
		i2 = i / 2;
		draw_rect_tft(cx-i2, cy-i2, i, i, color, tftVars);
	}
}

void testFastLines(uint16_t color1, uint16_t color2, tft_vars* tftVars)
{
	int  x, y, w = tftVars->width, h = tftVars->height;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	
	for(y=0; y<h; y+=5) draw_h_line_tft(0, y, w, color1, tftVars);
	for(x=0; x<w; x+=5) draw_v_line_tft(x, 0, h, color2, tftVars);
}

void testFilledRects(uint16_t color1, uint16_t color2, tft_vars* tftVars)
{
	int n, i, i2,
	
	cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	
	if (tftVars->width < tftVars->height) {
		n = tftVars->width;
	} else {
		n = tftVars->height;
	}	
	
	for (i = n; i > 0; i -= 6) {
		i2    = i / 2;
			
		fill_rect_tft(cx-i2, cy-i2, i, i, color1, tftVars);
		draw_rect_tft(cx-i2, cy-i2, i, i, color2,tftVars);
	}
}

void testCircles(uint8_t radius, uint16_t color, tft_vars* tftVars)
{
	int x, y, r2 = radius * 2,
	w = tftVars->width + radius,
	h = tftVars->height + radius;
	
	fill_screen_tft(ILI9341_BLACK, tftVars);

	for (x = 0; x < w; x += r2) {
		for(y = 0; y < h; y += r2) {
			draw_circle_tft(x, y, radius, color, tftVars);
		}
	}
}

void testFilledCircles(uint8_t radius, uint16_t color, tft_vars* tftVars)
{
	int x, y, w = tftVars->width, h = tftVars->height, r2 = radius * 2;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	
	for(x=radius; x<w; x+=r2) {
		for(y=radius; y<h; y+=r2) {
			fill_circle_tft(x, y, radius, color, tftVars);
		}
	}
}

void testTriangles(tft_vars* tftVars)
{
	int n, i, cx = tftVars->width / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	
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

void testFilledTriangles(tft_vars* tftVars)
{
	int i, cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	
	if (cx < cy) {
		i = cx;
	} else {
		i = cy;
	}
	for (;i > 10; i -= 5) {
		fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		color565(0, i*10, i*10), tftVars);

		drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
		color565(i*10, i*10, 0), tftVars);
	}
}

void testRoundRects(tft_vars* tftVars)
{
	int w, i, i2,
	cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	
	if (tftVars->width < tftVars->height) {
		w = tftVars->width;
	} else {
		w = tftVars->height;
	}
	for(i=0; i<w; i+=6) {
		i2 = i / 2;
		draw_round_rect_tft(cx-i2, cy-i2, i, i, i/8, color565(i, 0, 0), tftVars);
	}
}

void testFilledRoundRects(tft_vars* tftVars)
{
	int i, i2,
	cx = tftVars->width  / 2 - 1,
	cy = tftVars->height / 2 - 1;

	fill_screen_tft(ILI9341_BLACK, tftVars);
	
	if (tftVars->width < tftVars->height) {
		i = tftVars->width;
	} else {
		i = tftVars->height;
	}
	for(;i>20; i-=6) {
		i2 = i / 2;
		fillRoundRect(cx-i2, cy-i2, i, i, i/8, color565(0, i, 0), tftVars);
	}
}

void testText(tft_vars* tftVars)
{
    fill_screen_tft(ILI9341_BLACK, tftVars);

	tftVars->gfx_font = NULL;
    tftVars->text_color = ILI9341_BLUE;
	tftVars->text_bg_color = ILI9341_BLACK;
    tftVars->text_size = 4;
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
	
	tftVars->text_size = 2;

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
	
	tftVars->text_size = 1;
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
