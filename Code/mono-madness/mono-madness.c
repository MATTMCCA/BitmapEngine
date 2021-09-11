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
