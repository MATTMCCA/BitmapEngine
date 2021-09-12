// mono-madness.c
// 
// MJM - 2021
// 
// bitmaps from scratch, code should be pretty portable 
// 
// 
// testing freetype
// 
// hacked apart: https://www.freetype.org/freetype2/docs/tutorial/example1.c
// 
// 
// not sure how to turn off antialiasing.... learning curve, yay!
// 
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bitmap-mono.h" //mono bitmap lib

#include <ft2build.h>
#include FT_FREETYPE_H


#define WIDTH   640
#define HEIGHT  480

/* origin is the upper left corner */
unsigned char image[HEIGHT][WIDTH];



int bmp_1(const char* path); //test bitmap creation
int bmp_2(const char* path); //test bitmap set pixles
int bmp_3(const char* path); //test bitmap Horizontal Line
int bmp_4(const char* path); //test bitmap Vertical Line
int bmp_5(const char* path); //test bitmap box
int bmp_6(const char* path); //test bitmap boxfill
int bmp_7(const char* path); //test invert
int bmp_8(const char* path); //test sprite alpha = 0
int bmp_9(const char* path); //test sprite alpha = 1
int bmp_10(const char* path); //test new line code
int bmp_11(const char* path); //test circle, no thick yet



/* Replace this function with something useful. */

void draw_bitmap(FT_Bitmap* bitmap,
    FT_Int      x,
    FT_Int      y)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max = x + bitmap->width;
    FT_Int  y_max = y + bitmap->rows;


    /* for simplicity, we assume that `bitmap->pixel_mode' */
    /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

    for (i = x, p = 0; i < x_max; i++, p++)
    {
        for (j = y, q = 0; j < y_max; j++, q++)
        {
            if (i < 0 || j < 0 ||
                i >= WIDTH || j >= HEIGHT)
                continue;

            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
        }
    }
}

void saveImage(const char* path)
{
    int  i, j;

    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "FreeTypeTest.bmp");

    canvas myTest = { NULL };
    createBMP(&myTest, WIDTH, HEIGHT);
       
    for (i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++) 
            if (image[i][j] > 0)
                setPixle(&myTest, j, i, 1);

    saveCanvas(&myTest, 300 /*DPI*/, filename);
    freeBMP(&myTest);
}


int main()
{
    const char TRASH_DIR[] = "A:\\Users\\Matt\\Pictures\\TRASH\\";


    const char FONT_NAME[] = "C:\\Windows\\Fonts\\consola.ttf";
    const char TEXT_THIS[] = "MEOW!";


    FT_Library    library;
    FT_Face       face;

    FT_GlyphSlot  slot;
    FT_Matrix     matrix;                 /* transformation matrix */
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;

    char* filename;
    char* text;

    double        angle;
    int           target_height;
    int           n, num_chars;


    filename = (char*) &FONT_NAME;
    text = (char*) &TEXT_THIS;

    num_chars = strlen(text);
    angle = 0; // (25.0 / 360) * 3.14159 * 2;           /* use 25 degrees     */
    target_height = HEIGHT;

    error = FT_Init_FreeType(&library);                 /* initialize library */
    /* error handling omitted */

    error = FT_New_Face(library, filename, 0, &face);   /* create face object */
    /* error handling omitted */

    /* use 50pt at 100dpi */
    error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0);   /* set character size */
    /* error handling omitted */

    /* cmap selection omitted;                                        */
    /* for simplicity we assume that the font contains a Unicode cmap */

    slot = face->glyph;

    /* set up matrix */
    matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
    matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
    matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
    matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

    /* the pen position in 26.6 cartesian space coordinates; */
    /* start at (300,200) relative to the upper left corner  */
    pen.x = 300 * 64;
    pen.y = (target_height - 200) * 64;

    for (n = 0; n < num_chars; n++)
    {
        /* set transformation */
        FT_Set_Transform(face, &matrix, &pen);

        /* load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(face, text[n], FT_LOAD_RENDER); 
        if (error)
            continue;                 /* ignore errors */

        /* now, draw to our target surface (convert position) */
        draw_bitmap(&slot->bitmap, slot->bitmap_left, target_height - slot->bitmap_top);

        /* increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    saveImage(TRASH_DIR);

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    return 0;

    /*
    printf("1-bit Bitmap Creation Test\n");
    printf("=====================================\n");
    //=================================================
    printf("bmp_1: ");
    if (bmp_1(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_2: ");
    if (bmp_2(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_3: ");
    if (bmp_3(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_4: ");
    if (bmp_4(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_5: ");
    if (bmp_5(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_6: ");
    if (bmp_6(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_7: ");
    if (bmp_7(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_8: ");
    if (bmp_8(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_9: ");
    if (bmp_9(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    //=====================================
    printf("bmp_10: ");
    if (bmp_10(TRASH_DIR)) printf("FAIL\n");
    else                   printf("PASS\n");
    //=====================================
    printf("bmp_11: ");
    if (bmp_11(TRASH_DIR)) printf("FAIL\n");
    else                   printf("PASS\n");
    //=====================================
    printf("=====================================\n");

    return 0;
    */
}

int bmp_1(const char* path)
{
    char filename[500] = {0x00};
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_001.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 30, 30))
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_2(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_002.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 30, 30))
        return 1;

    for (int i = 0; i < 30; i++)
        if (setPixle(&myTest, i, i, 1))
            return 1;
    
    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_3(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_003.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 30, 30))
        return 1;

    if (drawHorizontalLine(&myTest, 2, 1, 25, 1, 1))  //1 pix wide
        return 1;

    if (drawHorizontalLine(&myTest, 2, 4, 25, 3, 1))  //3 pix wide
        return 1;

    if (drawHorizontalLine(&myTest, 2, 10, 25, 5, 1)) //4 pix wide
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_4(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_004.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 30, 30))
        return 1;

    if (drawVerticalLine(&myTest, 1, 2, 25, 1, 1))  //1 pix wide
        return 1;

    if (drawVerticalLine(&myTest, 4, 2, 25, 3, 1))  //3 pix wide
        return 1;

    if (drawVerticalLine(&myTest, 10, 2, 25, 4, 1)) //4 pix wide
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_5(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_005.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 100, 80))
        return 1;

    if (drawBox(&myTest, 1, 1, 20, 90, 1, 1))       //1 pix thick
        return 1;

    if (drawBox(&myTest, 3, 30, 40, 70, 5, 1))      //5 pix thick
       return 1;

    if (drawBox(&myTest, 20, 10, 50, 60, 2, 1))     //2 pix thick
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_6(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_006.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 100, 80))
        return 1;

    if (drawBoxFill(&myTest, 1, 1, 20, 90, 1))      //black fill
        return 1;

    if (drawBoxFill(&myTest, 3, 30, 40, 70, 1))     //black fill
        return 1;

    if (drawBoxFill(&myTest, 20, 10, 50, 60, 0))    //white fill
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_7(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_007.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 20, 20))
        return 1;

    if (drawBox(&myTest, 1, 1, 15, 15, 2, 1))      //2 pix thick
        return 1;
    
    if (invertCanvas(&myTest))
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_8(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_008.bmp");

    canvas myTest = { NULL };
    canvas mysprite = { NULL };

    /* create two canvases */
    if (createBMP(&myTest, 20, 20)) return 1;
    if (createBMP(&mysprite, 9, 9))   return 1;

    /* mysprite = checkerboard */
    bool toggle = 1;
    for (size_t i = 0; i < (mysprite.x * mysprite.y); i++) {
        mysprite.ptr[i] = toggle;
        toggle = !toggle;
    }

    if (drawBox(&myTest, 5, 5, 10, 10, 1, 1))
        return 1;

    /* add sprite to mytest */
    if (addSpriteCanvas(&myTest, &mysprite, 3, 3, 0)) return 1;

    /* free sprite after copy */
    freeBMP(&mysprite);

    if (saveCanvas(&myTest, 300 /*DPI*/, filename)) return 1;

    freeBMP(&myTest);

    return 0;
}

int bmp_9(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_009.bmp");

    canvas myTest = { NULL };
    canvas mysprite = { NULL };

    /* create two canvases */
    if (createBMP(&myTest, 20, 20)) return 1;
    if (createBMP(&mysprite, 9, 9))   return 1;

    /* mysprite = checkerboard */
    bool toggle = 1;
    for (size_t i = 0; i < (mysprite.x * mysprite.y); i++) {
        mysprite.ptr[i] = toggle;
        toggle = !toggle;
    }

    if (drawBox(&myTest, 5, 5, 10, 10, 1, 1))
        return 1;

    /* add sprite to mytest */
    if (addSpriteCanvas(&myTest, &mysprite, 3, 3, 1)) return 1;

    /* free sprite after copy */
    freeBMP(&mysprite);

    if (saveCanvas(&myTest, 300 /*DPI*/, filename)) return 1;

    freeBMP(&myTest);

    return 0;
}

int bmp_10(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_010.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 50, 50))
        return 1;

    if (drawLine(&myTest, 2, 2, 40, 40, 4, 1))
        return 1;

    if (drawLine(&myTest, 45, 1, 5, 40, 4, 1))
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}

int bmp_11(const char* path)
{
    char filename[500] = { 0x00 };
    strcat_s(filename, 500, path);
    strcat_s(filename, 500, "bmp_011.bmp");

    canvas myTest = { NULL };
    if (createBMP(&myTest, 100, 100))
        return 1;

    if (drawCircle(&myTest, 50, 50, 40, 1))
        return 1;

    if (drawCircle(&myTest, 50, 50, 30, 1))
        return 1;

    if (drawCircle(&myTest, 50, 50, 10, 1))
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}