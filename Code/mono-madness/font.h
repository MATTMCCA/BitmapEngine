// ! ref !
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/fontconvert.c
//
//MJM 2021

#pragma once

#include <vector>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ft2build.h>
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_TRUETYPE_DRIVER_H

#include "canvas.h"

typedef struct {               // Data stored PER GLYPH
	uint32_t  bitmapOffset;     // Pointer into GFXfont->bitmap
	uint32_t  width, height;    // Bitmap dimensions in pixels
	uint32_t  xAdvance;         // Distance to advance cursor (x axis)
	int32_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;


typedef struct {           // Data stored for FONT AS A WHOLE:
	uint8_t*   bitmap;       // Glyph bitmaps, concatenated
	GFXglyph*  glyph;       // Glyph array
	uint32_t   first, last; // ASCII extents
	uint32_t   yAdvance;    // Newline distance (y axis)
} GFXfont;


class font
{

public:

	font();

	int create(const char* fontName, int point, int dpi);
	int changeCharOffset(int32_t x, int32_t y);
	GFXfont* getGFXfont(void);

	int writeCanvas(canvas* ptr, const char* str);

	int charBounds(unsigned char c, int32_t* x, int32_t* y,
		int32_t* minx, int32_t* miny, int32_t* maxx,
		int32_t* maxy);

	int getTextBounds(const char* str, int32_t x, int32_t y,
		int32_t* x1, int32_t* y1, uint32_t* w,
		uint32_t* h);

	int drawChar(canvas* ptr, unsigned char c, int32_t x0, int32_t y0);
	int write(canvas* ptr, unsigned char c, int32_t* cursor_x, int32_t* cursor_y);

	int print(canvas* ptr, const char* str, int32_t x0, int32_t y0);

	~font();

private:

	void enbit(uint8_t value, std::vector<uint8_t> *bmp);

protected:

	GFXfont* myFont = nullptr;

};

