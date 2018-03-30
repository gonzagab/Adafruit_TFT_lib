/********************************************************************
 * File:        gfx_font.h
 * Author:      Bryant Gonzaga
 * Created:     11/30/2017
 * Modified:    3/29/2018
 *
 * Notes:
 *  This was originally provided by Adafruit.
 *
 * Description:
 *  Font Structure for font info.
 *******************************************************************/

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
