// ! ref !
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/fontconvert.c
//
//MJM 2021

#include "font.hpp"

font::font()
{
}

int font::create(const char* fontName, int point, int dpi)
{
	int					i, j, err, byte;
	int					first = ' ';
	int					last = '~';
	int32_t			    bitmapOffset = 0;
	unsigned int		x, y;

	FT_Library			library;
	FT_Face				face;
	FT_Glyph			glyph;
	FT_Bitmap*          bitmap;
	FT_BitmapGlyphRec*  g;
	uint8_t				bit;
	
	std::vector<uint8_t> BITMAP;

	myFont = new GFXfont;
	myFont->glyph = new GFXglyph[last - first + 1];

	// Init FreeType lib, load font
	if ((err = FT_Init_FreeType(&library)))
	{
		fprintf(stderr, "FreeType init error: %d\n", err);
		return err;
	}

	FT_UInt interpreter_version = TT_INTERPRETER_VERSION_35;
	FT_Property_Set(library, "truetype", "interpreter-version", &interpreter_version);

	if ((err = FT_New_Face(library, fontName, 0, &face)))
	{
		fprintf(stderr, "Font load error: %d\n", err);
		FT_Done_FreeType(library);
		return err;
	}

	// << 6 because '26dot6' fixed-point format
	FT_Set_Char_Size(face, point << 6, 0, dpi, 0);

	// Process glyphs and output huge bitmap data array
	for (i = first, j = 0; i <= last; i++, j++)
	{
		// MONO renderer provides clean image with perfect crop
		// (no wasted pixels) via bitmap struct.
		if ((err = FT_Load_Char(face, i, FT_LOAD_TARGET_MONO)))
		{
			fprintf(stderr, "Error %d loading char '%c'\n", err, i);
			return err;
		}

		if ((err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO)))
		{
			fprintf(stderr, "Error %d rendering char '%c'\n", err, i);
			return err;
		}

		if ((err = FT_Get_Glyph(face->glyph, &glyph)))
		{
			fprintf(stderr, "Error %d getting glyph '%c'\n", err, i);
			return err;
		}

		bitmap = &face->glyph->bitmap;
		g = (FT_BitmapGlyphRec*)glyph;

		myFont->glyph[j].bitmapOffset = bitmapOffset;
		myFont->glyph[j].width = bitmap->width;
		myFont->glyph[j].height = bitmap->rows;
		myFont->glyph[j].xAdvance = (uint32_t)(face->glyph->advance.x >> 6);
		myFont->glyph[j].xOffset = g->left;
		myFont->glyph[j].yOffset = 1 - g->top;

		for (y = 0; y < bitmap->rows; y++)
		{
			for (x = 0; x < bitmap->width; x++)
			{
				byte = x / 8;
				bit = 0x80 >> (x & 7);
				enbit(bitmap->buffer[y * bitmap->pitch + byte] & bit, &BITMAP);
			}
		}

		// Pad end of char bitmap to next byte boundary if needed
		int n = (bitmap->width * bitmap->rows) & 7;
		if (n) // Pixel count not an even multiple of 8?
		{
			n = 8 - n; // # bits to next multiple
			while (n--)
			{
				enbit(0, &BITMAP);
			}
		}
		bitmapOffset += (bitmap->width * bitmap->rows + 7) / 8;
		FT_Done_Glyph(glyph);
	}

	uint32_t h = BITMAP.size();
	myFont->bitmap = new uint8_t[h] {0x00};

	for (uint32_t i = 0; i < h; i++)
		myFont->bitmap[i] = BITMAP[i];

	myFont->first = first;
	myFont->last = last;

	if (face->size->metrics.height == 0) {
		myFont->yAdvance = myFont->glyph[0].height;
	}
	else {
		myFont->yAdvance = (uint32_t)(face->size->metrics.height >> 6);
	}

	FT_Done_FreeType(library);

	return 0;
}

int font::changeCharOffset(int32_t x, int32_t y)
{
	int	i, j;
	int	first = ' ';
	int	last = '~';

	if (myFont != nullptr) {
		myFont->yAdvance += y;
		for (i = first, j = 0; i <= last; i++, j++)
			myFont->glyph[j].xAdvance += x;
	}
	return 0;
}

GFXfont* font::getGFXfont(void)
{
	return myFont;
}

int font::writeCanvas(canvas* ptr, const char* str, int32_t x0, int32_t y0)
{
	int32_t x, y;
	uint32_t w, h;

	getTextBounds(str, 0, 0, &x, &y, &w, &h);

	ptr->create(w, h, 0); //return 1

	if (x != 0) {
		if (x > 0) {
			x = 0 - x;
		}
		else {
			x *= -1;
		}
	}

	if (y != 0) {
		if (y > 0) {
			y = 0 - y;
		}
		else {
			y *= -1;
		}
	}

	print(ptr, str, x + x0, y + y0, x0);

	return 0;
}

/*!
	@brief  Helper to determine size of a character with current font/size.
			Broke this out as it's used by both the PROGMEM- and RAM-resident
			getTextBounds() functions.
	@param  c     The ASCII character in question
	@param  x     Pointer to x location of character. Value is modified by
				  this function to advance to next character.
	@param  y     Pointer to y location of character. Value is modified by
				  this function to advance to next character.
	@param  minx  Pointer to minimum X coordinate, passed in to AND returned
				  by this function -- this is used to incrementally build a
				  bounding rectangle for a string.
	@param  miny  Pointer to minimum Y coord, passed in AND returned.
	@param  maxx  Pointer to maximum X coord, passed in AND returned.
	@param  maxy  Pointer to maximum Y coord, passed in AND returned.
*/

int font::charBounds(unsigned char c, int32_t* x, int32_t* y, int32_t* minx, int32_t* miny, int32_t* maxx, int32_t* maxy)
{
	if (c == '\n') // Newline?
	{ 
		*x = 0;									// Reset x to zero, advance y by one line
		*y += (uint32_t) myFont->yAdvance;
	}

	else if (c != '\r') // Not a carriage return; is normal char
	{ 
		uint8_t first = myFont->first;
		uint8_t last = myFont->last;
		if ((c >= first) && (c <= last)) // Char present in this font?
		{ 
			GFXglyph* glyph = &myFont->glyph[c - first];
			uint32_t gw = glyph->width;
			uint32_t gh = glyph->height;
			uint32_t xa = glyph->xAdvance;

			int32_t xo = glyph->xOffset;
			int32_t yo = glyph->yOffset;
			
			/* fix at some point */
			//if (/*wrap*/ true && ((*x + (((int16_t)xo + gw) )) > _width)) 
			//{
			//	*x = 0; // Reset x to zero, advance y by one line
			//	*y += (uint8_t)myFont->yAdvance;
			//}

			int32_t tsx = 1, tsy = 1;
			int32_t x1 = *x + xo * tsx;
			int32_t y1 = *y + yo * tsy;
			int32_t x2 = x1 + gw * tsx - 1;
			int32_t y2 = y1 + gh * tsy - 1;
			if (x1 < *minx)
			*minx = x1;
			if (y1 < *miny)
			*miny = y1;
			if (x2 > *maxx)
			*maxx = x2;
			if (y2 > *maxy)
			*maxy = y2;
			*x += xa * tsx;
		}
	}

	return 0;
}

/*!
	@brief  Helper to determine size of a string with current font/size.
			Pass string and a cursor position, returns UL corner and W,H.
	@param  str  The ASCII string to measure
	@param  x    The current cursor X
	@param  y    The current cursor Y
	@param  x1   The boundary X coordinate, returned by function
	@param  y1   The boundary Y coordinate, returned by function
	@param  w    The boundary width, returned by function
	@param  h    The boundary height, returned by function
*/

int font::getTextBounds(const char* str, int32_t x, int32_t y, int32_t* x1, int32_t* y1, uint32_t* w, uint32_t* h)
{
	uint8_t c; // Current character
	int32_t minx = 0x7FFFFFFF, miny = 0x7FFFFFFF, maxx = -1, maxy = -1; // Bound rect
	// Bound rect is intentionally initialized inverted, so 1st char sets it

	*x1 = x; // Initial position is value passed in
	*y1 = y;
	*w = *h = 0; // Initial size is zero

	while ((c = *str++)) {
		// charBounds() modifies x/y to advance for each character,
		// and min/max x/y are updated to incrementally build bounding rect.
		charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
	}

	if (maxx >= minx) {     // If legit string bounds were found...
		*x1 = minx;           // Update x1 to least X coord,
		*w = maxx - minx + 1; // And w to bound rect width
	}
	if (maxy >= miny) { // Same for height
		*y1 = miny;
		*h = maxy - miny + 1;
	}

	return 0;
}

int font::drawChar(canvas* ptr, unsigned char c, int32_t x0, int32_t y0)
{
	if (myFont != nullptr)
	{
		c -= (uint8_t)myFont->first;
		GFXglyph* glyph = &myFont->glyph[c];
		uint8_t* bitmap = myFont->bitmap;

		uint32_t bo = glyph->bitmapOffset;
		uint32_t w = glyph->width;
		uint32_t h = glyph->height;
		int32_t xo = glyph->xOffset;
		int32_t yo = glyph->yOffset;

		uint32_t xx, yy;
		uint8_t bits = 0, bit = 0;
		int32_t xo16 = 0, yo16 = 0;

		for (yy = 0; yy < h; yy++) {
			for (xx = 0; xx < w; xx++) {
				if (!(bit++ & 7)) {
					bits = bitmap[bo++];
				}

				if (bits & 0x80) {
					ptr->setPixle((x0 + xo + xx), (y0 + yo + yy), 1);
				}

				bits <<= 1;
			}
		}
	}

	return 0;
}

int font::write(canvas* ptr, unsigned char c, int32_t* cursor_x, int32_t* cursor_y, int32_t x_offset)
{
	if (c == '\n')
	{
		*cursor_x = x_offset + 0;
		*cursor_y += (uint32_t)myFont->yAdvance;
	}
	else if (c != '\r')
	{
		uint8_t first = myFont->first;
		if ((c >= first) && (c <= (uint8_t)myFont->last))
		{
			if (myFont->glyph != NULL)
			{
				GFXglyph* glyph = &myFont->glyph[c - first];

				uint32_t w = glyph->width;
				uint32_t h = glyph->height;

				if ((w > 0) && (h > 0))
				{
					int32_t xo = glyph->xOffset;

					/* fix at some point */
					//if (true && ((*cursor_x + (xo + w)) > (uint32_t)ptr->get_x()))
					//{
					//	*cursor_x = 0;
					//	*cursor_y += myFont->yAdvance;
					//}
					
					drawChar(ptr, c, *cursor_x, *cursor_y);
				}
				*cursor_x += (uint32_t)glyph->xAdvance;
			}
		}
	}
	return 0;
}

int font::print(canvas* myCanvas, const char* str, int32_t x0, int32_t y0, int32_t x_offset)
{
	size_t index = 0;
	size_t n = strlen(str);
	while (n--)
	{
		if (!write(myCanvas, str[index], &x0, &y0, x_offset))
			index++;
		else
			break;
	}

	return 0;
}

font::~font()
{
	if (myFont != nullptr)
	{
		if(myFont->bitmap != NULL)
			delete[] myFont->bitmap;
		if (myFont->glyph != NULL)
			delete[] myFont->glyph;
		delete myFont;
	}
}

void font::enbit(uint8_t value, std::vector<uint8_t>* bmp)
{
	static uint8_t row = 0, sum = 0, bit = 0x80, firstCall = 1;

	if (value) // Set bit if needed
		sum |= bit;

	if (!(bit >>= 1)) // Advance to next bit, end of byte reached?
	{
		bmp->push_back(sum);
		sum = 0;				// Clear for next byte
		bit = 0x80;				// Reset bit counter
	}

	return;
}
