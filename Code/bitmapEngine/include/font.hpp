/*
    ***************************************************************************************
    MJM -2021-
    Font Lib, Derived from Adafruit_GFX font converter & GFX Lib
    
    https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
    https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/fontconvert.c

    ======================================================================================= 
    
    File: font.hpp

        Notes: Uses Freetype, https://github.com/ShiftMediaProject/freetype2
        
    =======================================================================================

    Copyright <2021> <Matthew McCardle>
    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in the
    Software without restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the
    following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    ***************************************************************************************
    
    TrueType to Adafruit_GFX font converter.  Derived from Peter Jakobs'
    Adafruit_ftGFX fork & makefont tool, and Paul Kourany's Adafruit_mfGFX.
    NOT AN ARDUINO SKETCH.  This is a command-line tool for preprocessing
    fonts to be used with the Adafruit_GFX Arduino library.
    For UNIX-like systems.  Outputs to stdout; redirect to header file, e.g.:
      ./fontconvert ~/Library/Fonts/FreeSans.ttf 18 > FreeSans18pt7b.h
    REQUIRES FREETYPE LIBRARY.  www.freetype.org
    Currently this only extracts the printable 7-bit ASCII chars of a font.
    Will eventually extend with some int'l chars a la ftGFX, not there yet.
    Keep 7-bit fonts around as an option in that case, more compact.
    See notes at end for glyph nomenclature & other tidbits.

    ***************************************************************************************
    
    This is the core graphics library for all our displays, providing a common
    set of graphics primitives (points, lines, circles, etc.).  It needs to be
    paired with a hardware-specific library for each display device we carry
    (to handle the lower-level functions).
    Adafruit invests time and resources providing this open source code, please
    support Adafruit & open-source hardware by purchasing products from Adafruit!
    Copyright (c) 2013 Adafruit Industries.  All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    - Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    *************************************************************************************** 
*/

#pragma once

#include <vector>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ft2build.h>
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_TRUETYPE_DRIVER_H

#include "canvas.hpp"

typedef struct {                // Data stored PER GLYPH
    uint32_t  bitmapOffset;     // Pointer into GFXfont->bitmap
    uint32_t  width, height;    // Bitmap dimensions in pixels
    uint32_t  xAdvance;         // Distance to advance cursor (x axis)
    int32_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;


typedef struct {             // Data stored for FONT AS A WHOLE:
    uint8_t*   bitmap;       // Glyph bitmaps, concatenated
    GFXglyph*  glyph;        // Glyph array
    uint32_t   first, last;  // ASCII extents
    uint32_t   yAdvance;     // Newline distance (y axis)
} GFXfont;


class font
{

public:

    font();

    bool create(const char* fontName, int point, int dpi);
    bool changeCharOffset(int32_t x, int32_t y);
    bool writeCanvas(canvas* ptr, const char* str, int32_t x0 = 0, int32_t y0 = 0);
    
    GFXfont* getGFXfont(void);

    ~font();

private:

    bool drawChar(canvas* ptr, unsigned char c, int32_t x0, int32_t y0);
    bool write(canvas* ptr, unsigned char c, int32_t* cursor_x, int32_t* cursor_y, int32_t x_offset = 0);
    bool print(canvas* ptr, const char* str, int32_t x0, int32_t y0, int32_t x_offset = 0);
    bool charBounds(unsigned char c, int32_t* x, int32_t* y, int32_t* minx, int32_t* miny, int32_t* maxx, int32_t* maxy);
    bool getTextBounds(const char* str, int32_t x, int32_t y, int32_t* x1, int32_t* y1, uint32_t* w, uint32_t* h);
    void enbit(uint8_t value, std::vector<uint8_t> *bmp);

    void GFX_free(void)
    {
        if (myFont->bitmap != nullptr) {
            delete[] myFont->bitmap;
            myFont->bitmap = nullptr;
        }

        if (myFont->glyph != nullptr) {
            delete[] myFont->glyph;
            myFont->glyph = nullptr;
        }

        if (myFont != nullptr) {
            delete myFont;
            myFont = nullptr;
        }
    }

protected:

    GFXfont* myFont = nullptr;

};

