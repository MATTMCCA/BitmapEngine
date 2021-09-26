/*
	***************************************************************************************
	MJM -2021-

	---------------------------------------------------------------------------------------

	=======================================================================================

		Notes: Very little error handling, its a demo?

	=======================================================================================

	Copyright <2021> <Matthew McCardle>
	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation the rights to use, copy,
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so, subject to the
	following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	***************************************************************************************
*/

#define _CRT_SECURE_NO_DEPRECATE

#include <string>

#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include "canvas.hpp"
#include "font.hpp"

using namespace std::chrono;

//const char ARIAL_FONT[] = "C:\\Windows\\Fonts\\arial.ttf";
//const char ARIAL_FONT_BOLD[] = "C:\\Windows\\Fonts\\arialbd.ttf";
//const char TEST_FILE[] = "A:\\Users\\Matt\\Pictures\\TRASH\\static_test\\test.csv";

const char TEST_DIR[] = "A:\\Users\\Matt\\Pictures\\TRASH\\libtest\\";
const char TEST_IMG[] = "A:\\Users\\Matt\\Pictures\\TRASH\\Lena.bmp";
const int DPI = 300;

bool save_bmp(canvas* ptr, const char* dir, int cnt);
void print_pass_fail(const char* testname, bool err);

bool canvas_noninv(const char* dir, int cnt);
bool canvas_inv(const char* dir, int cnt);
bool convas_fill(const char* dir, int cnt);
bool canvas_setPixle(const char* dir, int cnt);
bool canvas_setPixle_rand(const char* dir, int cnt);
bool canvas_Hline(const char* dir, int cnt);
bool canvas_Vline(const char* dir, int cnt);
bool canvas_box(const char* dir, int cnt);
bool canvas_box_fill(const char* dir, int cnt);
bool canvas_box_fill_inv(const char* dir, int cnt);

int main(int argc, char* argv[])
{
	char* output_dir = (char*)TEST_DIR;
	char* tst_img = (char*)TEST_IMG;
	auto start = high_resolution_clock::now();

	if (argc > 2) {
		tst_img = argv[1];
		output_dir = argv[2];
	}
	printf("output_dir: %s\n", output_dir); //debugging
	printf("test_image: %s\n", tst_img); //debugging

	/* Running Tests */
	int tc = 0;

	canvas_noninv(output_dir, tc++);
	canvas_inv(output_dir, tc++);
	convas_fill(output_dir, tc++);
	canvas_setPixle(output_dir, tc++);
	canvas_setPixle_rand(output_dir, tc++);
	canvas_Hline(output_dir, tc++);
	canvas_Vline(output_dir, tc++);
	canvas_box(output_dir, tc++);
	canvas_box_fill(output_dir, tc++);
	canvas_box_fill_inv(output_dir, tc++);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("Took: %I64d milliseconds\n", duration.count());

	return 0;
}

bool save_bmp(canvas* ptr, const char* dir, int cnt)
{
	std::string output = std::string(dir) + std::to_string(cnt) + std::string(".bmp");
	return ptr->save(output.c_str(), DPI);
}

void print_pass_fail(const char* testname, bool err)
{
	if (err)
		printf("FAILED:\t%s\n", testname);
	else
		printf("PASSED:\t%s\n", testname);
}

bool canvas_noninv(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);
	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (non inverted)", err);
	return err;
}

bool canvas_inv(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 1);
	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (inverted)", err);
	return err;
}

bool convas_fill(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);
	err |= c.fill(1);
	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (fill)", err);
	return err;
}

bool canvas_setPixle(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);
	
	bool flop = 0;
	for (int y = 0; y < 200; y++) {
		for (int x = 0; x < 200; x++)
			err |= c.setPixle(x, y, (flop = !flop));
		flop = !flop;
	}

	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (setPixle)", err);
	return err;
}

bool canvas_setPixle_rand(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);

	for (int i = 0; i < 500; i++) {
		err |= c.setPixle(rand() % 200, rand() % 200, 1);
	}

	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (setPixle_random)", err);
	return err;
}

bool canvas_Hline(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);

	for (int i = 0; i < 10; i++) {
		err |= c.drawHorizontalLine(rand() % 200, rand() % 200, rand() % 200, rand() % 20, 1);
	}

	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (drawHorizontalLine)", err);
	return err;
}

bool canvas_Vline(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);

	for (int i = 0; i < 10; i++) {
		err |= c.drawVerticalLine(rand() % 200, rand() % 200, rand() % 200, rand() % 20, 1);
	}

	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (drawVerticalLine)", err);
	return err;
}

bool canvas_box(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);

	for (int i = 1; i < 10; i ++) {
		int h = i * 5;
		err |= c.drawBox(h, h, 200 - h*4, 200 - h*4, 2, 1);
	}

	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (box)", err);
	return err;
}

bool canvas_box_fill(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);

	for (int i = 1; i < 20; i++) 
		err |= c.drawBoxFill(i*4, i*4, 20, 20, i%2);

	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (box_fill)", err);
	return err;
}

bool canvas_box_fill_inv(const char* dir, int cnt)
{
	bool err = 0;
	canvas c;
	err |= c.create(200, 200, 0);

	for (int i = 1; i < 20; i++)
		err |= c.drawBoxFill(i * 4, i * 4, 20, 20, i % 2);

	err |= c.invert(1);
	err |= save_bmp(&c, dir, cnt);
	print_pass_fail("Canvas (box_fill_invert)", err);
	return err;
}