/*
    ***************************************************************************************
    MJM -2021-

    ---------------------------------------------------------------------------------------

    =======================================================================================

        Notes: 
        
        DUDE! https://www.youtube.com/watch?v=Th83z2ddz9c
        https://www.thresholdsecurity.com/products/tab_expiring_direct_thermal_badges/
        3"x2" = 2.8125" x 1.9375"
        @ 300DPI, 843 x 581

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

#include <string>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include <zint.h>

#include "canvas.hpp"
#include "font.hpp"

#define FONT_ARIAL      "C:\\Windows\\Fonts\\arial.ttf"
#define SAVE_AS         "A:\\Users\\Matt\\Pictures\\TRASH\\ID\\ID.bmp"
#define LOGO            "A:\\Users\\Matt\\Pictures\\TRASH\\logo.bmp"
#define SUBJECT         "A:\\Users\\Matt\\Pictures\\TRASH\\girl.bmp"

const int DPI = 300;    //used for bitmap

int add_barcode_to_canvas(canvas* can, zint_symbol* my_symbol);

int getPDF417(canvas* can, const char* str);
int getSubject(canvas* can, const char* str, int brightness, int contrast);
int getLogo(canvas* can, const char* str);
int getTXT_A(canvas* can, const char* str);
int getTXT_B(canvas* can, const char* str);
int getTime(canvas* can);
int getDate(canvas* can);

int main(int argc, char* argv[])
{
    canvas* temp;
    canvas master;

    const char* _save_as = SAVE_AS;
    const char* _logo = LOGO;
    const char* _subject = SUBJECT;
    const char* _name = "Martha White";

    if (argc > 4) {
        _logo = argv[1];
        _subject = argv[2];
        _name = argv[3];
        _save_as = argv[4];
    }
    else {
        printf("Using Debug Paths [logo] [subject] [name] [save_as]\n");
    }
    
    if (master.create(843, 581) == 1)                return 1;

    temp = new canvas;
    if (getSubject(temp, _subject, 0, 0) == 1)       return 1;
    if (master.addSprite(temp, 10, 200, 0) == 1)     return 1;
    delete temp;

    temp = new canvas;
    if (getLogo(temp, _logo) == 1)                   return 1;
    if (master.addSprite(temp, 75, 20, 0) == 1)      return 1;
    delete temp;

    temp = new canvas;
    if (getPDF417(temp, _name) == 1)                 return 1;
    if (master.addSprite(temp, 500, 500, 0) == 1)    return 1;
    delete temp;

    temp = new canvas;
    if (getTXT_B(temp, _name) == 1)                  return 1;
    if (master.addSprite(temp, 380, 160, 0) == 1)    return 1;
    delete temp;

    temp = new canvas;
    if (getDate(temp) == 1)                          return 1;
    if (master.addSprite(temp, 500, 350, 0) == 1)    return 1;
    delete temp;

    temp = new canvas;
    if (getTime(temp) == 1)                          return 1;
    if (master.addSprite(temp, 520, 420, 0) == 1)    return 1;
    delete temp;

    if (master.saveBMP(_save_as, DPI) == 1)          return 1;

    return 0;
}

int getTime(canvas* can)
{
    char c[100];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    snprintf(c, 100, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    return getTXT_B(can, c);
}

int getDate(canvas* can)
{
    char c[100];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    snprintf(c, 100, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return getTXT_B(can, c);
}

int getTXT_B(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 12, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getTXT_A(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 10, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getLogo(canvas* can, const char* str)
{
    return can->import_24bit(str);
}

int getSubject(canvas* can, const char *str, int brightness, int contrast)
{
    return can->import_24bit(str, DITHER::Jarvis, brightness, contrast);
}

int getPDF417(canvas* can, const char* str)
{
    int zint_error = 0;
    int canvas_error = 0;

    struct zint_symbol* my_symbol;

    my_symbol = ZBarcode_Create();

    my_symbol->symbology = BARCODE_PDF417;
    my_symbol->scale = 1.0;
    my_symbol->output_options |= OUT_BUFFER_INTERMEDIATE;

    zint_error |= ZBarcode_Encode(my_symbol, (const unsigned char*)str, strlen(str));
    zint_error |= ZBarcode_Buffer(my_symbol, 0);

    if (zint_error == 0)
        add_barcode_to_canvas(can, my_symbol);

    ZBarcode_Delete(my_symbol);

    return zint_error ? 1 : 0;
}

int add_barcode_to_canvas(canvas* can, zint_symbol* my_symbol)
{
    bool err;
    int row, col, i = 0;
    err = can->create(my_symbol->bitmap_width, my_symbol->bitmap_height, 0);
    for (row = 0; row < my_symbol->bitmap_height; row++)
        for (col = 0; col < my_symbol->bitmap_width; col++, i++)
            if (my_symbol->bitmap[i] == '1')
                can->setPixle(col, row, 1);
    return err;
}