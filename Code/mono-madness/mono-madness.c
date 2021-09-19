/*
TrueType to Adafruit_GFX font converter.  Derived from Peter Jakobs'
Adafruit_ftGFX fork & makefont tool, and Paul Kourany's Adafruit_mfGFX.

NOT AN ARDUINO SKETCH.  This is a command-line tool for preprocessing
fonts to be used with the Adafruit_GFX Arduino library.

For UNIX-like systems.  Outputs to stdout; redirect to header file, e.g.:
  ./fontconvert ~/Library/Fonts/FreeSans.ttf 18 > FreeSans18pt7b.h

REQUIRES FREETYPE LIBRARY.  www.freetype.org

Currently this only extracts the printable 7-bit ASCII chars of a font.
Will eventually extend with some int'l chars a la ftGFX, not there yet.
Keep 7-bit fonts around as an option in that case, more compact.

See notes at end for glyph nomenclature & other tidbits.
*/

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>


#include "bitmap-mono.h"



int main(int argc, char* argv[]) {

	const char TRASH_DIR[] = "A:\\Users\\Matt\\Pictures\\TRASH\\";
	char filename[500] = { 0x00 };
	strcat_s(filename, 500, TRASH_DIR);
	strcat_s(filename, 500, "char_test.bmp");

	/*
	const char FONT_NAME[] = "C:\\Windows\\Fonts\\consola.ttf";
	const int FONT_POINT = 20;
	const int DPI = 300;
	*/

	font_att myFont = { "C:\\Windows\\Fonts\\consola.ttf", 20, 300 };


	/* test */
	/*
	GFXfont test_font = {NULL};
	if (create_font(&test_font, FONT_NAME, FONT_POINT, DPI))
		printf("hmm\n");

	//do stuff
	printf("breakpoint here\n");

	free_font(&test_font);

	*/

	canvas myTest = { NULL };
	if (createBMP(&myTest, 500, 500))
		return 1;

	/*
	if (drawChar(&myTest, &myFont, 'M', 100, 100))
		return 1;
	*/

	if (print(&myTest, &myFont, "moo\nboo", 0, 100))
		return 1;

	if (saveCanvas(&myTest, 300 /*DPI*/, filename))
		return 1;

	freeBMP(&myTest);
	return 0;
}

