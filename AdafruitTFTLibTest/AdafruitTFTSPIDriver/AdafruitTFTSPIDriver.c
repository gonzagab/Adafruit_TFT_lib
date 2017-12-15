//*****************************************************************
//* Author:         Bryant Gonzaga
//* Modified:       11/30/2017
//* Name:           AdafruitTFTSPIDriver.c
//* Description:
//*
//********************************************************************

#include "AdafruitTFTSPIDriver.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

void initTFT(TFT_vars* var)
{
    //DATA DIRECTION REGISTERS
    *(var->dc->DDRx) |= var->dc->mask;
    *(var->rst->DDRx) |= var->rst->mask;

    // toggle RST low to reset
    *(var->rst->PORTx) |= var->rst->mask;
    _delay_ms(100);
    *(var->rst->PORTx) &= ~(var->rst->mask);
    _delay_ms(100);
    *(var->rst->PORTx) |= var->rst->mask;
    _delay_ms(100);

    //INITIALIZE SPI
    spiMasterInit(var->cs, var->sclk, var->mosi, var->miso);

    spiStartTransmission(var->cs);
    writeCommand(0xEF, var);
    spiMasterTransmit(0x03);
    spiMasterTransmit(0x80);
    spiMasterTransmit(0x02);

    writeCommand(0xCF, var);
    spiMasterTransmit(0x00);
    spiMasterTransmit(0XC1);
    spiMasterTransmit(0X30);

    writeCommand(0xED, var);
    spiMasterTransmit(0x64);
    spiMasterTransmit(0x03);
    spiMasterTransmit(0X12);
    spiMasterTransmit(0X81);

    writeCommand(0xE8, var);
    spiMasterTransmit(0x85);
    spiMasterTransmit(0x00);
    spiMasterTransmit(0x78);

    writeCommand(0xCB, var);
    spiMasterTransmit(0x39);
    spiMasterTransmit(0x2C);
    spiMasterTransmit(0x00);
    spiMasterTransmit(0x34);
    spiMasterTransmit(0x02);

    writeCommand(0xF7, var);
    spiMasterTransmit(0x20);

    writeCommand(0xEA, var);
    spiMasterTransmit(0x00);
    spiMasterTransmit(0x00);

    writeCommand(ILI9341_PWCTR1, var);    //Power control
    spiMasterTransmit(0x23);   //VRH[5:0]

    writeCommand(ILI9341_PWCTR2, var);    //Power control
    spiMasterTransmit(0x10);   //SAP[2:0];BT[3:0]

    writeCommand(ILI9341_VMCTR1, var);    //VCM control
    spiMasterTransmit(0x3e);
    spiMasterTransmit(0x28);

    writeCommand(ILI9341_VMCTR2, var);    //VCM control2
    spiMasterTransmit(0x86);  //--

    writeCommand(ILI9341_MADCTL, var);    // Memory Access Control
    spiMasterTransmit(0x48);

    writeCommand(ILI9341_VSCRSADD, var); // Vertical scroll
    spiMasterTransmit16(0);                 // Zero

    writeCommand(ILI9341_PIXFMT, var);
    spiMasterTransmit(0x55);

    writeCommand(ILI9341_FRMCTR1, var);
    spiMasterTransmit(0x00);
    spiMasterTransmit(0x18);

    writeCommand(ILI9341_DFUNCTR, var);    // Display Function Control
    spiMasterTransmit(0x08);
    spiMasterTransmit(0x82);
    spiMasterTransmit(0x27);

    writeCommand(0xF2, var);    // 3Gamma Function Disable
    spiMasterTransmit(0x00);

    writeCommand(ILI9341_GAMMASET, var);    //Gamma curve selected
    spiMasterTransmit(0x01);

    writeCommand(ILI9341_GMCTRP1, var);    //Set Gamma
    spiMasterTransmit(0x0F);
    spiMasterTransmit(0x31);
    spiMasterTransmit(0x2B);
    spiMasterTransmit(0x0C);
    spiMasterTransmit(0x0E);
    spiMasterTransmit(0x08);
    spiMasterTransmit(0x4E);
    spiMasterTransmit(0xF1);
    spiMasterTransmit(0x37);
    spiMasterTransmit(0x07);
    spiMasterTransmit(0x10);
    spiMasterTransmit(0x03);
    spiMasterTransmit(0x0E);
    spiMasterTransmit(0x09);
    spiMasterTransmit(0x00);

    writeCommand(ILI9341_GMCTRN1, var);    //Set Gamma
    spiMasterTransmit(0x00);
    spiMasterTransmit(0x0E);
    spiMasterTransmit(0x14);
    spiMasterTransmit(0x03);
    spiMasterTransmit(0x11);
    spiMasterTransmit(0x07);
    spiMasterTransmit(0x31);
    spiMasterTransmit(0xC1);
    spiMasterTransmit(0x48);
    spiMasterTransmit(0x08);
    spiMasterTransmit(0x0F);
    spiMasterTransmit(0x0C);
    spiMasterTransmit(0x31);
    spiMasterTransmit(0x36);
    spiMasterTransmit(0x0F);

    writeCommand(ILI9341_SLPOUT, var);    //Exit Sleep
    _delay_ms(120);
    writeCommand(ILI9341_DISPON, var);    //Display on
    _delay_ms(120);

    spiEndTransmission(var->cs);
    
    var->width  = TFT_WIDTH;
    var->height = TFT_HEIGHT;
}

void writeCommand(uint8_t cmd, TFT_vars* var)
{
    //dc low to indicate command
    *(var->dc->PORTx) &= ~(var->dc->mask);
    //send command over
    spiMasterTransmit(cmd);
    //reset dc to its default value of 1 for data transfer
    *(var->dc->PORTx) |= var->dc->mask;
}

void setRotation(uint8_t m, TFT_vars* var)
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
    
    spiStartTransmission(var->cs);
    writeCommand(ILI9341_MADCTL, var);
    spiMasterTransmit(m);
    spiEndTransmission(var->cs);
}

void invertDisplay(bool i, TFT_vars* var)
{
    spiStartTransmission(var->cs);
    writeCommand(i ? ILI9341_INVON : ILI9341_INVOFF, var);
    spiEndTransmission(var->cs);
}

void scrollTo(uint16_t y, TFT_vars* var)
{
    spiStartTransmission(var->cs);
    writeCommand(ILI9341_VSCRSADD, var);
    spiMasterTransmit16(y);
    spiEndTransmission(var->cs);
}

void drawPixel(int16_t x, int16_t y, uint16_t color, TFT_vars* var)
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

    setAddrWindow(x, y, 1, 1, var);
    spiStartTransmission(var->cs);
    spiMasterTransmit16(color);
    spiEndTransmission(var->cs);
}

// Transaction API not used by GFX
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, TFT_vars* var)
{
    uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
    uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);
    
    spiStartTransmission(var->cs);
    writeCommand(ILI9341_CASET, var); // Column addr set
    spiMasterTransmit32(xa);
    writeCommand(ILI9341_PASET, var); // Row addr set
    spiMasterTransmit32(ya);
    writeCommand(ILI9341_RAMWR, var); // write to RAM
    spiEndTransmission(var->cs);
}

void writePixels(uint16_t* colors, uint32_t len, TFT_vars* var)
{
    len = len * 2;
    
    spiStartTransmission(var->cs);
    for (uint32_t i = 0; i < len; i += 2) {
        spiMasterTransmit(((uint8_t*)colors)[i + 1]);
        spiMasterTransmit(((uint8_t*)colors)[i]);
    }
    spiEndTransmission(var->cs);
}

void writeColor(uint16_t color, uint32_t len, TFT_vars* var)
{    
    uint8_t hi = color >> 8, lo = color;

    spiStartTransmission(var->cs);
    for (uint32_t t = len; t; t--) {
        spiMasterTransmit(hi);
        spiMasterTransmit(lo);
    }
    spiEndTransmission(var->cs);
}

// Recommended Non-Transaction
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color, TFT_vars* var)
{
    fillRect(x, y, 1, h, color, var);
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color, TFT_vars* var)
{
    fillRect(x, y, w, 1, color, var);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFT_vars* var)
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
    setAddrWindow(x, y, w, h, var);
    writeColor(color, len, var);
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, TFT_vars* var)
{
    // Update in subclasses if desired!
    if (x0 == x1) {
        if (y0 > y1) {
            _swap_int16_t(y0, y1);
        }
        drawFastVLine(x0, y0, y1 - y0 + 1, color, var);
    } else if (y0 == y1) {
        if (x0 > x1) {
            _swap_int16_t(x0, x1);
        }
        drawFastHLine(x0, y0, x1 - x0 + 1, color, var);
    } else {
        writeLine(x0, y0, x1, y1, color, var);
    }
}

void fillScreen(uint16_t color, TFT_vars* var)
{
    fillRect(0, 0, var->width, var->height, color, var);
}

void setCursor(int16_t x, int16_t y, TFT_vars* var)
{
    var->cursor_x = x;
    var->cursor_y = y;
}

// Draw a rectangle
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, TFT_vars* var)
{
    drawFastHLine(x, y, w, color, var);
    drawFastHLine(x, (y + h - 1), w, color, var);
    drawFastVLine(x, y, h, color, var);
    drawFastVLine((x + w - 1), y, h, color, var);
}

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, TFT_vars* var)
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

void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color, TFT_vars* var) {
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

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, TFT_vars* var)
{
    drawFastVLine(x0, (y0 - r), ((2 * r) + 1), color, var);
    fillCircleHelper(x0, y0, r, 3, 0, color, var);
}

// Used to do circles and round rectangles
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color, TFT_vars* var)
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
            drawFastVLine((x0 + x), (y0 - y), ((2 * y) + 1 + delta), color, var);
            drawFastVLine((x0 + y), (y0 - x), ((2 * x) + 1 + delta), color, var);
        }
        if (cornername & 0x2) {
            drawFastVLine((x0 - x), (y0 - y), ((2 * y) + 1 + delta), color, var);
            drawFastVLine((x0 - y), (y0 - x), ((2 * x) + 1 + delta), color, var);
        }
    }
}

// Draw a rounded rectangle
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, TFT_vars* var)
{
    // smarter version
    drawFastHLine((x + r)    ,  y         , (w - (2 * r)), color, var); // Top
    drawFastHLine((x + r)    , (y + h - 1), (w - (2 * r)), color, var); // Bottom
    drawFastVLine( x         , (y + r)    , (h - (2 * r)), color, var); // Left
    drawFastVLine((x + w - 1), (y + r )   , (h - (2 * r)), color, var); // Right
    // draw four corners
    drawCircleHelper((x + r)        , (y + r)        , r, 1, color, var);
    drawCircleHelper((x + w - r - 1), (y + r)        , r, 2, color, var);
    drawCircleHelper((x + w - r - 1), (y + h - r - 1), r, 4, color, var);
    drawCircleHelper((x + r)        , (y + h - r - 1), r, 8, color, var);
}

// Fill a rounded rectangle
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, TFT_vars* var)
{
    // smarter version
    fillRect((x + r), y, (w - (2 * r)), h, color, var);
    // draw four corners
    fillCircleHelper((x + w - r - 1), (y + r), r, 1, (h - (2 * r) - 1), color, var);
    fillCircleHelper((x + r)        , (y + r), r, 2, (h - (2 * r) - 1), color, var);
}

// Draw a triangle
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFT_vars* var)
{
    drawLine(x0, y0, x1, y1, color, var);
    drawLine(x1, y1, x2, y2, color, var);
    drawLine(x2, y2, x0, y0, color, var);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, TFT_vars* var)
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
        drawFastHLine(a, y0, b-a+1, color, var);
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
        drawFastHLine(a, y, b-a+1, color, var);
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
        drawFastHLine(a, y, b-a+1, color, var);
    }
}

// Bresenham's algorithm - thanks Wikipedia
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, TFT_vars* var)
{
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


// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

// Draw a PROGMEM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground color (unset bits are transparent).
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, TFT_vars* var)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
    uint8_t byte = 0;

    for (int16_t j=0; j<h; j++, y++) {
        for (int16_t i=0; i<w; i++) {
            if (i & 7) {
                byte <<= 1;
            } else {
                byte = pgm_read_byte(&bitmap[((j * byteWidth) + i) / 8]);
            }
            if (byte & 0x80) {
                drawPixel(x+i, y, color, var);
            }
        }
    }
}

// Draw a PROGMEM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground (for set bits) and background (unset
// bits) colors.
void drawBitmap1(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg, TFT_vars* var)
{
    int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
    uint8_t byte = 0;

    for (int16_t j=0; j<h; j++, y++) {
        for (int16_t i=0; i<w; i++ ) {
            if (i & 7) {
                byte <<= 1;
            } else {
                byte = pgm_read_byte(&bitmap[((j * byteWidth) + i) / 8]);
            }
            drawPixel((x + i), y, (byte & 0x80) ? color : bg, var);
        }
    }
}

//ADD ONE BY ONE
/*

// Draw a RAM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground color (unset bits are transparent).
void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
uint8_t byte = 0;

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
if (i & 7) {
byte <<= 1;
} else {
byte   = bitmap[j * byteWidth + i / 8];
}
if (byte & 0x80) {
writePixel(x+i, y, color);
}
}
}
}

// Draw a RAM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground (for set bits) and background (unset
// bits) colors.
void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
uint8_t byte = 0;


for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
if (i & 7) { byte <<= 1; }
else      { byte   = bitmap[j * byteWidth + i / 8]; }
writePixel(x+i, y, (byte & 0x80) ? color : bg);
}
}

}

// Draw PROGMEM-resident XBitMap Files (*.xbm), exported from GIMP,
// Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
// C Array can be directly used with this function.
// There is no RAM-resident version of this function; if generating bitmaps
// in RAM, use the format defined by drawBitmap() and call that instead.
void drawXBitmap(int16_t x, int16_t y,
const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {

int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
uint8_t byte = 0;


for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
if (i & 7) { byte >>= 1; }
else      { byte   = pgm_read_byte(&bitmap[j * byteWidth + i / 8]); }
// Nearly identical to drawBitmap(), only the bit order
// is reversed here (left-to-right = LSB to MSB):
if (byte & 0x01) { writePixel(x+i, y, color); }
}
}

}

// Draw a PROGMEM-resident 8-bit image (grayscale) at the specified (x,y)
// pos.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
void drawGrayscaleBitmap(int16_t x, int16_t y,
const uint8_t bitmap[], int16_t w, int16_t h) {

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
writePixel(x+i, y, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
}
}

}

// Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y)
// pos.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
void drawGrayscaleBitmap(int16_t x, int16_t y,
uint8_t *bitmap, int16_t w, int16_t h) {

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
writePixel(x+i, y, bitmap[j * w + i]);
}
}

}

// Draw a PROGMEM-resident 8-bit image (grayscale) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) position.
// BOTH buffers (grayscale and mask) must be PROGMEM-resident.
// Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
void drawGrayscaleBitmap(int16_t x, int16_t y,
const uint8_t bitmap[], const uint8_t mask[],
int16_t w, int16_t h) {
int16_t bw   = (w + 7) / 8; // Bitmask scan line pad = whole byte
uint8_t byte = 0;

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
if (i & 7) { byte <<= 1; }
else      { byte   = pgm_read_byte(&mask[j * bw + i / 8]); }
if (byte & 0x80) {
writePixel(x+i, y, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
}
}
}

}

// Draw a RAM-resident 8-bit image (grayscale) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) pos.
// BOTH buffers (grayscale and mask) must be RAM-resident, no mix-and-
// match.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
void drawGrayscaleBitmap(int16_t x, int16_t y,
uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h) {
int16_t bw   = (w + 7) / 8; // Bitmask scan line pad = whole byte
uint8_t byte = 0;

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
if (i & 7) { byte <<= 1; }
else      { byte   = mask[j * bw + i / 8]; }
if (byte & 0x80) {
writePixel(x+i, y, bitmap[j * w + i]);
}
}
}

}

// Draw a PROGMEM-resident 16-bit image (RGB 5/6/5) at the specified (x,y)
// position.  For 16-bit display devices; no color reduction performed.
void drawRGBBitmap(int16_t x, int16_t y,
const uint16_t bitmap[], int16_t w, int16_t h) {

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
writePixel(x+i, y, pgm_read_ptr(&bitmap[j * w + i]));
}
}

}

// Draw a RAM-resident 16-bit image (RGB 5/6/5) at the specified (x,y)
// position.  For 16-bit display devices; no color reduction performed.
void drawRGBBitmap(int16_t x, int16_t y,
uint16_t *bitmap, int16_t w, int16_t h) {

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
writePixel(x+i, y, bitmap[j * w + i]);
}
}

}

// Draw a PROGMEM-resident 16-bit image (RGB 5/6/5) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) position.
// BOTH buffers (color and mask) must be PROGMEM-resident.
// For 16-bit display devices; no color reduction performed.
void drawRGBBitmap(int16_t x, int16_t y,
const uint16_t bitmap[], const uint8_t mask[],
int16_t w, int16_t h) {
int16_t bw   = (w + 7) / 8; // Bitmask scan line pad = whole byte
uint8_t byte = 0;

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
if (i & 7) { byte <<= 1; }
else      { byte   = pgm_read_byte(&mask[j * bw + i / 8]); }
if (byte & 0x80) {
writePixel(x+i, y, pgm_read_word(&bitmap[j * w + i]));
}
}
}

}

// Draw a RAM-resident 16-bit image (RGB 5/6/5) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) pos.
// BOTH buffers (color and mask) must be RAM-resident, no mix-and-match.
// For 16-bit display devices; no color reduction performed.
void drawRGBBitmap(int16_t x, int16_t y,
uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h) {
int16_t bw   = (w + 7) / 8; // Bitmask scan line pad = whole byte
uint8_t byte = 0;

for (int16_t j=0; j<h; j++, y++) {
for (int16_t i=0; i<w; i++ ) {
if (i & 7) { byte <<= 1; }
else      { byte   = mask[j * bw + i / 8]; }
if (byte & 0x80) {
writePixel(x+i, y, bitmap[j * w + i]);
}
}
}

}
*/

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------	 
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size, TFT_vars* var)
{
    if (!(var->gfxFont)) { // 'Classic' built-in font
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
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for (int8_t j = 0; j < 8; j++, line >>= 1) {
                if (line & 1) {
                    if (size == 1) {
                        drawPixel(x+i, y+j, color, var);
                    } else {
                        fillRect(x+i*size, y+j*size, size, size, color, var);
                    }
                } else if (bg != color) {
                    if (size == 1) {
                        drawPixel(x+i, y+j, bg, var);
                    } else {
                        fillRect(x+i*size, y+j*size, size, size, bg, var);
                    }
                }
            }
        }
        if (bg != color) { // If opaque, draw vertical line for last column
            if (size == 1) {
                drawFastVLine(x+5, y, 8, bg, var);
            } else {
                fillRect(x+5*size, y, size, 8*size, bg, var);
            }
        }

    } else { // Custom font
        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= (uint8_t)pgm_read_byte(&(var->gfxFont->first));
        GFXglyph *glyph  = &(((GFXglyph *)pgm_read_ptr(&var->gfxFont->glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)pgm_read_ptr(&var->gfxFont->bitmap);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
                 h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
                 yo = pgm_read_byte(&glyph->yOffset);
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
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if (bits & 0x80) {
                    if (size == 1) {
                        drawPixel(x+xo+xx, y+yo+yy, color, var);
                    } else {
                        fillRect(x+(xo16+xx)*size, y+(yo16+yy)*size,
                                 size, size, color, var);
                    }
                }
                bits <<= 1;
            }
        }

    } // End classic vs custom font
}

// Enable (or disable) Code Page 437-compatible charset.
// There was an error in glcdfont.c for the longest time -- one character
// (#176, the 'light shade' block) was missing -- this threw off the index
// of every character that followed it.  But a TON of code has been written
// with the erroneous character indices.  By default, the library uses the
// original 'wrong' behavior and old sketches will still work.  Pass 'true'
// to this function to use correct CP437 character values in your code.
//void cp437(bool x, TFT_vars* var)
//{
    //var->cp437 = x;
//}

void setFont(const GFXfont* f, TFT_vars* var)
{
    if (f) {           // Font struct pointer passed in?
        if (!var->gfxFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            var->cursor_y += 6;
        }
    } else if (var->gfxFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        var->cursor_y -= 6;
    }
	var->gfxFont = (GFXfont*) f;
}

// Pass string and a cursor position, returns UL corner and W,H.
void getTextBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h, TFT_vars* var)
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
/*
// Same as above, but for PROGMEM strings
void getTextBounds(const __FlashStringHelper *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
    uint8_t *s = (uint8_t *)str, c;

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = var->width, miny = var->height, maxx = -1, maxy = -1;

    while ((c = pgm_read_byte(s++)))
    { charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy); }

    if (maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if (maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}
*/

void write(uint8_t c, TFT_vars* var)
{
    if (!(var->gfxFont)) { // 'Classic' built-in font
        if (c == '\n') {                       // Newline?
            var->cursor_x  = 0;                     // Reset x to zero,
            var->cursor_y += var->textSize * 8;          // advance y one line
        } else if (c != '\r') {                // Ignore carriage returns
            if (var->wrap && ((var->cursor_x + var->textSize * 6) > var->width)) { // Off right?
                var->cursor_x  = 0;                 // Reset x to zero,
                var->cursor_y += var->textSize * 8;      // advance y one line
            }
            drawChar(var->cursor_x, var->cursor_y, c, var->textColor, var->textBGColor, var->textSize, var);
            var->cursor_x += var->textSize * 6;          // Advance x one char
        }

    } else { // Custom font

        if (c == '\n') {
            var->cursor_x  = 0;
            var->cursor_y += (int16_t)var->textSize *
                        (uint8_t)pgm_read_byte(&var->gfxFont->yAdvance);
        } else if (c != '\r') {
            uint8_t first = pgm_read_byte(&var->gfxFont->first);
            if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&var->gfxFont->last))) {
                GFXglyph *glyph = &(((GFXglyph*)pgm_read_ptr(
                                         &var->gfxFont->glyph))[c - first]);
                uint8_t w = pgm_read_byte(&glyph->width);
                uint8_t h = pgm_read_byte(&glyph->height);
                if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if (var->wrap && ((var->cursor_x + var->textSize * (xo + w)) > var->width)) {
                        var->cursor_x  = 0;
                        var->cursor_y += (int16_t)var->textSize *
                                    (uint8_t)pgm_read_byte(&var->gfxFont->yAdvance);
                    }
                    drawChar(var->cursor_x, var->cursor_y, c, var->textColor, var->textBGColor, var->textSize, var);
                }
                var->cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)var->textSize;
            }
        }

    }

}

// Broke this out as it's used by both the PROGMEM- and RAM-resident
// getTextBounds() functions.
void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, TFT_vars* var)
{

    if (var->gfxFont) {
        if (c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += var->textSize * (uint8_t)pgm_read_byte(&var->gfxFont->yAdvance);
        } else if (c != '\r') { // Not a carriage return; is normal char
            uint8_t first = pgm_read_byte(&var->gfxFont->first);
            uint8_t last  = pgm_read_byte(&var->gfxFont->last);
            if ((c >= first) && (c <= last)) { // Char present in this font?
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_ptr(
                                         &var->gfxFont->glyph))[c - first]);
                uint8_t gw = pgm_read_byte(&glyph->width),
                        gh = pgm_read_byte(&glyph->height),
                        xa = pgm_read_byte(&glyph->xAdvance);
                int8_t  xo = pgm_read_byte(&glyph->xOffset),
                        yo = pgm_read_byte(&glyph->yOffset);
                if (var->wrap && ((*x+(((int16_t)xo+gw)*var->textSize)) > var->width)) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += var->textSize * (uint8_t)pgm_read_byte(&var->gfxFont->yAdvance);
                }
                int16_t ts = (int16_t)var->textSize,
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
            *y += var->textSize * 8;             // advance y one line
            // min/max x/y unchanged -- that waits for next 'normal' character
        } else if (c != '\r') { // Normal char; ignore carriage returns
            if (var->wrap && ((*x + var->textSize * 6) > var->width)) { // Off right?
                *x  = 0;                    // Reset x to zero,
                *y += var->textSize * 8;         // advance y one line
            }
            int x2 = *x + var->textSize * 6 - 1, // Lower-right pixel of char
                y2 = *y + var->textSize * 8 - 1;
            if (x2 > *maxx) { *maxx = x2; }     // Track max x, y
            if (y2 > *maxy) { *maxy = y2; }
            if (*x < *minx) { *minx = *x; }     // Track min x, y
            if (*y < *miny) { *miny = *y; }
            *x += var->textSize * 6;             // Advance x one char
        }
    }
}

//STUFF IDK WHAT TO DO WITH
/*
// Adapted from https://github.com/PaulStoffregen/ILI9341_t3
// by Marc MERLIN. See examples/pictureEmbed to use this.
// 5/6/2017: function name and arguments have changed for compatibility
// with current GFX library and to avoid naming problems in prior
// implementation.  Formerly drawBitmap() with arguments in different order.
void Adafruit_ILI9341::drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h)
{

int16_t x2, y2; // Lower-right coord
if(( x             >= var->width ) ||      // Off-edge right
( y             >= var->height) ||      // " top
((x2 = (x+w-1)) <  0      ) ||      // " left
((y2 = (y+h-1)) <  0)     ) return; // " bottom

int16_t bx1=0, by1=0, // Clipped top-left within bitmap
saveW=w;      // Save original bitmap width value
if(x < 0) { // Clip left
w  +=  x;
bx1 = -x;
x   =  0;
}
if(y < 0) { // Clip top
h  +=  y;
by1 = -y;
y   =  0;
}
if(x2 >= var->width ) w = var->width  - x; // Clip right
if(y2 >= var->height) h = var->height - y; // Clip bottom

pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left

setAddrWindow(x, y, w, h); // Clipped area
while(h--) { // For each (clipped) scan line...
writePixels(pcolors, w); // Push one (clipped) row
pcolors += saveW; // Advance pointer by one full (unclipped) line
}

}

uint8_t Adafruit_ILI9341::readcommand8(uint8_t c, uint8_t index)
{
uint32_t freq = _freq;
if(_freq > 24000000){
_freq = 24000000;
}

writeCommand(0xD9);  // woo sekret command?
SPIMasterTransmit(0x10 + index);
writeCommand(c);
uint8_t r = spiRead();

_freq = freq;
return r;
}



uint8_t Adafruit_ILI9341::spiRead()
{
return SPISlaveReceive();
}
*/
