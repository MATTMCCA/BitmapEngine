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

int main(int argc, char* argv[]) 
{
	printf("Test Begin\n");

	canvas master;
	master.create(1200, 2400, 0);	//canvas 1200x2400, non-inverted

	build_demo_outline(&master);
	add_some_text(&master);
	add_rnd_text(&master);
	add_checkerboard(&master);
	add_time(&master);

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

	char buffer[(20 * 21) + 1] = { 0x00 };

	for (int i = 0; i < 20; i++) 
		for (int j = 0; j < 21; j++) 
			buffer[(i * 20) + j] = (rand() % 95) + 32;
		
	for (int j = 0; j < (21 * 20); j+=20) 
		buffer[j] = '\n';

	canvas rnd_text;
	DejaVuSerif_20pt.writeCanvas(&rnd_text, buffer);

	mstr->addSprite(&rnd_text, 30, 220, 1);
}

void add_checkerboard(canvas* mstr)
{
	canvas checkerboard;
	checkerboard.create(300, 340, 0);

	bool tog = 0;
	for (int y = 0; y < 340; y += 10) 
	{
		for (int x = 0; x < 300; x += 10)
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

	mstr->addSprite(&time_date, 40, 2300, 1);
}