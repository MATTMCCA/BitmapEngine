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

static const uint32_t COLOR_TABLE[2] = { 0x00000000, 0xFFFFFF00 }; //might need to flip

/************************************************/


typedef struct {
	bool* ptr;
	size_t x;
	size_t y;
} canvas;


int createBMP(canvas* myCanvas, size_t X, size_t Y);
int freeBMP(canvas* myCanvas);

int saveCanvas(canvas* myCanvas, int DPI, const char* filename);