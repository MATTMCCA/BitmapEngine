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

#include <process.h>

#include "canvas.hpp"
#include "font.hpp"

#define FONT_ARIAL      "C:\\Windows\\Fonts\\arialbd.ttf"

#define SAVE_AS         "A:\\Users\\Matt\\Pictures\\TRASH\\LLOUT\\"
#define LOGO            "A:\\Users\\Matt\\Pictures\\TRASH\\LL.bmp"
#define LIPS            "A:\\Users\\Matt\\Pictures\\TRASH\\lips.bmp"


const int DPI = 203; //used for bitmap

int getLogo2(canvas* can, const char* str, int brightness, int contrast);
int getLogo(canvas* can, const char* str);

int getTXT_A(canvas* can, const char* str);
int getTXT_B(canvas* can, const char* str);
int getTXT_C(canvas* can, const char* str);

int getTime(std::string *str);
int getDate(std::string* str);
int getToekn(std::string* str);

int create_cell(canvas* can, char num, int c_size, bool idk);
int create_play_area(canvas* can);

int get_calls(canvas* can);


int calc_offset(int32_t m, int32_t s)
{
    return (m - s) / 2;
}

int main(int argc, char* argv[])
{

    std::string DB_TOKEN;
    std::string FILE_PATH = std::string(SAVE_AS);

    canvas* temp;
    canvas master;
    
    getToekn(&DB_TOKEN);
    FILE_PATH += (std::string("\\") + DB_TOKEN + std::string(".bmp"));

    /* runtime debug */
    printf("FilePath: %s\n",    FILE_PATH.c_str());
    printf("Token: %s\n",       DB_TOKEN.c_str());

    /* Create Master Canvas */
    if (master.create(812, 1624) == 1)                              return 1;

    /* open logo */
    temp = new canvas;
    if (getLogo(temp, LOGO) == 1)                                   return 1;
    if (master.addSprite(temp, 
        calc_offset(master.get_x(), temp->get_x()),
        30, 0) == 1)                                                return 1;
    delete temp;

    /* open logo part 2 */
    temp = new canvas;
    if (getLogo2(temp, LIPS, 0, 0) == 1)                            return 1;
    if (master.addSprite(temp, 
        210, 190, 1) == 1)                                          return 1;
    delete temp;

    /* gen play field */
    temp = new canvas;
    if (create_play_area(temp) == 1)                                return 1;
    if (master.addSprite(temp, 
        calc_offset(master.get_x(), temp->get_x()), 
        400, 0) == 1)                                               return 1;
    delete temp;


    /* Save as MONO Thermal Bitmap */
    if (master.saveBMP(FILE_PATH.c_str(), DPI) == 1)           return 1;

    return 0;
}

int get_calls(canvas* can)
{
    bool ch;
    bool err = 0;
    char bingo[5] = { 'B', 'I', 'N', 'G', 'O' };
    int _calls[5 * 6];

    for (int i = 0; i < (5 * 6); i++)
    {
        int* k = &_calls[i];
        do
        {
            ch = 0;
            *k = (rand() % 75) + 1;
            for (int i = 0; i < 25; i++)
                if (k != &_calls[i])
                    ch |= (*k == _calls[i]);
        } while (ch);
    }



    //finish


    return err;
}

int create_cell(canvas* can, char num, int c_size, bool idk = 0)
{
    bool err = 0;
    canvas* temp = new canvas;
    err |= can->create(c_size, c_size, 0);
    err |= can->drawBox(0, 0, c_size+1, c_size+1, 1, 1);


    if (idk) //char
    {
        err |= (bool)getTXT_C(temp, std::string(1, num).c_str());
    }
    else //num
    {
        err |= (bool)getTXT_C(temp, std::to_string((int)num).c_str());
    }

    err |= can->addSprite(temp,
        calc_offset(can->get_x(), temp->get_x()),
        calc_offset(can->get_y(), temp->get_y()),
        1);

    delete temp;

    return err;
}

int create_play_area(canvas* can)
{
    bool err = 0;
    const int cell_size = 150;
    err |= can->create((cell_size * 5), (cell_size * 6), 0);

    char table[5 * 6];
    memset(table, 0x00, (5 * 6) * sizeof(char));
    table[0] = 'B';
    table[1] = 'I';
    table[2] = 'N';
    table[3] = 'G';
    table[4] = 'O';

    srand((unsigned)time(NULL) * _getpid());

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 5; x++) {

            canvas* temp = new canvas;
            char *k = &table[x + (y * 5)];

            if (*k != 0) {
                err |= (bool) create_cell(temp, *k, cell_size, 1);
            }
            else {
                bool ch = 0;
                char* p = &table[5];
                do {
                    ch = 0;
                    *k = (rand() % 75) + 1;
                    for (int i = 0; i < 25; i++)
                        if(k != &p[i])
                            ch |= (*k == p[i]);
                } while (ch);
                        
                err |= (bool) create_cell(temp, *k, cell_size);
            }

            // gen free spaces?

            err |= can->addSprite(temp, (x * cell_size), (y * cell_size), 0);
            delete temp;
        }
    }

    err |= can->drawBox(0, 0, (cell_size * 6), (cell_size * 5), 1, 1);

    return err;
}

/*
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
*/

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
    return can->import_24bit(str, DITHER::Threshold);
}

int getLogo2(canvas* can, const char *str, int brightness, int contrast)
{
    return can->import_24bit(str, DITHER::Stucki, brightness, contrast);
}