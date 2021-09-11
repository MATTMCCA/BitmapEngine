// mono-madness.c
// 
// MJM - 2021
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

    myTest.ptr[0] = 1;
    myTest.ptr[3] = 1;

    saveCanvas(&myTest, 96, TRASH_DIR);
    freeBMP(&myTest);

    printf("Fin\n");

    return 0;
}
