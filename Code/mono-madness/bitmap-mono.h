// bitmap-mono.h
// 
// MJM - 2021
// 
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE		//I just wanted to use fopen....

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* a=target variable, b=bit number to act upon 0-n */
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

/****************** bitmap structs ******************/
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

/************************************************/


typedef struct {
	bool* ptr;
	size_t x;
	size_t y;
} canvas;


int createBMP(canvas* myCanvas, size_t X, size_t Y);
int freeBMP(canvas* myCanvas);

/* accessor functions, bitmap goodness */

bool getPixle(canvas* myCanvas, size_t x, size_t y);
int setPixle(canvas* myCanvas, size_t x, size_t y, bool val);

int saveCanvas(canvas* myCanvas, int DPI, const char* filename);