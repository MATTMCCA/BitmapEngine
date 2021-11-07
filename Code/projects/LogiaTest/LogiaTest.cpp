/*
    ***************************************************************************************
    MJM -2021-

    ---------------------------------------------------------------------------------------

    =======================================================================================

        Notes: 
            Logia Thermal Printer Test (I'm pore, its a cheap 300DPI printer lol)

            http://labelary.com/viewer.html

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

#include <zint.h>

#include "canvas.hpp"
#include "font.hpp"

#define ARIAL "C:\\Windows\\Fonts\\arial.ttf"

int catch_barcode_error(int zerr, char* cptr);
int add_barcode_to_canvas(canvas* can, zint_symbol* my_symbol);
int getQRcode(canvas* can, const char* str);
int getTextA(canvas* can, int DPI, const char* str);
int getImage(canvas* can, const char* fileName);
int save_format(
    canvas* can, int DPI, float stock_width_inch, 
    float stock_height_inch, int rate, int darkness, 
    int quantity, const char* file);

int32_t calc_offset(int32_t m, int32_t s)
{
    return (m - s) / 2;
}

int main(int argc, char* argv[])
{
    canvas* temp;
    canvas master;

    char* inputimage;
    char* txt;
    char* save_as;
    float inch_w;
    float inch_h;
    int dpi;
    int rate;
    int count;
    int dark;

    if (argc < 9) {
        printf("%s dark, rate, dpi, w h count txt img_in save_as.zpl\n", argv[0]);
        return 1;
    }

    dark = atoi(argv[1]);
    rate = atoi(argv[2]);
    dpi = atoi(argv[3]);
    inch_w = (float)atof(argv[4]);
    inch_h = (float)atof(argv[5]);
    count = atoi(argv[6]);
    txt = argv[7];
    inputimage = argv[8];
    save_as = argv[9];

    if (master.create((int32_t) (dpi*inch_w), (int32_t)(dpi * inch_h)) == 1)
        return 1;

    int32_t _h = 0;

    temp = new canvas;
    if (getImage(temp, inputimage) == 0) {
        if (master.addSprite(temp, calc_offset(master.get_x(), temp->get_x()), 0+50, 0) == 1)
            return 1;
        _h += temp->get_y();
        delete temp;
    }

    temp = new canvas;
    if (getQRcode(temp, txt) == 0) {
        if (master.addSprite(temp, calc_offset(master.get_x(), temp->get_x()), _h+100, 0) == 1)
            return 1;
        _h += temp->get_y();
        delete temp;
    }

    temp = new canvas;
    if (getTextA(temp, dpi, txt) == 0) {
        if (master.addSprite(temp, calc_offset(master.get_x(), temp->get_x()), _h+150, 0) == 1)
            return 1;
        delete temp;
    }

    if (save_format(&master, dpi, 4.0, 6.0, 8, 8, 1, save_as)) {
        printf("Could not Save %s\n", save_as);
        return 1;
    }

    return 0;
}

int getTextA(canvas* can, int DPI, const char* str)
{
    font t;
    bool err = t.create(ARIAL, 12, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getImage(canvas* can, const char* fileName)
{
    return can->import_24bit(fileName, DITHER::Stucki);
}

int getQRcode(canvas* can, const char* str)
{
    int zint_error = 0;
    int canvas_error = 0;

    struct zint_symbol* my_symbol;

    my_symbol = ZBarcode_Create();

    my_symbol->symbology = BARCODE_QRCODE;
    my_symbol->scale = 2;
    my_symbol->output_options |= OUT_BUFFER_INTERMEDIATE;

    zint_error = ZBarcode_Encode(my_symbol, (const unsigned char*)str, strlen(str));
    if (!catch_barcode_error(zint_error, my_symbol->errtxt)) {
        zint_error = ZBarcode_Buffer(my_symbol, 0);
        if (!catch_barcode_error(zint_error, my_symbol->errtxt))
            zint_error = add_barcode_to_canvas(can, my_symbol);
    }

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

int catch_barcode_error(int zerr, char* cptr)
{
    if (zerr != 0) {
        printf("ZINT_ERR: %s\n", cptr); //remove?
        return 1;
    }
    return 0;
}

// ^MN
// ^XA
// ^LH0,0
// ^MMT
// ^PW1198
// ^LS0
// ^POI
// ^LL1798
// ^PR8
// ~SD8 
// ^FO0, 0, 0
// ^GFA, 270000, 270000, 150, 
// --<bmp>--
// ^PQ1 
// ^XZ

int save_format(
    canvas *can, int DPI, float stock_width_inch, 
    float stock_height_inch, int rate, int darkness, 
    int quantity, const char* file)
{
    /* Generate Format */
    bool err = 0;
    uint32_t _len = 0;
    stock_height_inch *= DPI;
    stock_width_inch *= DPI;
    
    FILE* fd;
    fopen_s(&fd, file, "wb");
    if (fd != NULL) {
        /* Media Tracking, Start Format, Lable Home [0,0], Print Mode [Tear-Off]*/
        fprintf(fd, "^MN\n^XA\n^LH0,0\n^MMT");
        /* print Width (in dots) */
        fprintf(fd, "^PW%d\n", (int)stock_width_inch);
        /* Lable Shift, Print Orientation [normal]*/
        fprintf(fd, "^LS0\n^PON\n");
        /* Lable Length (in dots) */
        fprintf(fd, "^LL%d\n", (int)stock_height_inch);
        /* Print Rate [ips] */
        fprintf(fd, "^PR%d\n", rate);
        /* Set Darkness */
        fprintf(fd, "~SD%02d\n", darkness);
        /* Field Origin [0,0,0] */
        fprintf(fd, "^FO0, 0, 0\n");

        /* Bitmap Data */
        uint8_t* zpldata = can->getZPL(&_len);
        if (zpldata != nullptr) {
            fwrite(zpldata, sizeof(uint8_t), _len, fd);
            delete[] zpldata;
        }
        else err |= 1;

        /* Print Quantity */
        fprintf(fd, "\r\n^PQ%d", quantity);
        /* End Format */
        fprintf(fd, "^XZ\r\n");

        if ((fclose(fd) != 0) || (ferror(fd)))
            err |= 1;
    }
    else err |= 1;

    return (int)err;
}