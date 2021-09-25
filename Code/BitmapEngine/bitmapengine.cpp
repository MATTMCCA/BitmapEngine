/*
	TEST
	MJM
	2021

	very little error handling..... needs fixed
*/

#define _CRT_SECURE_NO_DEPRECATE

//#include <iostream>

#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include "canvas.hpp"
#include "font.hpp"

/* CVS Parser, grabbed from https://github.com/AriaFallah/csv-parser */
#include "parser.hpp"

using namespace aria::csv;

const char ARIAL_FONT[] =		"C:\\Windows\\Fonts\\arial.ttf";
const char ARIAL_FONT_BOLD[] =  "C:\\Windows\\Fonts\\arialbd.ttf";
const char TEST_FILE[] =		"A:\\Users\\Matt\\Pictures\\TRASH\\static_test\\test.csv";
const int DPI = 300;

typedef struct
{
	const char*		text_field_001; //0
	const char*     text_field_002; //1
	const char*		text_field_003; //2
	const char*		text_field_004; //3
	const char*		text_field_005; //4
	const char*		text_field_006; //5
	const char*		text_field_007; //6
	const char*		text_field_008; //7
	const char*		text_field_009; //8
	const char*		text_field_010; //9
	const char*     graphic_001;    //10
	const char*     graphic_002;    //11
	const char*     graphic_003;    //12
	const char*		save_as;        //13

}static_format_struct;

int static_test(CsvParser* CSVptr);
int build_format(canvas* master);
int add_dynamic_data(static_format_struct* data, canvas* ptr);

void get_date(char* buf, size_t len);
void get_tracking(char* buf, size_t len);

int main(int argc, char* argv[]) 
{
	char* inputFile = (char*) TEST_FILE;

	if (argc > 1) {
		inputFile = argv[1];
	}
	printf("arg: %s\n", inputFile); //debugging

	std::ifstream f(inputFile);
	CsvParser parser(f);

	static_test(&parser);

	return 0;
}

void get_date(char* buf, size_t len)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	snprintf(buf, len, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void get_tracking(char* buf, size_t len)
{
	snprintf(buf, len, "%04d %04d %04d %04d %02d", 
		rand() % 9999, rand() % 9999, 
		rand() % 9999, rand() % 9999, 
		rand() % 99);
}

int static_test(CsvParser* CSVptr)
{
	int cnt = 0;
	canvas* temp;
	canvas my_format;
	build_format(&my_format);

	for (auto& row : *CSVptr) 
	{
		std::vector<std::string> _data;
		for (auto& field : row) {
			_data.push_back(field);
		}

		temp = new canvas;
		*temp = my_format;

		/****************************************************/
		static_format_struct fmt = 
		{ 
			_data[0].c_str(),
			_data[1].c_str(),
			_data[2].c_str(),
			_data[3].c_str(),
			_data[4].c_str(),
			_data[5].c_str(),
			_data[6].c_str(),
			_data[7].c_str(),
			_data[8].c_str(),
			_data[9].c_str(),
			_data[10].c_str(),
			_data[11].c_str(),
			_data[12].c_str(),
			_data[13].c_str(),
		};
		/****************************************************/

		add_dynamic_data(&fmt, temp);
		delete temp;

		//printf("parsed: %d\n", cnt++);
	}

	return 0;
}

int build_format(canvas* master)
{
	font arial_bold_70;
	font arial_bold_20;
	font arial_bold_10;
	font arial_bold_12;

	arial_bold_70.create(ARIAL_FONT_BOLD, 70, DPI);
	arial_bold_20.create(ARIAL_FONT_BOLD, 20, DPI);
	arial_bold_10.create(ARIAL_FONT_BOLD, 10, DPI);
	arial_bold_12.create(ARIAL_FONT_BOLD, 12, DPI);

	/* solid's outline */
	master->create(1275, 2025, 0);
	master->drawBox(60,  100, 1830, 1170, 10, 1);			//box 1
	master->drawBox(60,  100,  300,  300, 10, 1);			//box 2
	master->drawBox(350, 100,  300,  880, 10, 1);			//box 3
	master->drawHorizontalLine(60,  520, 1170, 10, 1);		//line 1
	master->drawHorizontalLine(60, 1300, 1170, 20, 1);		//line 2
	master->drawHorizontalLine(60, 1800, 1170, 20, 1);		//line 3
	master->drawBox(850, 800, 70, 160, 5, 1);				//box 4


	/* static text */
	arial_bold_10.writeCanvas(master, "TM",                 1060, 430);
	arial_bold_10.writeCanvas(master, "SHIP\nTO:",            90, 950);
	arial_bold_10.writeCanvas(master, "C001",                878, 819);
	arial_bold_12.writeCanvas(master, "USPS TRACKING #",     400, 1350);
	arial_bold_12.writeCanvas(master, "0023",                1040, 564);
	arial_bold_20.writeCanvas(master, "PRIORITY MAIL 2-DAY", 160, 430);
	arial_bold_70.writeCanvas(master, "P",                   130, 140);

	/* Font Test */
	font arial_2pt;
	font arial_4pt;
	font arial_6pt;
	font arial_8pt;

	arial_2pt.create(ARIAL_FONT, 2, DPI);
	arial_4pt.create(ARIAL_FONT, 4, DPI);
	arial_6pt.create(ARIAL_FONT, 6, DPI);
	arial_8pt.create(ARIAL_FONT, 8, DPI);

	arial_2pt.writeCanvas(master, "2 POINT FONT, 2 point font", 90, 1830);
	arial_4pt.writeCanvas(master, "4 POINT FONT, 4 point font", 90, 1840);
	arial_6pt.writeCanvas(master, "6 POINT FONT, 6 point font", 90, 1860);
	arial_8pt.writeCanvas(master, "8 POINT FONT, 8 point font", 90, 1888);



	return 0;
}

int add_dynamic_data(static_format_struct* data, canvas* ptr)
{
	char char_buffer[100];
	
	font arial_bold_6;
	font arial_bold_8;
	font arial_bold_12;
	font arial_bold_16;

	arial_bold_6.create(ARIAL_FONT_BOLD, 6, DPI);
	arial_bold_8.create(ARIAL_FONT_BOLD, 8, DPI);
	arial_bold_12.create(ARIAL_FONT_BOLD, 12, DPI);
	arial_bold_16.create(ARIAL_FONT_BOLD, 16, DPI);

	/**************************************************************************/
	arial_bold_6.writeCanvas(ptr, data->text_field_001, 390, 120);
	arial_bold_6.writeCanvas(ptr, data->text_field_002, 390, 120 + 25);
	arial_bold_6.writeCanvas(ptr, data->text_field_003, 390, 120 + 50);
	arial_bold_6.writeCanvas(ptr, data->text_field_004, 390, 120 + 75);
	/**************************************************************************/
	get_date(char_buffer, 100);
	arial_bold_6.writeCanvas(ptr, char_buffer, 1066, 350);	//date
	/**************************************************************************/
	get_tracking(char_buffer, 100);
	arial_bold_12.writeCanvas(ptr, char_buffer, 350, 1730); //tracking
	/**************************************************************************/
	arial_bold_8.writeCanvas(ptr, data->text_field_005, 90, 550);
	arial_bold_8.writeCanvas(ptr, data->text_field_006, 90, 550 + 40);
	arial_bold_8.writeCanvas(ptr, data->text_field_007, 90, 550 + 80);
	/**************************************************************************/
	arial_bold_16.writeCanvas(ptr, data->text_field_008, 250, 950);
	arial_bold_16.writeCanvas(ptr, data->text_field_009, 250, 950 + 70);
	arial_bold_16.writeCanvas(ptr, data->text_field_010, 250, 950 + 140);

	/************************** Add grphic ************************************/
	canvas *graphic;
	graphic = new canvas;
	graphic->import_24bit(data->graphic_001, DITHER::Threshold);
	ptr->addSprite(graphic, 390, 230, 0);
	delete graphic;
	/**************************************************************************/
	graphic = new canvas;
	graphic->import_24bit(data->graphic_002, DITHER::Jarvis);
	ptr->addSprite(graphic, 85, 1400, 0);
	delete graphic;
	/**************************************************************************/
	graphic = new canvas;
	graphic->import_24bit(data->graphic_003, DITHER::Threshold);
	ptr->addSprite(graphic, 1090, 240, 0);
	delete graphic;
	/**************************************************************************/


	ptr->save(data->save_as, DPI);
	return 0;
}
