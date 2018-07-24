/********************************************************************
 * File:        adafruit_tft_spi_driver.c
 * Author:      Bryant Gonzaga
 * Created:     11/27/2017
 * Modified:    3/29/2018
 *
 * Notes:
 *  This was originally provided my Adafruit. This is the modified
 * version that is expected to run on an MCU. The SPI Firmware Driver
 * used in this version only has support for the ATmega324 and
 * ATmega128.
 *
 * Description:
 *  Contains methods to communicate with Adafruit's "2.8" TFT LCD
 * with Capacitive Touch Breakout Board w/MicroSD Socket". All
 * communication through these functions are done with SPI. This file
 * has functions for users to draw lines and shapes onto the screen.
 * It also contains functions for writing text to the screen.
 * 
 * How To:
 *  1: Users should first initialize a tft_vars structure.
 *  2: Then users can call the init_tft() function, passing it a
 *     pointer to the tft_vars struct created in 1.
 *  3: Finally users can call the functions for drawing onto the
 *     screen.
 *******************************************************************/

#include "adafruit_tft_spi_driver.h"

//===================================================================
//                         STATIC FUNCTIONS
//===================================================================
/**
 * Sends a command to the touchscreen through SPI.
 * @param cmd   8-bit command
 * @param var   pointer to TFTVars structure.
 */
static void write_command_tft(uint8_t cmd, tft_vars* var)
{
    //dc low to indicate command
    *(var->dc->PORTx) &= ~(var->dc->mask);
    //send command over
    spi_master_transmit8(cmd);
    //reset dc to its default value of 1 for data transfer
    *(var->dc->PORTx) |= var->dc->mask;
}

static void set_addr_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h, tft_vars* var)
{
    uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
    uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

    spi_select_slave(var->cs);
    write_command_tft(ILI9341_CASET, var); // Column addr set
    spi_master_transmit32(xa);
    write_command_tft(ILI9341_PASET, var); // Row addr set
    spi_master_transmit32(ya);
    write_command_tft(ILI9341_RAMWR, var); // write to RAM
    spi_deselect_slave(var->cs);
}

//=================================================================//
//                         PUBLIC FUNCTIONS                        //
//=================================================================//
void init_tft(tft_vars* var)
{
    /* Set up Data Direction Registers */
    *(var->dc->DDRx) |= var->dc->mask;
    *(var->rst->DDRx) |= var->rst->mask;

    /* Toggle RST */
    *(var->rst->PORTx) |= var->rst->mask;   // set rst
    DELAY_MS(100);
    *(var->rst->PORTx) &= ~(var->rst->mask);// clear rst
    DELAY_MS(100);
    *(var->rst->PORTx) |= var->rst->mask;   // set rst
    DELAY_MS(100);

    /* Initialize SPI */
    spi_master_init(var->cs, BG_SPI_SCLK_DIV_2 | BG_SPI_SAMPLE_RISING);

    /* Initialize the touchscreen */
    spi_select_slave(var->cs);
    write_command_tft(0xEF, var);
    spi_master_transmit8(0x03);
    spi_master_transmit8(0x80);
    spi_master_transmit8(0x02);

    write_command_tft(0xCF, var);
    spi_master_transmit8(0x00);
    spi_master_transmit8(0XC1);
    spi_master_transmit8(0X30);

    write_command_tft(0xED, var);
    spi_master_transmit8(0x64);
    spi_master_transmit8(0x03);
    spi_master_transmit8(0X12);
    spi_master_transmit8(0X81);

    write_command_tft(0xE8, var);
    spi_master_transmit8(0x85);
    spi_master_transmit8(0x00);
    spi_master_transmit8(0x78);

    write_command_tft(0xCB, var);
    spi_master_transmit8(0x39);
    spi_master_transmit8(0x2C);
    spi_master_transmit8(0x00);
    spi_master_transmit8(0x34);
    spi_master_transmit8(0x02);

    write_command_tft(0xF7, var);
    spi_master_transmit8(0x20);

    write_command_tft(0xEA, var);
    spi_master_transmit8(0x00);
    spi_master_transmit8(0x00);

    write_command_tft(ILI9341_PWCTR1, var);    //Power control
    spi_master_transmit8(0x23);   //VRH[5:0]

    write_command_tft(ILI9341_PWCTR2, var);    //Power control
    spi_master_transmit8(0x10);   //SAP[2:0];BT[3:0]

    write_command_tft(ILI9341_VMCTR1, var);    //VCM control
    spi_master_transmit8(0x3e);
    spi_master_transmit8(0x28);

    write_command_tft(ILI9341_VMCTR2, var);    //VCM control2
    spi_master_transmit8(0x86);  //--

    write_command_tft(ILI9341_MADCTL, var);    // Memory Access Control
    spi_master_transmit8(0x48);

    write_command_tft(ILI9341_VSCRSADD, var); // Vertical scroll
    spi_master_transmit16(0);                 // Zero

    write_command_tft(ILI9341_PIXFMT, var);
    spi_master_transmit8(0x55);

    write_command_tft(ILI9341_FRMCTR1, var);
    spi_master_transmit8(0x00);
    spi_master_transmit8(0x18);

    write_command_tft(ILI9341_DFUNCTR, var);    // Display Function Control
    spi_master_transmit8(0x08);
    spi_master_transmit8(0x82);
    spi_master_transmit8(0x27);

    write_command_tft(0xF2, var);    // 3Gamma Function Disable
    spi_master_transmit8(0x00);

    write_command_tft(ILI9341_GAMMASET, var);    //Gamma curve selected
    spi_master_transmit8(0x01);

    write_command_tft(ILI9341_GMCTRP1, var);    //Set Gamma
    spi_master_transmit8(0x0F);
    spi_master_transmit8(0x31);
    spi_master_transmit8(0x2B);
    spi_master_transmit8(0x0C);
    spi_master_transmit8(0x0E);
    spi_master_transmit8(0x08);
    spi_master_transmit8(0x4E);
    spi_master_transmit8(0xF1);
    spi_master_transmit8(0x37);
    spi_master_transmit8(0x07);
    spi_master_transmit8(0x10);
    spi_master_transmit8(0x03);
    spi_master_transmit8(0x0E);
    spi_master_transmit8(0x09);
    spi_master_transmit8(0x00);

    write_command_tft(ILI9341_GMCTRN1, var);    //Set Gamma
    spi_master_transmit8(0x00);
    spi_master_transmit8(0x0E);
    spi_master_transmit8(0x14);
    spi_master_transmit8(0x03);
    spi_master_transmit8(0x11);
    spi_master_transmit8(0x07);
    spi_master_transmit8(0x31);
    spi_master_transmit8(0xC1);
    spi_master_transmit8(0x48);
    spi_master_transmit8(0x08);
    spi_master_transmit8(0x0F);
    spi_master_transmit8(0x0C);
    spi_master_transmit8(0x31);
    spi_master_transmit8(0x36);
    spi_master_transmit8(0x0F);

    write_command_tft(ILI9341_SLPOUT, var);    //Exit Sleep
    DELAY_MS(120);
    write_command_tft(ILI9341_DISPON, var);    //Display on
    DELAY_MS(120);

    spi_deselect_slave(var->cs);

    var->width  = TFT_WIDTH;
    var->height = TFT_HEIGHT;
}

void init_spi_tft(tft_vars* var)
{
    spi_master_init(var->cs, BG_SPI_SCLK_DIV_2 | BG_SPI_SAMPLE_RISING);
}

void set_rotation_tft(uint8_t m, tft_vars* var)
{
    var->rotation = m % 4; // can't be higher than 3
    switch (var->rotation) {
    case 0:
        m = (MADCTL_MX | MADCTL_BGR);
        var->width  = TFT_WIDTH;
        var->height = TFT_HEIGHT;
        break;
    case 1:
        m = (MADCTL_MV | MADCTL_BGR);
        var->width  = TFT_HEIGHT;
        var->height = TFT_WIDTH;
        break;
    case 2:
        m = (MADCTL_MY | MADCTL_BGR);
        var->width  = TFT_WIDTH;
        var->height = TFT_HEIGHT;
        break;
    case 3:
        m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
        var->width  = TFT_HEIGHT;
        var->height = TFT_WIDTH;
        break;
    }

    spi_select_slave(var->cs);
    write_command_tft(ILI9341_MADCTL, var);
    spi_master_transmit8(m);
    spi_deselect_slave(var->cs);
}

void invertDisplay(bool i, tft_vars* var)
{
    spi_select_slave(var->cs);
    write_command_tft(i ? ILI9341_INVON : ILI9341_INVOFF, var);
    spi_deselect_slave(var->cs);
}

void scrollTo(uint16_t y, tft_vars* var)
{
    spi_select_slave(var->cs);
    write_command_tft(ILI9341_VSCRSADD, var);
    spi_master_transmit16(y);
    spi_deselect_slave(var->cs);
}

void drawPixel(int16_t x, int16_t y, uint16_t color, tft_vars* var)
{
    if (x < 0) {
        return;
    }
    if (x >= var->width) {
        return;
    }
    if (y < 0) {
        return;
    }
    if (y >= var->height) {
        return;
    }

    set_addr_window(x, y, 1, 1, var);
    spi_select_slave(var->cs);
    spi_master_transmit16(color);
    spi_deselect_slave(var->cs);
}

void writePixels(uint16_t* colors, uint32_t len, tft_vars* var)
{
    len = len * 2;

    spi_select_slave(var->cs);
    for (uint32_t i = 0; i < len; i += 2) {
        spi_master_transmit8(((uint8_t*)colors)[i + 1]);
        spi_master_transmit8(((uint8_t*)colors)[i]);
    }
    spi_deselect_slave(var->cs);
}

void writeColor(uint16_t color, uint32_t len, tft_vars* var)
{
    uint8_t hi = color >> 8, lo = color;

    spi_select_slave(var->cs);
    for (uint32_t t = len; t; t--) {
        spi_master_transmit8(hi);
        spi_master_transmit8(lo);
    }
    spi_deselect_slave(var->cs);
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

void fill_screen_tft(uint16_t color, tft_vars* var)
{
    fill_rect_tft(0, 0, var->width, var->height, color, var);
}

void draw_v_line_tft(int16_t x, int16_t y, int16_t h, uint16_t color, tft_vars* var)
{
    fill_rect_tft(x, y, 1, h, color, var);
}

void draw_h_line_tft(int16_t x, int16_t y, int16_t w, uint16_t color, tft_vars* var)
{
    fill_rect_tft(x, y, w, 1, color, var);
}

void draw_line_tft(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, tft_vars* var)
{
    if (x0 == x1) {
        if (y0 > y1) {
            _swap_int16_t(y0, y1);
        }
        draw_v_line_tft(x0, y0, y1 - y0 + 1, color, var);
    } else if (y0 == y1) {
        if (x0 > x1) {
            _swap_int16_t(x0, x1);
        }
        draw_h_line_tft(x0, y0, x1 - x0 + 1, color, var);
    } else {
        // Bresenham's algorithm - thanks Wikipedia
        int16_t steep = abs(y1 - y0) > abs(x1 - x0);
        if (steep) {
            _swap_int16_t(x0, y0);
            _swap_int16_t(x1, y1);
        }

        if (x0 > x1) {
            _swap_int16_t(x0, x1);
            _swap_int16_t(y0, y1);
        }

        int16_t dx, dy;
        dx = x1 - x0;
        dy = abs(y1 - y0);

        int16_t err = dx / 2;
        int16_t ystep;

        if (y0 < y1) {
            ystep = 1;
        } else {
            ystep = -1;
        }

        for (; x0<=x1; x0++) {
            if (steep) {
                drawPixel(y0, x0, color, var);
            } else {
                drawPixel(x0, y0, color, var);
            }
            err -= dy;
            if (err < 0) {
                y0 += ystep;
                err += dx;
            }
        }
    }
}

void fill_rect_tft(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, tft_vars* var)
{
    if ((x >= var->width) || (y >= var->height)) {
        return;
    }

    int16_t x2 = x + w - 1;
    int16_t y2 = y + h - 1;

    if ((x2 < 0) || (y2 < 0)) {
        return;
    }

    // Clip left/top
    if (x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if (y < 0) {
        y = 0;
        h = y2 + 1;
    }

    // Clip right/bottom
    if (x2 >= var->width) {
        w = var->width  - x;
    }
    if (y2 >= var->height) {
        h = var->height - y;
    }

    int32_t len = (int32_t)w * h;
    set_addr_window(x, y, w, h, var);
    writeColor(color, len, var);
}

void draw_rect_tft(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, tft_vars* var)
{
    draw_h_line_tft(x, y, w, color, var);
    draw_h_line_tft(x, (y + h - 1), w, color, var);
    draw_v_line_tft(x, y, h, color, var);
    draw_v_line_tft((x + w - 1), y, h, color, var);
}

void draw_circle_tft(int16_t x0, int16_t y0, int16_t r, uint16_t color, tft_vars* var)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0  , y0+r, color, var);
    drawPixel(x0  , y0-r, color, var);
    drawPixel(x0+r, y0  , color, var);
    drawPixel(x0-r, y0  , color, var);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color, var);
        drawPixel(x0 - x, y0 + y, color, var);
        drawPixel(x0 + x, y0 - y, color, var);
        drawPixel(x0 - x, y0 - y, color, var);
        drawPixel(x0 + y, y0 + x, color, var);
        drawPixel(x0 - y, y0 + x, color, var);
        drawPixel(x0 + y, y0 - x, color, var);
        drawPixel(x0 - y, y0 - x, color, var);
    }
}

void fill_circle_tft(int16_t x, int16_t y, int16_t r, uint16_t color, tft_vars* var)
{
    draw_v_line_tft(x, (y - r), ((2 * r) + 1), color, var);
    fillCircleHelper(x, y, r, 3, 0, color, var);
}

void draw_round_rect_tft(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, tft_vars* var)
{
    // smarter version
    draw_h_line_tft((x + r)    ,  y         , (w - (2 * r)), color, var); // Top
    draw_h_line_tft((x + r)    , (y + h - 1), (w - (2 * r)), color, var); // Bottom
    draw_v_line_tft( x         , (y + r)    , (h - (2 * r)), color, var); // Left
    draw_v_line_tft((x + w - 1), (y + r )   , (h - (2 * r)), color, var); // Right
    // draw four corners
    drawCircleHelper((x + r)        , (y + r)        , r, 1, color, var);
    drawCircleHelper((x + w - r - 1), (y + r)        , r, 2, color, var);
    drawCircleHelper((x + w - r - 1), (y + h - r - 1), r, 4, color, var);
    drawCircleHelper((x + r)        , (y + h - r - 1), r, 8, color, var);
}

void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color, tft_vars* var) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            drawPixel(x0 + x, y0 + y, color, var);
            drawPixel(x0 + y, y0 + x, color, var);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color, var);
            drawPixel(x0 + y, y0 - x, color, var);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color, var);
            drawPixel(x0 - x, y0 + y, color, var);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color, var);
            drawPixel(x0 - x, y0 - y, color, var);
        }
    }
}

void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, tft_vars* var)
{
    // smarter version
    fill_rect_tft((x + r), y, (w - (2 * r)), h, color, var);
    // draw four corners
    fillCircleHelper((x + w - r - 1), (y + r), r, 1, (h - (2 * r) - 1), color, var);
    fillCircleHelper((x + r)        , (y + r), r, 2, (h - (2 * r) - 1), color, var);
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color, tft_vars* var)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1) {
            draw_v_line_tft((x0 + x), (y0 - y), ((2 * y) + 1 + delta), color, var);
            draw_v_line_tft((x0 + y), (y0 - x), ((2 * x) + 1 + delta), color, var);
        }
        if (cornername & 0x2) {
            draw_v_line_tft((x0 - x), (y0 - y), ((2 * y) + 1 + delta), color, var);
            draw_v_line_tft((x0 - y), (y0 - x), ((2 * x) + 1 + delta), color, var);
        }
    }
}

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, tft_vars* var)
{
    draw_line_tft(x0, y0, x1, y1, color, var);
    draw_line_tft(x1, y1, x2, y2, color, var);
    draw_line_tft(x2, y2, x0, y0, color, var);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, tft_vars* var)
{
    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1);
        _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)      { a = x1; }
        else if (x1 > b) { b = x1; }
        if (x2 < a)      { a = x2; }
        else if (x2 > b) { b = x2; }
        draw_h_line_tft(a, y0, b-a+1, color, var);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scan line crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scan line y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scan line y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2) { last = y1; }  // Include y1 scan line
    else         { last = y1-1; } // Skip it

    for (y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) { _swap_int16_t(a,b); }
        draw_h_line_tft(a, y, b-a+1, color, var);
    }

    // For lower part of triangle, find scan line crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for (; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b) { _swap_int16_t(a,b); }
        draw_h_line_tft(a, y, b-a+1, color, var);
    }
}

void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size, tft_vars* var)
{
    if (!(var->gfx_font)) { // 'Classic' built-in font
        if ((x >= var->width)    || // Clip right
                (y >= var->height)       || // Clip bottom
                ((x + 6 * size - 1) < 0) || // Clip left
                ((y + 8 * size - 1) < 0)) { // Clip top
            return;
        }
        if (!var->cp437 && (c >= 176)) {
            c++; // Handle 'classic' char set behavior
        }

        for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
            uint8_t line = READ_FLASH_BYTE( &font[c * 5 + i] );
            for (int8_t j = 0; j < 8; j++, line >>= 1) {
                if (line & 1) {
                    if (size == 1) {
                        drawPixel(x+i, y+j, color, var);
                    } else {
                        fill_rect_tft(x+i*size, y+j*size, size, size, color, var);
                    }
                } else if (bg != color) {
                    if (size == 1) {
                        drawPixel(x+i, y+j, bg, var);
                    } else {
                        fill_rect_tft(x+i*size, y+j*size, size, size, bg, var);
                    }
                }
            }
        }
        if (bg != color) { // If opaque, draw vertical line for last column
            if (size == 1) {
                draw_v_line_tft(x+5, y, 8, bg, var);
            } else {
                fill_rect_tft(x+5*size, y, size, 8*size, bg, var);
            }
        }

    } else { // Custom font
        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= (uint8_t)READ_FLASH_BYTE(&(var->gfx_font->first));
        GFXglyph *glyph  = &(((GFXglyph *)READ_FLASH_PTR(&var->gfx_font->glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)READ_FLASH_PTR(&var->gfx_font->bitmap);

        uint16_t bo = READ_FLASH_WORD(&glyph->bitmapOffset);
        uint8_t  w  = READ_FLASH_BYTE(&glyph->width),
                 h  = READ_FLASH_BYTE(&glyph->height);
        int8_t   xo = READ_FLASH_BYTE(&glyph->xOffset),
                 yo = READ_FLASH_BYTE(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;

        if (size > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here

        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL unfortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        for (yy=0; yy<h; yy++) {
            for (xx=0; xx<w; xx++) {
                if (!(bit++ & 7)) {
                    bits = READ_FLASH_BYTE(&bitmap[bo++]);
                }
                if (bits & 0x80) {
                    if (size == 1) {
                        drawPixel(x+xo+xx, y+yo+yy, color, var);
                    } else {
                        fill_rect_tft(x+(xo16+xx)*size, y+(yo16+yy)*size,
                                 size, size, color, var);
                    }
                }
                bits <<= 1;
            }
        }

    } // End classic vs custom font
}

void write(uint8_t c, tft_vars* var)
{
    if (!(var->gfx_font)) { // 'Classic' built-in font
        if (c == '\n') {                       // Newline?
            var->cursor_x  = 0;                     // Reset x to zero,
            var->cursor_y += var->text_size * 8;          // advance y one line
        } else if (c != '\r') {                // Ignore carriage returns
            if (var->wrap && ((var->cursor_x + var->text_size * 6) > var->width)) { // Off right?
                var->cursor_x  = 0;                 // Reset x to zero,
                var->cursor_y += var->text_size * 8;      // advance y one line
            }
            drawChar(var->cursor_x, var->cursor_y, c, var->text_color, var->text_bg_color, var->text_size, var);
            var->cursor_x += var->text_size * 6;          // Advance x one char
        }
    } else { // Custom font
        if (c == '\n') {
            var->cursor_x  = 0;
            var->cursor_y += (int16_t)var->text_size *
            (uint8_t)READ_FLASH_BYTE(&var->gfx_font->yAdvance);
            } else if (c != '\r') {
            uint8_t first = READ_FLASH_BYTE(&var->gfx_font->first);
            if ((c >= first) && (c <= (uint8_t)READ_FLASH_BYTE(&var->gfx_font->last))) {
                GFXglyph *glyph = &(((GFXglyph*)READ_FLASH_PTR(
                &var->gfx_font->glyph))[c - first]);
                uint8_t w = READ_FLASH_BYTE(&glyph->width);
                uint8_t h = READ_FLASH_BYTE(&glyph->height);
                if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)READ_FLASH_BYTE(&glyph->xOffset); // sic
                    if (var->wrap && ((var->cursor_x + var->text_size * (xo + w)) > var->width)) {
                        var->cursor_x  = 0;
                        var->cursor_y += (int16_t)var->text_size *
                        (uint8_t)READ_FLASH_BYTE(&var->gfx_font->yAdvance);
                    }
                    drawChar(var->cursor_x, var->cursor_y, c, var->text_color, var->text_bg_color, var->text_size, var);
                }
                var->cursor_x += (uint8_t)READ_FLASH_BYTE(&glyph->xAdvance) * (int16_t)var->text_size;
            }
        }

    }

}

void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, tft_vars* var)
{

    if (var->gfx_font) {
        if (c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += var->text_size * (uint8_t)READ_FLASH_BYTE(&var->gfx_font->yAdvance);
        } else if (c != '\r') { // Not a carriage return; is normal char
            uint8_t first = READ_FLASH_BYTE(&var->gfx_font->first);
            uint8_t last  = READ_FLASH_BYTE(&var->gfx_font->last);
            if ((c >= first) && (c <= last)) { // Char present in this font?
                GFXglyph *glyph = &(((GFXglyph *)READ_FLASH_PTR(
                                         &var->gfx_font->glyph))[c - first]);
                uint8_t gw = READ_FLASH_BYTE(&glyph->width),
                        gh = READ_FLASH_BYTE(&glyph->height),
                        xa = READ_FLASH_BYTE(&glyph->xAdvance);
                int8_t  xo = READ_FLASH_BYTE(&glyph->xOffset),
                        yo = READ_FLASH_BYTE(&glyph->yOffset);
                if (var->wrap && ((*x+(((int16_t)xo+gw)*var->text_size)) > var->width)) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += var->text_size * (uint8_t)READ_FLASH_BYTE(&var->gfx_font->yAdvance);
                }
                int16_t ts = (int16_t)var->text_size,
                        x1 = *x + xo * ts,
                        y1 = *y + yo * ts,
                        x2 = x1 + gw * ts - 1,
                        y2 = y1 + gh * ts - 1;
                if (x1 < *minx) { *minx = x1; }
                if (y1 < *miny) { *miny = y1; }
                if (x2 > *maxx) { *maxx = x2; }
                if (y2 > *maxy) { *maxy = y2; }
                *x += xa * ts;
            }
        }

    } else { // Default font

        if (c == '\n') {                    // Newline?
            *x  = 0;                        // Reset x to zero,
            *y += var->text_size * 8;             // advance y one line
            // min/max x/y unchanged -- that waits for next 'normal' character
        } else if (c != '\r') { // Normal char; ignore carriage returns
            if (var->wrap && ((*x + var->text_size * 6) > var->width)) { // Off right?
                *x  = 0;                    // Reset x to zero,
                *y += var->text_size * 8;         // advance y one line
            }
            int x2 = *x + var->text_size * 6 - 1, // Lower-right pixel of char
                y2 = *y + var->text_size * 8 - 1;
            if (x2 > *maxx) { *maxx = x2; }     // Track max x, y
            if (y2 > *maxy) { *maxy = y2; }
            if (*x < *minx) { *minx = *x; }     // Track min x, y
            if (*y < *miny) { *miny = *y; }
            *x += var->text_size * 6;             // Advance x one char
        }
    }
}

void getTextBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h, tft_vars* var)
{
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = var->width, miny = var->height, maxx = -1, maxy = -1;

    while ((c = *str++)) {
        charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy, var);
    }

    if (maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if (maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}

void setFont(const GFXfont* f, tft_vars* var)
{
    if (f) {           // Font struct pointer passed in?
        if (!var->gfx_font) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            var->cursor_y += 6;
        }
        } else if (var->gfx_font) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        var->cursor_y -= 6;
    }
    var->gfx_font = (GFXfont*) f;
}

void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, tft_vars* var)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
    uint8_t byte = 0;

    for (int16_t j=0; j<h; j++, y++) {
        for (int16_t i=0; i<w; i++) {
            if (i & 7) {
                byte <<= 1;
                } else {
                byte = READ_FLASH_BYTE(&bitmap[((j * byteWidth) + i) / 8]);
            }
            if (byte & 0x80) {
                drawPixel(x+i, y, color, var);
            }
        }
    }
}

void drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg, tft_vars* var)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
    uint8_t byte = 0;

    for (int16_t j=0; j<h; j++, y++) {
        for (int16_t i=0; i<w; i++ ) {
            if (i & 7) {
                byte <<= 1;
                } else {
                byte = READ_FLASH_BYTE(&bitmap[((j * byteWidth) + i) / 8]);
            }
            drawPixel((x + i), y, (byte & 0x80) ? color : bg, var);
        }
    }
}
