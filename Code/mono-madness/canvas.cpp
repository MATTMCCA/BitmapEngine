#include "canvas.h"

canvas::canvas()
{
}

int canvas::create(int32_t x, int32_t y, bool invert)
{
	if (ptr != nullptr)
		return 1;

	_x = x;
	_y = y;
	_inv = invert;

	size_t _asize = (_x * _y) * sizeof(bool);
	ptr = new bool[_asize] {0};

	return 0;
}

int canvas::getSize(int32_t* x, int32_t* y)
{
	*x = _x;
	*y = _y;
	return 0;
}

int canvas::fill(bool val)
{
	if (ptr != nullptr)
		return 1;

	delete[] ptr;
	size_t _asize = (_x * _y) * sizeof(bool);
	ptr = new bool[_asize] {1};

	return 0;
}

int32_t canvas::get_y(void)
{
	return _y;
}

int32_t canvas::get_x(void)
{
	return _x;
}

int canvas::addSprite(canvas* src, int32_t x, int32_t y, bool alpha)
{
	if ((src->ptr != nullptr) && (ptr != nullptr))
	{
		for (int32_t __y = 0; __y < src->_y; __y++)
			for (int32_t __x = 0; __x < src->_x; __x++)
				if (alpha) {
					if (src->getPixle( __x, __y) ^ src->getInvert())
						setPixle(x + __x, y + __y, 1);
				}
				else {
					setPixle(x + __x, y + __y, src->getPixle(__x, __y) ^ src->getInvert());
				}

		return 0;
	}

	return 1;
}

int canvas::save(const char* fileName, int DPI)
{
	if (ptr == nullptr)
		return 1;

	uint32_t image_y = _y;
	uint32_t image_x = (uint32_t)(_x / 8.0);

	if ((image_x * 8) < (uint32_t)_x)	         
		image_x++;   //math fix

	if (image_x == 0)							 
		image_x = 1; //non 0 byte width

	while ((image_x % 4))						 
		image_x++;   //4 byte padding

	header bmpHead;
	bmpHead.Signature = 0x4D42;
	
	bmpHead.FileSize = 54 /*header*/ + 8 /*color table*/ + (image_x * image_y) /*bytes*/;
	
	bmpHead.reserved = 0x00;
	bmpHead.DataOffset = 62;

	infoHeader bmpInfoHead;
	bmpInfoHead.Size = 40;
	bmpInfoHead.Width = _x;
	bmpInfoHead.Height = _y;
	bmpInfoHead.Planes = 0x01;
	bmpInfoHead.Bits_Per_Pixel = 0x01;
	bmpInfoHead.Compression = 0x00;
	bmpInfoHead.ImageSize = image_x * image_y;
	bmpInfoHead.XpixelsPerM = (uint32_t)(DPI * 39.3701);
	bmpInfoHead.YpixelsPerM = (uint32_t)(DPI * 39.3701);
	bmpInfoHead.Colors_Used = 0x00;
	bmpInfoHead.Important_Colors = 0x00;

	uint8_t* BMPDATA = nullptr;
	size_t BMPDATASIZE = image_x * image_y;
	BMPDATA = new uint8_t[BMPDATASIZE]{ 0x00 };

	if (BMPDATA != nullptr) {
		memset(BMPDATA, 0xFF, BMPDATASIZE);

		for (int32_t __y = 0; __y < _y; __y++) {
			bool* in_line = &ptr[(__y * _x)];
			uint8_t* out_line = &BMPDATA[(((image_y - 1) - __y) * image_x)];

			for (int32_t __x = 0; __x < _x; __x++) {
				if (in_line[__x] ^ _inv) BIT_CLEAR(out_line[(__x / 8)], (7 - (__x % 8)));
			}
		}

		FILE* fd;
		fd = fopen(fileName, "wb");

		if (fd != NULL)
		{
			//TODO: add error handling, or not...
			/////////////////////////////////////////////////////////////////////////
			fwrite(&bmpHead.Signature, sizeof(uint16_t), 1, fd);
			fwrite(&bmpHead.FileSize, sizeof(uint32_t), 1, fd);
			fwrite(&bmpHead.reserved, sizeof(uint32_t), 1, fd);
			fwrite(&bmpHead.DataOffset, sizeof(uint32_t), 1, fd);
			/////////////////////////////////////////////////////////////////////////
			fwrite(&bmpInfoHead.Size, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.Width, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.Height, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.Planes, sizeof(uint16_t), 1, fd);
			fwrite(&bmpInfoHead.Bits_Per_Pixel, sizeof(uint16_t), 1, fd);
			fwrite(&bmpInfoHead.Compression, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.ImageSize, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.XpixelsPerM, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.YpixelsPerM, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.Colors_Used, sizeof(uint32_t), 1, fd);
			fwrite(&bmpInfoHead.Important_Colors, sizeof(uint32_t), 1, fd);
			/////////////////////////////////////////////////////////////////////////
			fwrite(&COLOR_TABLE, sizeof(uint32_t), 2, fd);
			/////////////////////////////////////////////////////////////////////////
			fwrite(BMPDATA, sizeof(uint8_t), BMPDATASIZE, fd);
			/////////////////////////////////////////////////////////////////////////
			fclose(fd);
		}

		delete[] BMPDATA; //release when done
		return 0;
	}
	
	return 1;
}

int canvas::invert(bool invert)
{
	_inv = invert;
	return 0;
}

bool canvas::getPixle(int32_t x, int32_t y)
{
	if (ptr != nullptr) {
		if ((x >= 0) && (y >= 0) && (y < _y) && (x < _x)) {
			return ptr[(y * _x) + x];
		}
	}
	return 1;
}

bool canvas::getInvert(void)
{
	return _inv;
}

int canvas::setPixle(int32_t x, int32_t y, bool val)
{
	if (ptr != nullptr)	{
		if ((x >= 0) && (y >= 0) && (y < _y) && (x < _x)) {
			ptr[(y * _x) + x] = val;
			return 0;
		}
	}

	return 1;
}

canvas::~canvas()
{
	if (ptr != nullptr)
		delete[] ptr;
}
