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
bool getPixle(canvas* myCanvas, size_t x0, size_t y0)
{
	if (myCanvas->ptr == NULL)
		return 1;

	if ((x0 >= myCanvas->x) || (y0 >= myCanvas->y))
		return 1;

	return myCanvas->ptr[(y0 * myCanvas->x) + x0];

	return 0;
}

/* write new pixle value to canvas */
int setPixle(canvas* myCanvas, size_t x0, size_t y0, bool val)
{
	if (myCanvas->ptr == NULL) 
		return 1;

	if ((x0 >= myCanvas->x) || (y0 >= myCanvas->y))
		return 1;

	myCanvas->ptr[(y0 * myCanvas->x) + x0] = val;

	return 0;
}

/* draw a horizontal line of width, 'width', thickness of 'thick', with value of 'val' */
int drawHorizontalLine(canvas* myCanvas, size_t x0, size_t y0, size_t width, size_t thick, bool val)
{
	if (myCanvas->ptr == NULL)
		return 1;

	if ((thick <= 0) || (x0 + width >= myCanvas->x) || (y0 + thick >= myCanvas->y))
		return 1;

	for (size_t _x = 0; _x < width; _x++)
		for (size_t _y = 0; _y < thick; _y++)
			setPixle(myCanvas, x0 + _x, y0 + _y, val);

	return 0;
}

/* draw a vertical line of length, 'length', thickness of 'thick', with value of 'val' */
int drawVerticalLine(canvas* myCanvas, size_t x0, size_t y0, size_t length, size_t thick, bool val)
{
	if (myCanvas->ptr == NULL)
		return 1;

	if ((thick <= 0) || (y0 + length >= myCanvas->y) || (x0 + thick >= myCanvas->x))
		return 1;

	for (size_t _y = 0; _y < length; _y++)
		for (size_t _x = 0; _x < thick; _x++)
			setPixle(myCanvas, x0 + _x, y0 + _y, val);

	return 0;
}

/* draw a box at x,y, size of L&W, thickness of 'thick', value of 'va'l */
int drawBox(canvas* myCanvas, size_t x0, size_t y0, size_t length, size_t width, size_t thick, bool val)
{
	if (drawHorizontalLine(myCanvas, x0,                 y0,                    width,          thick, val))	return 1;
	if (drawHorizontalLine(myCanvas, x0,                 y0 + length - thick,	width,          thick, val))	return 1;
	if (drawVerticalLine  (myCanvas, x0,                 y0 + thick,            length - thick, thick, val))	return 1;
	if (drawVerticalLine  (myCanvas, x0 + width - thick, y0 + thick,	        length - thick,	thick, val))	return 1;

	return 0;
}

/* draw a filled box at x,y, size of L&W, value of 'val' */
int drawBoxFill(canvas* myCanvas, size_t x0, size_t y0, size_t length, size_t width, bool val)
{
	if (myCanvas->ptr == NULL)
		return 1;

	if ((y0 + length >= myCanvas->y) || (x0 + width >= myCanvas->x))
		return 1;

	for (size_t _y = 0; _y < length; _y++)
		for (size_t _x = 0; _x < width; _x++)
			setPixle(myCanvas, x0 + _x, y0 + _y, val);
	
	return 0;
}

/* invert bitmap */
int invertCanvas(canvas* myCanvas)
{
	if (myCanvas->ptr == NULL) 
		return 1;

	for (size_t i = 0; i < myCanvas->x * myCanvas->y; i++)
		myCanvas->ptr[i] = !myCanvas->ptr[i];

	return 0;
}

/* add canvas to another canvas, transparent alpha = 1, white becomes transparent */
int addSpriteCanvas(canvas* myCanvas, canvas* sprite, size_t x0, size_t y0, bool alpha)
{
	if ((myCanvas->ptr == NULL) || (sprite == NULL))
		return 1;

	if ((sprite->x + x0 > myCanvas->x) || (sprite->y + y0 > myCanvas->y))
		return 1;

	for (size_t _y = 0; _y < sprite->y; _y++)
		for (size_t _x = 0; _x < sprite->x; _x++)
			if (alpha) 	{
				if(getPixle(sprite, _x, _y))
					setPixle(myCanvas, x0 + _x, y0 + _y, 1);
			} 
			else {
				setPixle(myCanvas, x0 + _x, y0 + _y, getPixle(sprite, _x, _y));
			}

	return 0;
}

int fillCanvas(canvas* myCanvas, bool val)
{
	if (myCanvas->ptr == NULL) 
		return 1;

	size_t _asize = myCanvas->x * myCanvas->y * sizeof(bool);

	if (myCanvas->ptr != NULL) {
		uint8_t value = 0x00;
		if (val) value = 0xFF;
		memset(myCanvas->ptr, value, _asize);
	}

	return 0;
}

/* save canvase to fielsystem as a 1-bit monocrhome bitmap @ select DPI */
int saveCanvas(canvas* myCanvas, int DPI, const char* filename)
{
	if (myCanvas->ptr == NULL) 
		return 1;

	uint32_t image_y =            myCanvas->y;
	uint32_t image_x = (uint32_t)(myCanvas->x / 8.0);
	
	if ((image_x * 8) < myCanvas->x) image_x++;   //math fix
	if (image_x == 0)		         image_x = 1; //non 0 byte width
	while ((image_x % 4))	         image_x++;   //4 byte padding

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

int drawLine(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, size_t thick, bool val)
{
	if (myCanvas->ptr == NULL)
		return 1;

	for (size_t i = 0; i < thick; i++)
		if (_drawLine(myCanvas, x0+i, y0, x1+i, y1, val))
			return 1;

	return 0;
}


/**********************************************************************************************************/
// ref: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
int _drawLine(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, bool val)
{
	if (myCanvas->ptr == NULL)
		return 1;

	if (abs((int32_t)y1 - (int32_t)y0) < abs((int32_t)x1 - (int32_t)x0)) {
		if (x0 > x1) {

			if (_draw_low(myCanvas, x1, y1, x0, y0, val))
				return 1;
		}
		else {
			if (_draw_low(myCanvas, x0, y0, x1, y1, val))
				return 1;
		}
	}
	else {
		if (y0 > y1) {
			if (_draw_high(myCanvas, x1, y1, x0, y0, val))
				return 1;
		}
		else {
			if (_draw_high(myCanvas, x0, y0, x1, y1, val))
				return 1;
		}
	}

	return 0;
}

// ref: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
int _draw_high(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, bool val)
{
	if (myCanvas->ptr == NULL)
		return 1;

	if ((x0 >= myCanvas->x) || (y0 >= myCanvas->y) ||
		(x1 >= myCanvas->x) || (y1 >= myCanvas->y))
		return 1;

	int32_t dx = (int32_t)x1 - (int32_t)x0;
	int32_t dy = (int32_t)y1 - (int32_t)y0;
	int32_t xi = 1;

	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}

	int32_t D = (2 * dx) - dy;
	int32_t _x = x0;

	for (int32_t _y = (int32_t)y0; _y < (int32_t)y1; _y++) {
		setPixle(myCanvas, _x, _y, val);
		if (D > 0) {
			_x = _x + xi;
			D = D + (2 * (dx - dy));
		}
		else {
			D = D + 2 * dx;
		}
	}

	return 0;
}

// ref: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
int _draw_low(canvas* myCanvas, size_t x0, size_t y0, size_t x1, size_t y1, bool val)
{
	if (myCanvas->ptr == NULL)
		return 1;

	if ((x0 >= myCanvas->x) || (y0 >= myCanvas->y) ||
		(x1 >= myCanvas->x) || (y1 >= myCanvas->y))
		return 1;

	int32_t dx = (int32_t)x1 - (int32_t)x0;
	int32_t dy = (int32_t)y1 - (int32_t)y0;
	int32_t yi = 1;

	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int32_t D = (2 * dy) - dx;
	int32_t _y = y0;

	for (int32_t _x = (int32_t)x0; _x < (int32_t)x1; _x++) {
		setPixle(myCanvas, _x, _y, val);
		if (D > 0) {
			_y = _y + yi;
			D = D + (2 * (dy - dx));
		}
		else {
			D = D + 2 * dy;
		}
	}

	return 0;
}

//ref: https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
int _drawCircle(canvas* myCanvas, size_t x0, size_t y0, size_t xc, size_t yc, bool val)
{
	if (myCanvas->ptr == NULL) 
		return 1;


	/* bounds check done in setPixle */

	if (setPixle(myCanvas, xc + x0, yc + y0, val)) 		return 1;
	if (setPixle(myCanvas, xc - x0, yc + y0, val)) 		return 1;
	if (setPixle(myCanvas, xc + x0, yc - y0, val)) 		return 1;
	if (setPixle(myCanvas, xc - x0, yc - y0, val)) 		return 1;
	if (setPixle(myCanvas, xc + y0, yc + x0, val)) 		return 1;
	if (setPixle(myCanvas, xc - y0, yc + x0, val)) 		return 1;
	if (setPixle(myCanvas, xc + y0, yc - x0, val)) 		return 1;
	if (setPixle(myCanvas, xc - y0, yc - x0, val)) 		return 1;

	return 0;
}

//ref: https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
int drawCircle(canvas* myCanvas, size_t x0, size_t y0, size_t rad, bool val)
{
	int32_t x = 0;
	int32_t y = rad;
	int32_t d = 3 - 2 * rad;

	if (_drawCircle(myCanvas, x, y, x0, y0, val))
		return 1;

	while (y >= x) {
		x++;
		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else {
			d = d + 4 * x + 6;
		}

		if (_drawCircle(myCanvas, x, y, x0, y0, val))
			return 1;
	}

	return 0;
}