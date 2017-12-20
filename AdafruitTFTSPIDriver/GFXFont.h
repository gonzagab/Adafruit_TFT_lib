//*******************************************************************
//* File Name       :  GFXFont.h
//*
//* Author          :   Bryant Gonzaga
//* Created         :   11/30/2017
//* Modified        :   12/20/2017
//* Target Device   :   ATmega324A
//* Description:
//*     Font structure for font info.
//*******************************************************************
#ifndef GFXFONT_H_
#define GFXFONT_H_

#include <stdint.h>

typedef struct
{ // Data stored PER GLYPH
    uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
    uint8_t  width, height;    // Bitmap dimensions in pixels
    uint8_t  xAdvance;         // Distance to advance cursor (x axis)
    int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct
{ // Data stored for FONT AS A WHOLE:
    uint8_t  *bitmap;      // Glyph bitmaps, concatenated
    GFXglyph *glyph;       // Glyph array
    uint8_t   first, last; // ASCII extents
    uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

#endif /* GFXFONT_H_ */
