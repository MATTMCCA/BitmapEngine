/*
    ***************************************************************************************
    MJM -2021-

    This is a Sample application, designed to demonstrate the
    graphical capability of the bitmap engine.

    ---------------------------------------------------------------------------------------

    Dumps QR Code out as 1-bit bitmap

    =======================================================================================

    File: zint_test.cpp

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

/*
    installing zint
    //https://github.com/woo-j/zint/tree/master/win32

    Visual Studio 2019
    ------------------

    To build the Zint library DLL and the command line tool "zint.exe" with PNG
    support for x86/Win32:

    Install git (https://git-scm.com/downloads)
    Install cmake (https://cmake.org/download/)

    Open a "Developer Command Prompt for VS 2017" (should be available under the
    "Visual Studio 2017" tab in the Start menu).

    Make sure git and cmake are in your PATH, e.g. (your paths may differ)

      set "PATH=C:\Program Files\Git\cmd;%PATH%"
      set "PATH=C:\Program Files\CMake\bin;%PATH%"

    Download zint, zlib and libpng by going to the directory you want to clone them
    into:

      cd <project-directory>

    and cloning each:

      git clone https://git.code.sf.net/p/zint/code zint
      git clone https://git.code.sf.net/p/libpng/code lpng
      git clone https://github.com/madler/zlib.git zlib

    First build zlib:

      cd zlib

      nmake -f win32\Makefile.msc clean
      nmake -f win32\Makefile.msc LOC="-DASMV -DASMINF=" OBJA="inffas32.obj match686.obj"

      cd ..

    and then lpng:

      cd lpng

      cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_BUILD_TYPE=Release^
            -DPNG_BUILD_ZLIB=ON -DZLIB_INCLUDE_DIRS=..\zlib^
            -DPNG_STATIC=ON -DPNG_SHARED=OFF -B build

      cmake --build build --config Release

      cd ..

    If you now open "%cd%\zint\win32\zint.sln" with Visual Studio 2017, you
    should be able to build the Release configuration for Win32.

    "zint.dll" and "zint.exe" will be in "zint\win32\Release".

    
    Manual: https://zint.org.uk/manual/chapter/5

*/

#define _CRT_SECURE_NO_DEPRECATE

#include <chrono>

#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include <zint.h>

#include "canvas.hpp"
#include "font.hpp"

using namespace std::chrono;

const int DPI = 300;

int add_barcode_to_canvas(canvas* my_canvas, zint_symbol* my_symbol); //must use "OUT_BUFFER_INTERMEDIATE"

int main(int argc, char* argv[])
{
    unsigned const char str[] = "How Now Brown Cow";

    int zint_error = 0;
    int canvas_error = 0;

    auto start = high_resolution_clock::now();

    canvas master;
    struct zint_symbol* my_symbol;

    my_symbol = ZBarcode_Create();

    my_symbol->symbology = BARCODE_QRCODE;
    my_symbol->scale            = 2.0;
    my_symbol->output_options   |= OUT_BUFFER_INTERMEDIATE;

    zint_error |= ZBarcode_Encode(my_symbol, str, strlen((const char*)str));
    zint_error |= ZBarcode_Buffer(my_symbol, 0);

    if (zint_error == 0) {
        add_barcode_to_canvas(&master, my_symbol);
        master.save("QR_TEST.bmp", DPI); //will output to prog debug dir
    }
    else {
        printf("hmmm..... error?\n");
    }

    ZBarcode_Delete(my_symbol);
      

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    printf("Took: %I64d milliseconds\n", duration.count());

    return 0;
}

int add_barcode_to_canvas(canvas* my_canvas, zint_symbol* my_symbol)
{
    int row, col, i = 0;
    my_canvas->create(my_symbol->bitmap_width, my_symbol->bitmap_height, 0);

    for (row = 0; row < my_symbol->bitmap_height; row++) {
        for (col = 0; col < my_symbol->bitmap_width; col++) {
            if (my_symbol->bitmap[i] == '1') {
                my_canvas->setPixle(col, row, 1);
            }
            i++;
        }
    }

    return 0;
}

