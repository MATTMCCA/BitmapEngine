#include <cstdio>
#include <cstdint>
#include <cctype>

#include "canvas.h"
#include "font.h"


int main(int argc, char* argv[]) {

	printf("test\n");


	const char TRASH_DIR[] = "A:\\Users\\Matt\\Pictures\\TRASH\\";
	char filename[500] = { 0x00 };
	strcat_s(filename, 500, TRASH_DIR);
	strcat_s(filename, 500, "char_test.bmp");

	/*
	canvas mycan;
	if (mycan.create(300, 300, 1))
		return 1;

	mycan.setPixle(5, 5, 1);

	if (mycan.save(filename, 300))
		return 1;
	*/

	font consola_20point;
	consola_20point.create("C:\\Windows\\Fonts\\consola.ttf", 100, 300);

	canvas test;
	consola_20point.writeCanvas(&test, "HOT SHIT! DID THAT WORK!!!?\nGuess so, how about this?");
	test.save(filename, 300);


	/*
	int32_t x, y;
	uint32_t w, h;

	consola_20point.getTextBounds("test", 0, 0, &x, &y, &w, &h);

	canvas mycan;
	if (mycan.create(w, h, 0))
		return 1;

	if (mycan.save(filename, 300))
		return 1;
	*/


	return 0;
}