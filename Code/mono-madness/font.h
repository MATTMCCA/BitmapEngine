#pragma once

#include <vector>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ft2build.h>
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_TRUETYPE_DRIVER_H

typedef struct {               // Data stored PER GLYPH
	uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width, height;    // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;


typedef struct {           // Data stored for FONT AS A WHOLE:
	uint8_t* bitmap;       // Glyph bitmaps, concatenated
	GFXglyph* glyph;       // Glyph array
	uint8_t   first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;


class font
{

public:

	font();

	int create(const char* fontName, int point, int dpi);
	GFXfont* getGFXfont(void);

	~font();

private:

	void enbit(uint8_t value, std::vector<uint8_t> *bmp);

protected:

	GFXfont* myFont = nullptr;

};

