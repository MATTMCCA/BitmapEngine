// bitmap-mono.c
// 
// MJM - 2021
// 
// bitmaps from scratch, code should be pretty portable 
// 
//

#include "bitmap-mono.h"


/* create a bitmap canvas */
int createBMP(canvas* myCanvas, size_t X, size_t Y)
{
	if (myCanvas->ptr != NULL) 
		return 1;

	myCanvas->x = X;
	myCanvas->y = Y;
	size_t _asize = (myCanvas->x * myCanvas->y) * sizeof(bool);
	myCanvas->ptr = malloc(_asize);

	if (myCanvas->ptr == NULL) 
		return 1;

	if (myCanvas->ptr != NULL) 
		memset(myCanvas->ptr, 0x00, _asize);

	return 0;
}

/* free created bitmap canvas */
int freeBMP(canvas* myCanvas)
{
	if (myCanvas->ptr != NULL)
		free(myCanvas->ptr);

	myCanvas->x = 0;
	myCanvas->y = 0;

	return 0;
}

/* get pixle value from canvas */
bool getPixle(canvas* myCanvas, size_t x, size_t y)
{
	if ((x >= myCanvas->x) || (y >= myCanvas->y))
		return 1;

	return myCanvas->ptr[(y * myCanvas->x) + x];

	return 0;
}

/* write new pixle value to canvas */
int setPixle(canvas* myCanvas, size_t x, size_t y, bool val)
{
	if ((x >= myCanvas->x) || (y >= myCanvas->y))
		return 1;

	myCanvas->ptr[(y * myCanvas->x) + x] = val;

	return 0;
}

/* draw a horizontal line of width, 'width', thickness of 'thick', with value of 'val' */
int drawHorizontalLine(canvas* mycavas, size_t x, size_t y, size_t width, size_t thick, bool val)
{
	if ((thick <= 0) || (x + width >= mycavas->x) || (y + thick >= mycavas->y))
		return 1;

	for (size_t _x = 0; _x < width; _x++)
		for (size_t _y = 0; _y < thick; _y++)
			setPixle(mycavas, x + _x, y + _y, val);

	return 0;
}

/* draw a vertical line of length, 'length', thickness of 'thick', with value of 'val' */
int drawVerticalLine(canvas* mycavas, size_t x, size_t y, size_t length, size_t thick, bool val)
{
	if ((thick <= 0) || (y + length >= mycavas->y) || (x + thick >= mycavas->x))
		return 1;

	for (size_t _y = 0; _y < length; _y++)
		for (size_t _x = 0; _x < thick; _x++)
			setPixle(mycavas, x + _x, y + _y, val);

	return 0;
}

/* draw a box at x,y, size of L&W, thickness of 'thick', value of 'va'l */
int drawBox(canvas* mycavas, size_t x, size_t y, size_t length, size_t width, size_t thick, bool val)
{
	if (drawHorizontalLine(mycavas, x,                 y,                   width,          thick, val))	return 1;
	if (drawHorizontalLine(mycavas,	x,                 y + length - thick,	width,          thick, val))	return 1;
	if (drawVerticalLine  (mycavas, x,                 y + thick,           length - thick, thick, val))	return 1;
	if (drawVerticalLine  (mycavas, x + width - thick, y + thick,	        length - thick,	thick, val))	return 1;

	return 0;
}

/* draw a filled box at x,y, size of L&W, value of 'val' */
int drawBoxFill(canvas* mycavas, size_t x, size_t y, size_t length, size_t width, bool val)
{
	if ((y + length >= mycavas->y) || (x + width >= mycavas->x))
		return 1;

	for (size_t _y = 0; _y < length; _y++)
		for (size_t _x = 0; _x < width; _x++)
			setPixle(mycavas, x + _x, y + _y, val);
	
	return 0;
}

/* invert bitmap */
int invertCanvas(canvas* mycavas)
{
	for (size_t i = 0; i < mycavas->x * mycavas->y; i++)
		mycavas->ptr[i] = !mycavas->ptr[i];

	return 0;
}

/* add canvas to another canvas, transparent alpha = 1, white becomes transparent */
int addSpriteCanvas(canvas* mycavas, canvas* sprite, size_t x, size_t y, bool alpha)
{
	if ((sprite->x + x > mycavas->x) || (sprite->y + y > mycavas->y))
		return 1;

	for (size_t _y = 0; _y < sprite->y; _y++)
		for (size_t _x = 0; _x < sprite->x; _x++)
			if (alpha) 	{
				if(getPixle(sprite, _x, _y))
					setPixle(mycavas, x + _x, y + _y, 1);
			} 
			else {
				setPixle(mycavas, x + _x, y + _y, getPixle(sprite, _x, _y));
			}

	return 0;
}

/* save canvase to fielsystem as a 1-bit monocrhome bitmap @ select DPI */
int saveCanvas(canvas* myCanvas, int DPI, const char* filename)
{
	uint32_t image_y =            myCanvas->y;
	uint32_t image_x = (uint32_t)(myCanvas->x / 8.0);

	if (image_x == 0)		image_x = 1; //non 0 byte width
	while ((image_x % 4))	image_x++;   //4 byte padding

	header bmpHead;
	bmpHead.Signature	= 0x4D42;
	bmpHead.FileSize	= 54 /*header*/ + 8 /*color table*/ + (image_x * image_y) /*bytes*/;
	bmpHead.reserved	= 0x00;
	bmpHead.DataOffset	= 62;

	infoHeader bmpInfoHead;
	bmpInfoHead.Size				= 40;
	bmpInfoHead.Width				= myCanvas->x;
	bmpInfoHead.Height				= myCanvas->y;
	bmpInfoHead.Planes				= 0x01;
	bmpInfoHead.Bits_Per_Pixel		= 0x01;
	bmpInfoHead.Compression			= 0x00;
	bmpInfoHead.ImageSize			= image_x * image_y;
	bmpInfoHead.XpixelsPerM			= (uint32_t)(DPI * 39.3701);
	bmpInfoHead.YpixelsPerM			= (uint32_t)(DPI * 39.3701);
	bmpInfoHead.Colors_Used			= 0x00;
	bmpInfoHead.Important_Colors	= 0x00;

	uint8_t* BMPDATA = NULL;
	size_t BMPDATASIZE = image_x * image_y;
	BMPDATA = malloc(BMPDATASIZE); //raw data for bmp

	if (BMPDATA != NULL) {
		memset(BMPDATA, 0xFF, BMPDATASIZE);

		for (size_t _y = 0; _y < myCanvas->y; _y++) {
			uint8_t* in_line = &myCanvas->ptr[(_y * myCanvas->x)];
			uint8_t* out_line = &BMPDATA[(((image_y-1) - _y) * image_x)];

			for (size_t _x = 0; _x < myCanvas->x; _x++) {
				if (in_line[_x]) BIT_CLEAR(out_line[(_x / 8)], (7 - (_x % 8)));
			}
		}
	}

	FILE* fd;
	fd = fopen(filename, "wb");

	if ((fd != NULL) && (BMPDATA != NULL))
	{
		//TODO: add error handling, or not...
		/*************************************************************/
		fwrite(&bmpHead.Signature,            sizeof(uint16_t), 1, fd);
		fwrite(&bmpHead.FileSize,             sizeof(uint32_t), 1, fd);
		fwrite(&bmpHead.reserved,             sizeof(uint32_t), 1, fd);
		fwrite(&bmpHead.DataOffset,           sizeof(uint32_t), 1, fd);
		/*************************************************************/
		fwrite(&bmpInfoHead.Size,             sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.Width,            sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.Height,           sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.Planes,           sizeof(uint16_t), 1, fd);
		fwrite(&bmpInfoHead.Bits_Per_Pixel,   sizeof(uint16_t), 1, fd);
		fwrite(&bmpInfoHead.Compression,      sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.ImageSize,        sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.XpixelsPerM,      sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.YpixelsPerM,      sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.Colors_Used,      sizeof(uint32_t), 1, fd);
		fwrite(&bmpInfoHead.Important_Colors, sizeof(uint32_t), 1, fd);
		/*************************************************************/
		fwrite(&COLOR_TABLE,                  sizeof(uint32_t), 2, fd);
		/**********************************************************************/
		fwrite(BMPDATA,                       sizeof(uint8_t), BMPDATASIZE, fd);
		/**********************************************************************/
		fclose(fd);
	}

	if (BMPDATA != NULL)
		free(BMPDATA); //release when done

	return 0;
}