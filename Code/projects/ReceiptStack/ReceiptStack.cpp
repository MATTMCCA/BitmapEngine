/*
    ***************************************************************************************
    MJM -2021-

    This is a Sample application, designed to demonstrate the
    graphical capability of the bitmap engine.

    The chosen label format is a receipt format.

    ---------------------------------------------------------------------------------------

    This example takes a csv file as an argument.
        [ ReceoptStack.exe mycsv.csv ]

    =======================================================================================

    File:ReceiptStack.cpp

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

#include <chrono>

#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include "canvas.hpp"
#include "font.hpp"

#include "parser.hpp"           /* https://github.com/AriaFallah/csv-parser */

using namespace std::chrono;
using namespace aria::csv;

const char ARIAL_FONT[] = "C:\\Windows\\Fonts\\arial.ttf";
const char ARIAL_FONT_BOLD[] = "C:\\Windows\\Fonts\\arialbd.ttf";

const char TEST_FILE[] = "A:\\Users\\Matt\\Pictures\\TRASH\\rst\\test.csv";
const char OUTPUT[] = "A:\\Users\\Matt\\Pictures\\TRASH\\rst\\receipt.bmp";

const int DPI = 300;


//int static_test(CsvParser* CSVptr);

//int build_format(canvas* master);
//int add_dynamic_data(static_format_struct* data, canvas* ptr);

void get_date(char* buf, size_t len);

void build_head(canvas* ptr);
void build_foot(canvas* ptr);

void read_in_data(CsvParser* CSVptr, std::vector<std::string>* str);

uint32_t calc_length(std::vector<canvas>* ptr);
bool build_body(canvas* ptr, std::vector<std::string>* str);



int main(int argc, char* argv[])
{
    auto start = high_resolution_clock::now();

    char* inputFile = (char*)TEST_FILE;
    char* output_dir = (char*)OUTPUT;

    if (argc > 2) {
        inputFile = argv[1];
        output_dir = argv[2];
    }
    printf("csv: %s\n", inputFile);         //debugging
    printf("output_dir: %s\n", output_dir); //debugging

    std::ifstream f(inputFile);
    CsvParser parser(f);

    std::vector<std::string> my_data;
    read_in_data(&parser, &my_data);        //read in csv strings from test file

    canvas HEAD;
    canvas BODY;
    canvas FOOT;

    build_head(&HEAD);
    build_body(&BODY, &my_data);
    build_foot(&FOOT);

    canvas TOTAL;
    TOTAL.create(944, HEAD.get_y() + BODY.get_y() + FOOT.get_y(), 0);
    TOTAL.addSprite(&HEAD, 0, 0, 1);
    TOTAL.addSprite(&BODY, 0, HEAD.get_y(), 1);
    TOTAL.addSprite(&FOOT, 0, HEAD.get_y() + BODY.get_y(), 1);

    TOTAL.save(output_dir, DPI);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    printf("Took: %I64d milliseconds\n", duration.count());

    return 0;
}

void get_date(char* buf, size_t len)
{
    time_t t = time(NULL);
    char* tm = ctime(&t);
    snprintf(buf, len, "%s", tm);
    printf("%s\n", buf);
}


void read_in_data(CsvParser* CSVptr, std::vector<std::string>* str)
{
    for (auto& row : *CSVptr) {
        std::vector<std::string> _data;
        for (auto& field : row) {
            str->push_back(field);
        }
    }
}

//944 pix = 80mm @ 300dpi
bool build_body(canvas* ptr, std::vector<std::string>* str)
{
    font f;
    f.create(ARIAL_FONT, 8, DPI);

    std::vector<canvas> canvas_data;
    canvas_data.resize(str->size());

    for (uint32_t i = 0; i < str->size(); i++) {
        f.writeCanvas(&canvas_data[i], str->at(i).c_str(), 0, 0);
    }

    int virt_off = 10;
    uint32_t t_len = calc_length(&canvas_data);
    ptr->create(944, (virt_off * str->size()) + t_len, 0);

    uint32_t _v_off = 0;
    for (uint32_t i = 0; i < str->size(); i++) {
        ptr->addSprite(&canvas_data[i], 50, _v_off, 1);
        _v_off += (canvas_data[i].get_y() + virt_off);
    }

    return 0;
}

uint32_t calc_length(std::vector<canvas>* ptr)
{
    uint32_t len = 0;
    for (uint32_t i = 0; i < ptr->size(); i++) {
        len += ptr->at(i).get_y();
    }
    return len;
}

//944 pix = 80mm @ 300dpi
void build_head(canvas* ptr)
{
    font f1, f2, f3;
    ptr->create(944, 400, 0);

    f1.create(ARIAL_FONT_BOLD, 10, DPI);
    f2.create(ARIAL_FONT, 10, DPI);
    f3.create(ARIAL_FONT, 8, DPI);

    f1.writeCanvas(ptr, "Dirty Dan's", 350, 20);
    f1.writeCanvas(ptr, "  Pizza", 350, 60);

    f2.writeCanvas(ptr, "123 Smelly Way", 300, 110);
    f2.writeCanvas(ptr, "(555) 555-5555", 300, 150);

    char char_buffer[100];
    get_date(char_buffer, 100);

    f3.writeCanvas(ptr, "Order: 1234", 50, 260);
    f3.writeCanvas(ptr, char_buffer, 500, 260);
}

//944 pix = 80mm @ 300dpi
void build_foot(canvas* ptr)
{
    font f1, f2;
    ptr->create(944, 400, 0);

    f1.create(ARIAL_FONT_BOLD, 10, DPI);
    f2.create(ARIAL_FONT, 10, DPI);

    f1.writeCanvas(ptr, "Thanks for Stopping", 250, 60);
    f2.writeCanvas(ptr, "E-mail: DirtyDan@pieinthesky.com", 50, 150);
}
