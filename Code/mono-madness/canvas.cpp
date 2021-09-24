// ! ref !
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/fontconvert.c
// 
//
//MJM 2021

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
		uint32_t __y = 0, __x = 0;
		for (__y = 0; __y < src->_y; __y++)
			for (__x = 0; __x < src->_x; __x++)
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

		bool* in_line;
		uint8_t* out_line;
		uint32_t __y = 0, __x = 0;

		image_y--;
		for (__y = 0; __y < _y; __y++) {
			in_line = &ptr[(__y * _x)];
			out_line = &BMPDATA[(((image_y) - __y) * image_x)];

			for (__x = 0; __x < _x; __x++) {
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

bool canvas::getPixle(uint32_t x, uint32_t y)
{
	if (ptr != nullptr) {
		if ((y < _y) && (x < _x)) {
			return ptr[(y * _x) + x];
		}
	}
	return 1;
}

bool canvas::getInvert(void)
{
	return _inv;
}

int canvas::setPixle(uint32_t x, uint32_t y, bool val)
{
	if (ptr != nullptr)	{
		if ((y < _y) && (x < _x)) {
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

int canvas::drawHorizontalLine(int32_t x0, int32_t y0, int32_t width, int32_t thick, bool val)
{
	if (ptr != nullptr)
	{
		int32_t x = 0, y = 0;
		for (x = 0; x < width; x++)
			for (y = 0; y < thick; y++)
				setPixle(x0 + x, y0 + y, val);
	}
	return 0;
}

int canvas::drawVerticalLine(int32_t x0, int32_t y0, int32_t length, int32_t thick, bool val)
{
	if (ptr != nullptr)
	{
		int32_t x = 0, y = 0;
		for (y = 0; y < length; y++)
			for (x = 0; x < thick; x++)
				setPixle(x0 + x, y0 + y, val);
	}
	return 0;
}

int canvas::drawBox(int32_t x0, int32_t y0, int32_t length, int32_t width, int32_t thick, bool val)
{
	if (drawHorizontalLine(x0, y0, width, thick, val))	return 1;
	if (drawHorizontalLine(x0, y0 + length - thick, width, thick, val))	return 1;
	if (drawVerticalLine(x0, y0 + thick, length - thick, thick, val))	return 1;
	if (drawVerticalLine(x0 + width - thick, y0 + thick, length - thick, thick, val))	return 1;

	return 0;
}

int canvas::drawBoxFill(int32_t x0, int32_t y0, int32_t length, int32_t width, bool val)
{
	if (ptr != nullptr)
	{
		int32_t x = 0, y = 0;
		for (y = 0; y < length; y++)
			for (x = 0; x < width; x++)
				setPixle(x0 + x, y0 + y, val);
	}
	return 0;
}

uint8_t* canvas::img_open(const char* fileName, uint32_t *x0, uint32_t *y0, uint32_t *_size)
{
	uint32_t image_size = 0;
	uint8_t* image = nullptr;

	header bmpHead = { 0x00 };
	infoHeader bmpInfoHead = { 0x00 };

	FILE* fd;
	fd = fopen(fileName, "rb");

	if (fd != NULL)
	{
		fseek(fd, 0, SEEK_END);
		size_t file_size = ftell(fd);
		fseek(fd, 0, SEEK_SET);

		if (file_size >= 0x0036)
		{
			//no error checking, yet!
			//fread will fail hard!
			/////////////////////////////////////////////////////////////////////////
			fread(&bmpHead.Signature, sizeof(uint16_t), 1, fd);
			fread(&bmpHead.FileSize, sizeof(uint32_t), 1, fd);
			fread(&bmpHead.reserved, sizeof(uint32_t), 1, fd);
			fread(&bmpHead.DataOffset, sizeof(uint32_t), 1, fd);
			/////////////////////////////////////////////////////////////////////////
			fread(&bmpInfoHead.Size, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.Width, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.Height, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.Planes, sizeof(uint16_t), 1, fd);
			fread(&bmpInfoHead.Bits_Per_Pixel, sizeof(uint16_t), 1, fd);
			fread(&bmpInfoHead.Compression, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.ImageSize, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.XpixelsPerM, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.YpixelsPerM, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.Colors_Used, sizeof(uint32_t), 1, fd);
			fread(&bmpInfoHead.Important_Colors, sizeof(uint32_t), 1, fd);

			/* bmp check is not very good */
			if ((bmpInfoHead.Bits_Per_Pixel != 24) || (bmpHead.Signature != 0x4D42) ||
				(bmpInfoHead.Compression != 0) || (bmpInfoHead.Colors_Used != 0) ||
				(bmpHead.FileSize - 0x36 != bmpInfoHead.ImageSize)) {
				fclose(fd);
				return nullptr;
			}

			/* reads image */
			image_size = bmpInfoHead.ImageSize;
			image = new uint8_t[image_size];
			fread(image, sizeof(uint8_t), image_size, fd);
		}
		fclose(fd);
	}

	/* got the image */
	if (image != nullptr) {
		*y0 = bmpInfoHead.Height;
		*x0 = bmpInfoHead.Width;
		*_size = bmpInfoHead.ImageSize;
		return image;
	}

	return nullptr;
}

int canvas::import_24bit(const char* fileName, DITHER type)
{
	double lum;
	uint32_t y0 = 0, x0 = 0, x1 = 0;
	uint32_t x = 0, y = 0, s = 0;
	uint8_t* gray = nullptr;

	uint8_t* row = nullptr;
	uint8_t* image_24 = img_open(fileName, &x, &y, &s);

	if (image_24 != nullptr) 
	{
		uint32_t i_x = s / y, i_y = y, i_v = i_x / 3, gray_size = i_v * i_y;

		gray = new uint8_t[gray_size];
		uint8_t* tmp = gray;
	
		for (y0 = 0; y0 < i_y; y0++) 
		{
			x1 = 0;
			row = &image_24[i_x * y0];

			for (x0 = 0; x0 < i_v; x0++) 
			{
				//https://stackoverflow.com/questions/4147639/converting-color-bmp-to-grayscale-bmp
				/*                blue?              green?               red?             */
				lum = (row[x1++] * 0.11) + (row[x1++] * 0.59) + (row[x1++] * 0.30);
				if (lum < 0) lum = 0;
				if (lum > 255) lum = 255;
				*tmp = (uint8_t) lum;
				tmp++;
			}
		}
		delete[] image_24;

		if (gray != nullptr) 
		{
			//create image struct
			img _img = { gray, i_v, i_y }; 

			/* dither */
			switch (type)
			{
			case DITHER::_FloydSteinberg:
				FloydSteinberg(&_img);
				break;

			case DITHER::_Stucki:
				Stucki(&_img);
				break;

			default:
				Threshold(&_img, 256 / 2);
				break;
			};

			x1 = i_y - 1; //reused, not x, but y
			create(i_v, i_y, 0);			
			for (y0 = 0; y0 < i_y; y0++) {
				for (x0 = 0; x0 < i_v; x0++) {
					tmp = gray + ( ((((x1) - y0) * i_v)) + x0 );
					s = *tmp;

					if (s == 0) setPixle(x0, y0, 1);
				}
			}
			delete[] gray;
		}
	}

	return 0;
}

uint8_t canvas::_img_get(img* image, uint32_t x0, uint32_t y0)
{
	if (x0 < image->__img_x && y0 < image->__img_y) {
		return image->__img[(image->__img_x * y0) + x0];
	}
	return 0;
}

int canvas::_img_set(img* image, uint32_t x0, uint32_t y0, uint8_t value)
{
	if (x0 < image->__img_x && y0 < image->__img_y)	{
		image->__img[(image->__img_x * y0) + x0] = value;
		return 0;
	}
	return 1;
}

int canvas::Threshold(img* image, uint8_t value)
{
	if (image->__img != nullptr)
	{
		uint8_t newpixel;
		for (uint32_t y = 0; y < image->__img_y; y++) {
			for (uint32_t x = 0; x < image->__img_x; x++) {
				newpixel = (_img_get(image, x, y) < value) ? 0 : 255;
				_img_set(image, x, y, newpixel);
			}
		}
		return 0;
	}
	return 1;
}

/* https://imagej.net/Dithering */
int canvas::FloydSteinberg(img* image)
{	
	if (image->__img != nullptr)
	{
		int16_t oldpixel, newpixel; //uint8_t ?
		float quant_error;
		float w1 = 7.0 / 16.0, w2 = 3.0 / 16.0, w3 = 5.0 / 16.0, w4 = 1.0 / 16.0;
		uint32_t y = 0, x = 0;

		for (y = 0; y < image->__img_y; y++)
		{
			for (x = 0; x < image->__img_x; x++)
			{
				oldpixel = _img_get(image, x, y);
				newpixel = (oldpixel < 128) ? 0 : 255;
				_img_set(image, x, y, (uint8_t)newpixel);
				quant_error = (float)(oldpixel - newpixel);
				_img_set(image, x + 1, y,     (uint8_t) (_img_get(image, x + 1, y)      + w1 * quant_error));
				_img_set(image, x - 1, y + 1, (uint8_t) (_img_get(image, x - 1, y + 1)  + w2 * quant_error));
				_img_set(image, x, y + 1,     (uint8_t) (_img_get(image, x, y + 1)      + w3 * quant_error));
				_img_set(image, x + 1, y + 1, (uint8_t) (_img_get(image, x + 1, y + 1)  + w4 * quant_error));
			}
		}
		return 0;
	}
	return 1;
}

/* https://imagej.net/Dithering */
int canvas::Stucki(img* image)
{
	if (image->__img != nullptr)
	{
		float quant_error;
		int16_t oldpixel, newpixel; //uint8_t ?
		float w8 = (float)(8.0 / 42.0),
			  w7 = (float)(7.0 / 42.0),
			  w5 = (float)(5.0 / 42.0),
			  w4 = (float)(4.0 / 42.0),
			  w2 = (float)(2.0 / 42.0),
			  w1 = (float)(1.0 / 42.0);

		uint32_t y = 0, x = 0;

		for (y = 0; y < image->__img_y; y++)
		{
			for (x = 0; x < image->__img_x; x++)
			{
				oldpixel = _img_get(image, x, y);
				newpixel = (oldpixel < 128) ? 0 : 255;
				_img_set(image, x, y, (uint8_t)newpixel);
				quant_error = (int16_t)(oldpixel - newpixel);
				_img_set(image, x + 1, y,     (uint8_t)(_img_get(image, x + 1, y)     + w7 * quant_error));
				_img_set(image, x + 2, y,     (uint8_t)(_img_get(image, x + 2, y)     + w5 * quant_error));
				_img_set(image, x - 2, y + 1, (uint8_t)(_img_get(image, x - 2, y + 1) + w2 * quant_error));
				_img_set(image, x - 1, y + 1, (uint8_t)(_img_get(image, x - 1, y + 1) + w4 * quant_error));
				_img_set(image, x, y + 1,     (uint8_t)(_img_get(image, x, y + 1)     + w8 * quant_error));
				_img_set(image, x + 1, y + 1, (uint8_t)(_img_get(image, x + 1, y + 1) + w4 * quant_error));
				_img_set(image, x + 2, y + 1, (uint8_t)(_img_get(image, x + 2, y + 1) + w2 * quant_error));
				_img_set(image, x - 2, y + 2, (uint8_t)(_img_get(image, x - 2, y + 2) + w1 * quant_error));
				_img_set(image, x - 1, y + 2, (uint8_t)(_img_get(image, x - 1, y + 2) + w2 * quant_error));
				_img_set(image, x, y + 2,     (uint8_t)(_img_get(image, x, y + 2)     + w4 * quant_error));
				_img_set(image, x + 1, y + 2, (uint8_t)(_img_get(image, x + 1, y + 2) + w2 * quant_error));
				_img_set(image, x + 2, y + 2, (uint8_t)(_img_get(image, x + 2, y + 2) + w1 * quant_error));
			}
		}
		return 0;
	}
	return 1;
}