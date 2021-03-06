/*
    ***************************************************************************************
    MJM -2021-
    canvas / bitmap Lib

    =======================================================================================

    File: canvas.hpp

        Notes: 

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
*/

#pragma once

//#define _CRT_SECURE_NO_DEPRECATE //fixed

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstdbool>
#include <cstring>
#include <cmath>
#include <string> //ugh

#include "lodepng.hpp"
#include "bit_def.hpp"

extern "C" {
#include "jbig85.h"
}

#define COMP_XMAX 1024*8 //jbig buffer size


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
    float*   __img;
    uint32_t __img_x;
    uint32_t __img_y;
} img;

typedef struct {
    uint8_t* __img;
    uint32_t __img_len;
} jbg_buffer;

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

enum class MIRROR
{
    Vertical,
    Horizontal
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

enum class DEGREE
{
    ROT_0,
    ROT_90,
    ROT_180,
    ROT_270,
    ROT_360
};

/* jbig callbacks */
static void data_out(unsigned char* start, size_t len, void* file);
static int line_out(const struct jbg85_dec_state* s, unsigned char* start, size_t len, unsigned long y, void* file);

class canvas
{
public:
    canvas();

    bool create(int32_t x, int32_t y, bool invert = 0);
    bool create(bool* raw, int32_t x, int32_t y, bool invert = 0);

    int32_t get_y(void);
    int32_t get_x(void);
    bool getInvert(void);
    bool getSize(int32_t* x, int32_t* y);

    bool fill(bool val);
    bool invert(bool invert);
    bool rotate(DEGREE rot);
    bool rotate_full(int angle);
    bool mirror(MIRROR m);

    bool addSprite(canvas* src, int32_t x, int32_t y, bool alpha);

    bool getPixle(uint32_t x, uint32_t y);
    bool setPixle(uint32_t x, uint32_t y, bool val);

    bool drawHorizontalLine(int32_t x0, int32_t y0, int32_t width, int32_t thick, bool val);
    bool drawVerticalLine(int32_t x0, int32_t y0, int32_t length, int32_t thick, bool val);
    bool drawBox(int32_t x0, int32_t y0, int32_t length, int32_t width, int32_t thick, bool val);
    bool drawBoxFill(int32_t x0, int32_t y0, int32_t length, int32_t width, bool val);

    bool import_24bit(const char* fileName, DITHER type = DITHER::Threshold, int b_level = 0, int c_level = 0);
    bool import_png(const char* fileName, DITHER type = DITHER::Threshold, int b_level = 0, int c_level = 0);

    bool import_jbg(const char* fileName);
    bool import_pbm(const char* fileName);

    bool saveBMP(const char* fileName, int DPI);
    bool savePBM(const char* fileName); //portable bitmap raw
    bool saveJBG(const char* fileName); //JBIG (Joint Bi-level Image Experts Group)
    bool saveXBM(const char* fileName, const char* structName);

    /* MICROCOM COMPRESSION BINARY, Mode 2, no flash header */
    // https://microcomcorp.com/downloads/bmp2mic.zip
    bool saveMCB(const char* fileName, int headSize);
    
    bool scale(float x0, float y0);

    bool* get_pointer(void) {
        return this->ptr;
    }

    bool TEST(int head_size); //M-COM RLE TEST, mode 2

    ~canvas();

    canvas& operator= (const canvas& c) {
        if (c.ptr != nullptr) {
            uint32_t q = (c._x * c._y * sizeof(bool));
            create(c._x, c._y, c._inv);
            memcpy(ptr, c.ptr, q);
        }
        return *this;
    }

private:

    bool rot_calc(int32_t* x0, int32_t* y0, uint32_t x_size, uint32_t y_size, int angle);

    bool adj_brightness(uint8_t* _ptr, uint32_t x0, uint32_t y0, int val);
    bool adj_contrast(uint8_t* _ptr, uint32_t x0, uint32_t y0, int val);

    uint8_t* bmp_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size);
    uint8_t* jbg_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size);
    uint8_t* pbm_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size);
    uint8_t* png_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size);

    /* DITHER */
    bool dither(img* image, DITHER type);
    float _img_get(img* image, uint32_t x0, uint32_t y0);
    bool _img_set(img* image, uint32_t x0, uint32_t y0, float value);

    bool threshold(img *image, uint8_t value);
    bool floydSteinberg(img *image);
    bool stucki(img* image);
    bool jarvis(img* image);
    bool atkinson(img* image);
    bool bayer(img* image, uint8_t matrix);
    bool cluster(img* image);

    bool _pack_bool(uint8_t** BMPDATA, uint32_t* row, bool ort = 1, int byte_bound = 1);

protected:
    bool _inv = 0;
    bool* ptr = nullptr;
    uint32_t _x = 0, _y = 0;

};