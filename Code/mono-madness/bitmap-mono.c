// bitmap-mono.c
// 
// MJM - 2021
// 
//

#include "bitmap-mono.h"

int createBMP(canvas* myCanvas, size_t X, size_t Y)
{
	if (myCanvas->ptr != NULL)
		return 1;

	myCanvas->x = X;
	myCanvas->y = Y;
	myCanvas->ptr = malloc((myCanvas->x * myCanvas->y) * sizeof(bool));
	
	if (myCanvas->ptr == NULL)
		return 1;

	return 0;
}

int freeBMP(canvas* myCanvas)
{
	if (myCanvas->ptr != NULL)
		free(myCanvas->ptr);

	myCanvas->x = 0;
	myCanvas->y = 0;

	return 0;
}

int saveCanvas(canvas* myCanvas, const char* filename)
{
	uint32_t image_y = myCanvas->y;
	uint32_t image_x = (myCanvas->x / 8.0);

	if (image_x == 0)		image_x = 1; //non 0 byte width
	while ((image_x % 4))	image_x++;   //4 byte padding

	header bmpHead;
	bmpHead.Signature	= 0x4D42;
	bmpHead.FileSize	= 54 /*header*/ + 8 /*color table*/ + (image_x * image_y) /*bytes*/;
	bmpHead.reserved	= 0x00;
	bmpHead.DataOffset	= 62;





	return 0;
}
