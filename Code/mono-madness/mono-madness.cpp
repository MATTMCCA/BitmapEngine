/*

	TEST
	MJM
	2021

	its not perfect!

*/

#define _CRT_SECURE_NO_DEPRECATE

#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include "canvas.h"
#include "font.h"


char filename[500] = { 0x00 };
const char TRASH_DIR[] = "A:\\Users\\Matt\\Pictures\\TRASH\\";
const int DPI = 300;

void build_demo_outline(canvas* mstr);
void add_some_text(canvas* mstr);
void add_rnd_text(canvas* mstr);
void add_checkerboard(canvas* mstr);
void add_time(canvas* mstr);

void add_image(canvas* mstr);


int main(int argc, char* argv[]) 
{
	printf("Test Begin\n");

	canvas master;
	master.create(1200, 2400, 0);	//canvas 1200x2400, non-inverted

	//------------------------------------
	add_image(&master);
	build_demo_outline(&master);
	add_some_text(&master);
	add_rnd_text(&master);
	add_checkerboard(&master);
	add_time(&master);
	//------------------------------------

	strcat_s(filename, 500, TRASH_DIR);
	strcat_s(filename, 500, "test_123.bmp");
	master.save(filename, DPI);
	printf("Test End\n");
	return 0;
}


void build_demo_outline(canvas* mstr)
{
	mstr->drawBox(0 + 10, 0 + 10, 2400 - 20, 1200 - 20, 10, 1);	//draw box
	mstr->drawHorizontalLine(0 + 10, 0 + 200, 1200 - 20, 10, 1);	//draw line
}


void add_some_text(canvas* mstr)
{
	font arial_30pt;
	arial_30pt.create("C:\\Windows\\Fonts\\arial.ttf", 30, DPI);

	canvas top_text;
	arial_30pt.writeCanvas(&top_text, "! TOP OF BMP !");

	mstr->addSprite(&top_text, (mstr->get_x() - top_text.get_x()) / 2 , 50, 1);
}

void add_rnd_text(canvas* mstr)
{
	font DejaVuSerif_20pt;
	DejaVuSerif_20pt.create("C:\\Windows\\Fonts\\DejaVuSerif.ttf", 20, DPI);

	int i, j;
	char buffer[(20 * 21) + 1] = { 0x00 };
	
	for (i = 0; i < 20; i++) 
		for (j = 0; j < 21; j++) 
			buffer[(i * 20) + j] = (rand() % 95) + 32;
		
	for (j = 0; j < (21 * 20); j += 20) {
		if(j != 0)
			buffer[j] = '\n';
	}

	
	//canvas rnd_text;
	//DejaVuSerif_20pt.writeCanvas(&rnd_text, buffer);
	//mstr->addSprite(&rnd_text, 35, 220, 1);

	DejaVuSerif_20pt.writeCanvas(mstr, buffer, 35, 220);
}

void add_checkerboard(canvas* mstr)
{
	canvas checkerboard;
	checkerboard.create(300, 340, 0);

	int x, y;
	bool tog = 0;
	for (y = 0; y < 340; y += 10) 
	{
		for (x = 0; x < 300; x += 10)
		{
			checkerboard.drawBoxFill(x, y, 10, 10, tog);
			tog = !tog;
		}
		tog = !tog;
	}
	
	mstr->addSprite(&checkerboard, 800, 2000, 1);
}

void add_time(canvas* mstr)
{
	time_t current_time;
	char* c_time_string;
	current_time = time(NULL);
	c_time_string = ctime(&current_time);

	font consola_10pt;
	consola_10pt.create("C:\\Windows\\Fonts\\consola.ttf", 10, DPI);

	canvas time_date;
	consola_10pt.writeCanvas(&time_date, c_time_string);
	time_date.invert(1);

	mstr->addSprite(&time_date, 40, 2300, 0);
}


void add_image(canvas* mstr)
{
	/* create canvas */
	canvas g24_test;
	/* add 24-bit color image to canvas with stucki dithering */
	g24_test.import_24bit("A:\\Users\\Matt\\Pictures\\TRASH\\tst.bmp", _Stucki);
	/* overlay onto master canvas */
	mstr->addSprite(&g24_test, 0, 0, 0);
}