// ! ref !
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/fontconvert.c
//
//MJM 2021

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstdbool>
#include <cstring>
#include <cmath>

// a=target variable, b=bit number to act upon 0-n 
//https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
#define BIT_SET(a,b)				((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b)				((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b)				((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b)				(!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

#define BITMASK_SET(x, mask)		((x) |= (mask))
#define BITMASK_CLEAR(x, mask)		((x) &= (~(mask)))
#define BITMASK_FLIP(x, mask)		((x) ^= (mask))
#define BITMASK_CHECK_ALL(x, mask)	(!(~(x) & (mask)))
#define BITMASK_CHECK_ANY(x, mask)	((x) & (mask))


//ref: http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
typedef struct {
	uint16_t Signature;
	uint32_t FileSize;
	uint32_t reserved;
	uint32_t DataOffset;

} header;

typedef struct {
	uint32_t Size;
	uint32_t Width;
	uint32_t Height;
	uint16_t Planes;
	uint16_t Bits_Per_Pixel;
	uint32_t Compression;
	uint32_t ImageSize;
	uint32_t XpixelsPerM;
	uint32_t YpixelsPerM;
	uint32_t Colors_Used;
	uint32_t Important_Colors;

} infoHeader;

static const uint32_t COLOR_TABLE[2] = { 0x00000000, 0x00FFFFFF };


class canvas
{
public:
	canvas();

	int create(int32_t x, int32_t y, bool invert);
	int getSize(int32_t* x, int32_t* y);
	int32_t get_y(void);
	int32_t get_x(void);
	int fill(bool val);
	int addSprite(canvas* src, int32_t x, int32_t y, bool alpha);
	int save(const char* fileName, int DPI);
	int invert(bool invert);
	bool getInvert(void);
	bool getPixle(int32_t x, int32_t y);
	int setPixle(int32_t x, int32_t y, bool val);

	int drawHorizontalLine(int32_t x0, int32_t y0, int32_t width, int32_t thick, bool val);
	int drawVerticalLine(int32_t x0, int32_t y0, int32_t length, int32_t thick, bool val);
	int drawBox(int32_t x0, int32_t y0, int32_t length, int32_t width, int32_t thick, bool val);
	int drawBoxFill(int32_t x0, int32_t y0, int32_t length, int32_t width, bool val);

	int import_24bit(const char* fileName);

	~canvas();

private:


protected:
	bool _inv = 0;
	bool* ptr = nullptr;
	int32_t _x = 0, _y = 0;

};

