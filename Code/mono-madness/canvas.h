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

typedef struct {
	uint8_t* __img;
	uint32_t __img_x;
	uint32_t __img_y;
} img;


enum class DITHER 
{ 
	Threshold, 
	FloydSteinberg,
	Stucki, 
	Jarvis, 
	Atkinson, 
	Bayer_2x2, 
	Bayer_4x4, 
	Bayer_8x8, 
	Bayer_16x16,
	Cluster
};


/*************** Dithering Tables ********************************************/
static const float c_4x4[16] = 
{ 
	191,  95, 159,  63, 
	 15, 255, 223, 111, 
	127, 207, 239,  31,
	 47, 143,  79, 175
};
static const uint8_t b_2x2[4] = 
{  
	 0, 128,								  
	32, 160
};
static const uint8_t b_4x4[16] = 
{ 
	  0, 128,  32, 160,
	192,  64, 224,  96,
	 48, 176,  16, 144,
	240, 112, 208,  80 
};
static const uint8_t b_8x8[64] = 
{  
	  0, 128,  32, 160,   8, 136,  40, 168,
	192,  64, 224,  96, 200,  72, 232, 104,
	 48, 176,  16, 144,  56, 184,  24, 152,
	240, 112, 208,  80, 248, 120, 216,  88,
	 12, 140,  44, 172,   4, 132,  36, 164,
	204,  76, 236, 108, 196,  68, 228, 100,
	 60, 188,  28, 156,  52, 180,  20, 148,
	252, 124, 220,  92, 244, 116, 212,  84 
};
static const uint8_t b_16x16[256] =
{  
	  0, 128,  32, 160,   8, 136,  40, 168,   2, 130,  34, 162,  10, 138,  42, 170,
	192,  64, 224,  96, 200,  72, 232, 104, 194,  66, 226,  98, 202,  74, 234, 106,
	 48, 176,  16, 144,  56, 184, 024, 152,  50, 178,  18, 146,  58, 186,  26, 154,
	240, 112, 208,  80, 248, 120, 216,  88, 242, 114, 210,  82, 250, 122, 218,  90,
	 12, 140,  44, 172,   4, 132,  36, 164,  14, 142,  46, 174,   6, 134,  38, 166,
	204,  76, 236, 108, 196,  68, 228, 100, 206,  78, 238, 110, 198,  70, 230, 102,
	 60, 188,  28, 156,  52, 180,  20, 148,  62, 190,  30, 158,  54, 182,  22, 150,
	252, 124, 220,  92, 244, 116, 212,  84, 254, 126, 222,  94, 246, 118, 214,  86,
	  3, 131,  35, 163,  11, 139,  43, 171,   1, 129,  33, 161,   9, 137,  41, 169,
	195,  67, 227,  99, 203,  75, 235, 107, 193,  65, 225,  97, 201,  73, 233, 105,
	 51, 179,  19, 147,  59, 187,  27, 155,  49, 177,  17, 145,  57, 185,  25, 153,
	243, 115, 211,  83, 251, 123, 219,  91, 241, 113, 209,  81, 249, 121, 217,  89,
	 15, 143,  47, 175,   7, 135,  39, 167,  13, 141,  45, 173,   5, 133,  37, 165,
	207,  79, 239, 111, 199,  71, 231, 103, 205,  77, 237, 109, 197,  69, 229, 101,
	 63, 191,  31, 159,  55, 183,  23, 151,  61, 189,  29, 157,  53, 181,  21, 149,
	255, 127, 223,  95, 247, 119, 215,  87, 253, 125, 221,  93, 245, 117, 213,  85 
};
static const uint8_t* matrix_array[4] = { b_2x2, b_4x4, b_8x8, b_16x16 };
/*****************************************************************************/

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
	bool getPixle(uint32_t x, uint32_t y);
	int setPixle(uint32_t x, uint32_t y, bool val);

	int drawHorizontalLine(int32_t x0, int32_t y0, int32_t width, int32_t thick, bool val);
	int drawVerticalLine(int32_t x0, int32_t y0, int32_t length, int32_t thick, bool val);
	int drawBox(int32_t x0, int32_t y0, int32_t length, int32_t width, int32_t thick, bool val);
	int drawBoxFill(int32_t x0, int32_t y0, int32_t length, int32_t width, bool val);

	int import_24bit(const char* fileName, DITHER type = DITHER::Threshold);

	~canvas();

private:

	uint8_t* img_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size);

	/* DITHER!!!! */
	int dither(img* image, DITHER type);
	uint8_t _img_get(img *image, uint32_t x0, uint32_t y0);
	int _img_set(img* image, uint32_t x0, uint32_t y0, uint8_t value);

	int threshold(img *image, uint8_t value);
	int floydSteinberg(img *image);
	int stucki(img* image);
	int jarvis(img* image);
	int atkinson(img* image);
	int bayer(img* image, uint8_t matrix);
	int cluster(img* image);

protected:
	bool _inv = 0;
	bool* ptr = nullptr;
	uint32_t _x = 0, _y = 0;

};

