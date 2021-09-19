// bitmap-mono.h
// 
// MJM - 2021
// 
// bitmaps from scratch, code should be pretty portable 
// 
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE				//I just wanted to use fopen....

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "font-mono.h"

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
	bool* ptr;
	size_t x;
	size_t y;
} canvas;

// create a bitmap canvas
int createBMP(canvas* myCanvas, size_t X, size_t Y);

// free created bitmap canvas 
int freeBMP(canvas* myCanvas);


// get pixle value from canvas 
bool getPixle(canvas* myCanvas, size_t x0, size_t y0);
/* write new pixle value to canvas */
int setPixle(canvas* myCanvas, size_t x0, size_t y0, bool val);

// draw a horizontal line of width, 'width', thickness of 'thick', with value of 'val' 
int drawHorizontalLine(canvas* myCanvas, size_t x0, size_t y0, size_t width, size_t thick, bool val);

// draw a vertical line of length, 'length', thickness of 'thick', with value of 'val' 
int drawVerticalLine(canvas* myCanvas, size_t x0, size_t y0, size_t length, size_t thick, bool val);

// draw a line from x0,y0 -> x1, y1, with a thickness of 'thick', value of 'val' 
//int drawLine(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, size_t thick, bool val);

// draw a box at x,y, size of L&W, thickness of 'thick', value of 'val' 
int drawBox(canvas* myCanvass, size_t x0, size_t y0, size_t length, size_t width, size_t thick, bool val);

/* draw a filled box at x,y, size of L&W, value of 'val' */
int drawBoxFill(canvas* myCanvas, size_t x0, size_t y0, size_t length, size_t width, bool val);


// WIP, need to change a bunch of things with circle logic
// draw circle @ x,y with radious of 'rad' 
//int drawCircle(canvas* myCanvas, size_t x0, size_t y0, size_t rad, bool val);

// fill with val
int fillCanvas(canvas* myCanvas, bool val);

// invert bitmap 
int invertCanvas(canvas* myCanvas);

// add canvas to another canvas, transparent alpha = 1, white becomes transparent 
int addSpriteCanvas(canvas* myCanvas, canvas* sprite, size_t x, size_t y, bool alpha);




// save canvase to fielsystem as a 1-bit monocrhome bitmap @ select DPI 
int saveCanvas(canvas* myCanvas, int DPI, const char* filename);


int drawChar(canvas* myCanvas, GFXfont* _font, unsigned char c, int16_t x0, int16_t y0);
int write(canvas* myCanvas, GFXfont* _font, unsigned char c, int16_t* cursor_x, int16_t* cursor_y);
//int write(canvas* myCanvas, GFXfont* _font, unsigned char c, int16_t* cursor_x, int16_t* cursor_y)

int print(canvas* myCanvas, font_att* _font, const char str[], int16_t x0, int16_t y0);



/*********************************** helpers **********************************************/
/*
int _draw_low(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, bool val);
int _draw_high(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, bool val);
int _drawLine(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, bool val);
int _drawCircle(canvas* myCanvas, size_t x0, size_t y0, size_t xc, size_t yc, bool val);
*/