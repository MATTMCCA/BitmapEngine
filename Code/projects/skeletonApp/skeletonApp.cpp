/*
    ***************************************************************************************
    MJM -2021-

    ---------------------------------------------------------------------------------------

    =======================================================================================

        Notes: Skeleton App

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
#include <cstdio>
#include <cstdint>
#include <cctype>

#include <zint.h>

#include "canvas.hpp"
#include "font.hpp"

#define ARIAL "C:\\Windows\\Fonts\\arial.ttf"
#define CATIMAGE "CAT.bmp"

const int DPI = 300; //used for bitmap

/************************ Helpers ******************************/
int add_barcode_to_canvas(canvas* can, zint_symbol* my_symbol);

void test(void);
/***************************************************************/


/**************** FUN-ctions YAY!! *************************/
int getQRcode(canvas* can, const char* str);
int getUPCA(canvas* can, const char* str);
int getTextA(canvas* can, const char* str);
int getTextB(canvas* can, const char* str);
int getImage(canvas* can, const char* fileName);
/***********************************************************/


int main(int argc, char* argv[])
{

    //test();
    //return 0;


    canvas* temp;
    canvas master;

    if (master.create(500, 500) == 1)                   
        return 1;

    if (master.drawBox(5, 5, 490, 490, 5, 1) == 1)      
        return 1;

    if (master.drawHorizontalLine(40, 128, 400, 2, 1) == 1)
        return 1;

    temp = new canvas;
    if (getQRcode(temp, "Meow") == 0) {
        if (master.addSprite(temp, 20, 20, 0) == 1)
            return 1;
        delete temp;
    }

    temp = new canvas;
    if (getUPCA(temp, "72527270270") == 0) {
        temp->rotate(DEGREE::ROT_90);
        if (master.addSprite(temp, 40, 200, 0) == 1)
            return 1;
        delete temp;
    }

    temp = new canvas;
    if (getTextA(temp, "Meow") == 0) {
        if (master.addSprite(temp, 150, 30, 0) == 1)
            return 1;
        delete temp;
    }

    temp = new canvas;
    if (getTextB(temp, "CATS\nMEOW") == 0) {
        temp->invert(1);
        if (master.addSprite(temp, 250, 150, 0) == 1)
            return 1;
        delete temp;
    }

    temp = new canvas;
    if (getImage(temp, CATIMAGE) == 0) {
        if (master.addSprite(temp, 200, 250, 0) == 1)
            return 1;
        delete temp;
    }

    if (master.saveBMP("MEOW.bmp", DPI) == 1) {
        printf("bmp err\n");
        return 1;
    }

    if (master.savePBM("MEOW.pbm") == 1) {
        printf("bmp err\n");
        return 1;
    }

    if (master.saveJBG("MEOW.jbg") == 1) {
        printf("bmp err\n");
        return 1;
    }

    return 0;
}


/**************** FUN-ctions YAY!! *************************/

int getQRcode(canvas* can, const char* str)
{
    int zint_error = 0;
    int canvas_error = 0;

    struct zint_symbol* my_symbol;

    my_symbol = ZBarcode_Create();

    my_symbol->symbology = BARCODE_QRCODE;
    my_symbol->scale = 2.0;
    my_symbol->output_options |= OUT_BUFFER_INTERMEDIATE;

    zint_error |= ZBarcode_Encode(my_symbol, (const unsigned char*) str, strlen(str));
    zint_error |= ZBarcode_Buffer(my_symbol, 0);

    if (zint_error == 0)
        add_barcode_to_canvas(can, my_symbol);
    
    ZBarcode_Delete(my_symbol);

    return zint_error ? 1 : 0;
}

int getUPCA(canvas* can, const char* str)
{
    int zint_error = 0;
    int canvas_error = 0;

    struct zint_symbol* my_symbol;

    my_symbol = ZBarcode_Create();

    my_symbol->symbology = BARCODE_UPCA;
    my_symbol->scale = 1.0;
    my_symbol->output_options |= OUT_BUFFER_INTERMEDIATE;

    zint_error |= ZBarcode_Encode(my_symbol, (const unsigned char*)str, strlen(str));
    zint_error |= ZBarcode_Buffer(my_symbol, 0);

    if (zint_error == 0)
        add_barcode_to_canvas(can, my_symbol);

    ZBarcode_Delete(my_symbol);

    return zint_error ? 1 : 0;
}

int getTextA(canvas* can, const char* str)
{
    bool err = 0;
    font f_20pt;
    err |= f_20pt.create(ARIAL, 20, DPI);
    err |= f_20pt.writeCanvas(can, str, 0, 0);
    return err;
}

int getTextB(canvas* can, const char* str)
{
    bool err = 0;
    font f_8pt;
    err |= f_8pt.create(ARIAL, 8, DPI);
    err |= f_8pt.writeCanvas(can, str, 0, 0);
    return err;
}

int getImage(canvas* can, const char* fileName)
{
    bool err = 0;
    err |= can->import_24bit(fileName, DITHER::Stucki);
    return err;
}

/************************************************************/


/////////////////////// HELPERS ///////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////


void test(void)
{
    canvas hhh;
    hhh.import_pbm("moo.pbm");
    hhh.saveBMP("moo_conv.bmp", DPI);
}