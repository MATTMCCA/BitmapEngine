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

    if (drawHorizontalLine(&myTest, 2, 1, 25, 1, 1))
        return 1;

    if (drawHorizontalLine(&myTest, 2, 4, 25, 3, 1))
        return 1;

    if (drawHorizontalLine(&myTest, 2, 10, 25, 5, 1))
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

    if (drawVerticalLine(&myTest, 1, 2, 25, 1, 1))
        return 1;

    if (drawVerticalLine(&myTest, 4, 2, 25, 3, 1))
        return 1;

    if (drawVerticalLine(&myTest, 10, 2, 25, 4, 1))
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

    if (drawBox(&myTest, 1, 1, 20, 90, 1, 1))
        return 1;

    if (drawBox(&myTest, 3, 30, 40, 70, 5, 1))
       return 1;

    if (drawBox(&myTest, 20, 10, 50, 60, 2, 1))
        return 1;

    if (saveCanvas(&myTest, 300 /*DPI*/, filename))
        return 1;

    freeBMP(&myTest);
    return 0;
}
