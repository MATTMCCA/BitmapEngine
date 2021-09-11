// mono-madness.c
// 
// MJM - 2021
// 
// bitmaps from scratch, code should be pretty portable 
// 
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bitmap-mono.h" //mono bitmap lib

int bmp_1(const char *path); //test bitmap creation
int bmp_2(const char* path); //test bitmap set pixles
int bmp_3(const char* path); //test bitmap Horizontal Line
int bmp_4(const char* path); //test bitmap Vertical Line
int bmp_5(const char* path); //test bitmap box
int bmp_6(const char* path); //test bitmap boxfill
int bmp_7(const char* path); //test invert
int bmp_8(const char* path); //test sprite alpha = 0
int bmp_9(const char* path); //test sprite alpha = 1


int main()
{
    const char TRASH_DIR[] = "A:\\Users\\Matt\\Pictures\\TRASH\\";

    printf("1-bit Bitmap Creation Test\n");
    printf("=====================================\n");
    /*******************************************************/
    printf("bmp_1: ");
    if (bmp_1(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_2: ");
    if (bmp_2(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_3: ");
    if (bmp_3(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_4: ");
    if (bmp_4(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_5: ");
    if (bmp_5(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_6: ");
    if (bmp_6(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_7: ");
    if (bmp_7(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_8: ");
    if (bmp_8(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("bmp_9: ");
    if (bmp_9(TRASH_DIR)) printf("FAIL\n");
    else                  printf("PASS\n");
    /***************************************/
    printf("=====================================\n");

    return 0;
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
