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

#define TRASH_DIR "A:\\Users\\Matt\\Pictures\\TRASH\\test.bmp"

int main()
{
    printf("1-bit Bitmap Creation Test\n");

    canvas myTest = { NULL };
    createBMP(&myTest, 8, 8);    

    setPixle(&myTest, 0, 0, 1);
    setPixle(&myTest, 3, 3, 1);
    setPixle(&myTest, 4, 4, 1);
    setPixle(&myTest, 7, 7, 1);

    saveCanvas(&myTest, 300 /*DPI*/, TRASH_DIR);
    freeBMP(&myTest);

    printf("Fin\n");

    return 0;
}
