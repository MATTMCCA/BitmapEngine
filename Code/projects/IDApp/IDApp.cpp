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
#include <process.h>

#include "canvas.hpp"
#include "font.hpp"

#define FONT_ARIAL      "C:\\Windows\\Fonts\\arial.ttf"

#define SAVE_AS         "A:\\Users\\Matt\\Pictures\\TRASH\\ID\\"
#define LOGO            "A:\\Users\\Matt\\Pictures\\TRASH\\LB.bmp"
#define SUBJECT         "A:\\Users\\Matt\\Pictures\\TRASH\\blank.bmp"
#define FIRST_NAME      "<FNAME>"
#define LAST_NAME       "<LNAME>"
#define _EXTRA          "<EXTRA>"

const int DPI = 300;              //used for bitmap

typedef struct
{
    std::string FIRSTNAME;        //1
    std::string LASTNAME;         //2
    std::string EXTRA_FEILD;      //3
    std::string DATE;             //AUTO
    std::string TIME;             //AUTO
    std::string DB_TOKEN;         //AUTO
    std::string HEADER;           //4
    std::string ID_PHOTO;         //5
    std::string FILE_PATH;        //6
}META;

int add_barcode_to_canvas(canvas* can, zint_symbol* my_symbol);

int getBARCODE(canvas* can, const char* str);

int getSubject(canvas* can, const char* str, int brightness, int contrast);
int getLogo(canvas* can, const char* str);

int getTXT_A(canvas* can, const char* str);
int getTXT_B(canvas* can, const char* str);
int getTXT_C(canvas* can, const char* str);

int getTime(std::string *str);
int getDate(std::string* str);
int getToekn(std::string* str);

int main(int argc, char* argv[])
{
    META data;
    canvas* temp;
    canvas master;

    /* parse in struct */
    if (argc > 6) {
        data.FIRSTNAME          = std::string(argv[1]);
        data.LASTNAME           = std::string(argv[2]);
        data.EXTRA_FEILD        = std::string(argv[3]);
        data.HEADER             = std::string(argv[4]);
        data.ID_PHOTO           = std::string(argv[5]);
        data.FILE_PATH          = std::string(argv[6]);
    }
    else {
        /* DEBUG PATHS */
        data.FIRSTNAME          = std::string(FIRST_NAME);
        data.LASTNAME           = std::string(LAST_NAME);
        data.EXTRA_FEILD        = std::string(_EXTRA);
        data.HEADER             = std::string(LOGO);
        data.ID_PHOTO           = std::string(SUBJECT);
        data.FILE_PATH          = std::string(SAVE_AS);
        printf("!!Using Debug Paths!!\n");
    }
    
    /* gen dynamic data */
    getDate(&data.DATE);
    getTime(&data.TIME);
    getToekn(&data.DB_TOKEN);
    data.FILE_PATH += (std::string("\\") + data.DB_TOKEN + std::string(".bmp"));

    /* runtime debug */
    printf("FirstName: %s\n",   data.FIRSTNAME.c_str());
    printf("LastName: %s\n",    data.LASTNAME.c_str());
    printf("Extra: %s\n",       data.EXTRA_FEILD.c_str());
    printf("HeaderPath: %s\n",  data.HEADER.c_str());
    printf("ID_Path: %s\n",     data.ID_PHOTO.c_str());
    printf("FilePath: %s\n",    data.FILE_PATH.c_str());
    printf("Date: %s\n",        data.DATE.c_str());
    printf("Time: %s\n",        data.TIME.c_str());
    printf("Token: %s\n",       data.DB_TOKEN.c_str());


    /* Create Master Canvas */
    if (master.create(843, 581) == 1)                               return 1;

    /* open header logo */
    temp = new canvas;
    if (getLogo(temp, data.HEADER.c_str()) == 1)                    return 1;
    if (master.addSprite(temp, 75, 20, 0) == 1)                     return 1;
    delete temp;

    /* open ID Photo */
    temp = new canvas;
    if (getSubject(temp, data.ID_PHOTO.c_str(), 0, 0) == 1)         return 1;
    if (master.addSprite(temp, 10, 180, 0) == 1)                    return 1;
    delete temp;

    /* render first name */
    temp = new canvas;
    if (getTXT_C(temp, data.FIRSTNAME.c_str()) == 1)                return 1;
    if (master.addSprite(temp, 380, 160, 0) == 1)                   return 1;
    delete temp;

    /* render last name */
    temp = new canvas;
    if (getTXT_C(temp, data.LASTNAME.c_str()) == 1)                 return 1;
    if (master.addSprite(temp, 380, 212, 0) == 1)                   return 1;
    delete temp;

    /* render extra */
    temp = new canvas;
    if (getTXT_B(temp, data.EXTRA_FEILD.c_str()) == 1)              return 1;
    if (master.addSprite(temp, 380, 270, 0) == 1)                   return 1;
    delete temp;

    /* render date */
    temp = new canvas;
    if (getTXT_A(temp, data.DATE.c_str()) == 1)                     return 1;
    if (master.addSprite(temp, 380, 400, 0) == 1)                   return 1;
    delete temp;

    /* render time */
    temp = new canvas;
    if (getTXT_A(temp, data.TIME.c_str()) == 1)                     return 1;
    if (master.addSprite(temp, 380, 440, 0) == 1)                   return 1;
    delete temp;

    /* render token */
    temp = new canvas;
    if (getTXT_A(temp, data.DB_TOKEN.c_str()) == 1)                 return 1;
    if (master.addSprite(temp, 380, 330, 0) == 1)                   return 1;
    delete temp;

    /* render barcode */
    temp = new canvas;
    if (getBARCODE(temp, data.DB_TOKEN.c_str()) == 1)               return 1;
    if (master.addSprite(temp, 430, 500, 0) == 1)                   return 1;
    delete temp;

    /* Save as MONO Thermal Bitmap */
    if (master.saveBMP(data.FILE_PATH.c_str(), DPI) == 1)           return 1;

    return 0;
}

int getTime(std::string* str)
{
    char c[100];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    snprintf(c, 100, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    *str = std::string(c);
    return 0;
}

int getDate(std::string* str)
{
    char c[100];
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    snprintf(c, 100, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    *str = std::string(c);
    return 0;
}

int getToekn(std::string* str)
{
    str->clear();
    srand((unsigned)time(NULL) * _getpid());
    for (int i = 0; i < 10; i++) {
        int h = rand() % 10;
        *str += std::to_string(h);
    }

    return 0;
}

int getTXT_A(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 8, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getTXT_B(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 10, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getTXT_C(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 12, DPI);
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

int getBARCODE(canvas* can, const char* str)
{
    int zint_error = 0;
    int canvas_error = 0;

    struct zint_symbol* my_symbol;

    my_symbol = ZBarcode_Create();

    my_symbol->symbology = BARCODE_MICROPDF417;
    my_symbol->scale = 2;
    my_symbol->output_options |= OUT_BUFFER_INTERMEDIATE;

    zint_error |= ZBarcode_Encode(my_symbol, (const unsigned char*)str, strlen(str));
    zint_error |= ZBarcode_Buffer(my_symbol, 0);

    if (zint_error == 0)
        add_barcode_to_canvas(can, my_symbol);

    ZBarcode_Delete(my_symbol); //<-- will crash if the barcode is in the wrong format

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