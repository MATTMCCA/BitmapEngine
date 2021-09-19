
#include "font-mono.h"

int grow_DFB(DFB* ptr, int concat)
{
	if (ptr->ptr == NULL) {
		ptr->ptr = malloc(sizeof(uint8_t));
		if (ptr->ptr == NULL) {
			printf("GROW_DFB error\n");
			return 1;
		}
	}

	if (concat > 0) {
		uint8_t* tmp = realloc(ptr->ptr, (ptr->_size + concat) * sizeof(uint8_t));
		if (tmp != NULL) {
			ptr->_size += concat;
			ptr->ptr = tmp;
		}
		else {
			printf("GROW_DFB err\n");
			return 1;
		}
	}
	return 0;
}

int free_DFB(DFB* ptr)
{
	if (ptr->ptr != NULL)
		free(ptr->ptr);
	ptr->_size = 0;
	return 0;
}

int enbit(uint8_t value, DFB* ptr) 
{
	static uint8_t row = 0, sum = 0, bit = 0x80, firstCall = 1;

	if (value) // Set bit if needed
		sum |= bit;

	if (!(bit >>= 1)) // Advance to next bit, end of byte reached?
	{
		if (!grow_DFB(ptr, 1)) {
			ptr->ptr[ptr->_size - 1] = sum;
			sum = 0;				// Clear for next byte
			bit = 0x80;				// Reset bit counter
		}
		else {
			printf("enbit Error\n");
			return 1;
		}
	}

	return 0;
}

int free_font(GFXfont* font)
{
	if (font->glyph != NULL)
		free(font->glyph);

	if (font->bitmap != NULL)
		free(font->bitmap);

	font->bitmap = NULL;
	font->first = 0;
	font->glyph = NULL;
	font->last = 0;
	font->yAdvance = 0;

	return 0;
}

//if this function return with an error, mem never gets freed
int create_font(GFXfont* font, const char* fontName, int fontPoint, int dpi)
{
	int					i, j, err, byte;
	int					first = ' ';
	int					last = '~';
	int					bitmapOffset = 0;
	unsigned int		x, y;

	FT_Library			library;
	FT_Face				face;
	FT_Glyph			glyph;
	FT_Bitmap* bitmap;
	FT_BitmapGlyphRec* g;
	uint8_t				bit;
	DFB BITMAP = { NULL };

	free_font(font); //clear out struct

	if (font->glyph == NULL)
		font->glyph = (GFXglyph*)malloc((last - first + 1) * sizeof(GFXglyph));

	if (font->glyph == NULL) {
		fprintf(stderr, "could not malloc glyph\n");
		return 1;
	}

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
	FT_Set_Char_Size(face, fontPoint << 6, 0, dpi, 0);

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

		font->glyph[j].bitmapOffset = bitmapOffset;
		font->glyph[j].width = bitmap->width;
		font->glyph[j].height = bitmap->rows;
		font->glyph[j].xAdvance = (uint8_t)(face->glyph->advance.x >> 6);
		font->glyph[j].xOffset = g->left;
		font->glyph[j].yOffset = 1 - g->top;

		for (y = 0; y < bitmap->rows; y++)
		{
			for (x = 0; x < bitmap->width; x++)
			{
				byte = x / 8;
				bit = 0x80 >> (x & 7);
				if (enbit(bitmap->buffer[y * bitmap->pitch + byte] & bit, &BITMAP)) {
					fprintf(stderr, "Memory Error\n");
					return 1;
				}
			}
		}

		// Pad end of char bitmap to next byte boundary if needed
		int n = (bitmap->width * bitmap->rows) & 7;
		if (n) // Pixel count not an even multiple of 8?
		{
			n = 8 - n; // # bits to next multiple
			while (n--)
			{
				if (enbit(0, &BITMAP)) {
					fprintf(stderr, "Memory Error\n");
					return 1;
				}
			}
		}
		bitmapOffset += (bitmap->width * bitmap->rows + 7) / 8;
		FT_Done_Glyph(glyph);
	}

	font->bitmap = BITMAP.ptr;
	font->first = first;
	font->last = last;

	if (face->size->metrics.height == 0) {
		font->yAdvance = font->glyph[0].height;
	}
	else {
		font->yAdvance = (uint8_t)(face->size->metrics.height >> 6);
	}

	FT_Done_FreeType(library);

	return 0;
}