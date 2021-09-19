#include "font.h"

font::font()
{
}

int font::create(const char* fontName, int point, int dpi)
{
	int					i, j, err, byte;
	int					first = ' ';
	int					last = '~';
	int					bitmapOffset = 0;
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
		myFont->glyph[j].xAdvance = (uint8_t)(face->glyph->advance.x >> 6);
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

	size_t h = BITMAP.size();
	myFont->bitmap = new uint8_t[h] {0x00};

	for (size_t i = 0; i < h; i++)
		myFont->bitmap[i] = BITMAP[i];

	myFont->first = first;
	myFont->last = last;

	if (face->size->metrics.height == 0) {
		myFont->yAdvance = myFont->glyph[0].height;
	}
	else {
		myFont->yAdvance = (uint8_t)(face->size->metrics.height >> 6);
	}

	FT_Done_FreeType(library);

	return 0;
}

GFXfont* font::getGFXfont(void)
{
	return myFont;
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
