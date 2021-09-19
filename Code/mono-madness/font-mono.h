#pragma once

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <ft2build.h>
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_TRUETYPE_DRIVER_H

#define _CRT_SECURE_NO_DEPRECATE

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


typedef struct {
	uint8_t* ptr;
	int _size;
} DFB;

typedef struct {
	const char* font;
	int pointSize;
	int dpi;
} font_att;

int grow_DFB(DFB* ptr, int concat);
int free_DFB(DFB* ptr);

int free_font(GFXfont* font);
int enbit(uint8_t value, DFB* ptr);
//if this function return with an error, mem never gets freed
int create_font(GFXfont* font, const char* fontName, int fontPoint, int dpi);
