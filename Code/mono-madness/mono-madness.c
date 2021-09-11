// mono-madness.c
// 
// MJM - 2021
// 
//

#include <stdio.h>
#include <stdint.h>

#include "bitmap-mono.h" //mono bitmap lib


#define TRASH_DIR "A:\\Users\\Matt\\Pictures\\TRASH\\test.bmp"


int main()
{
    printf("test\n");

    canvas myTest = {0x00};

    createBMP(&myTest, 2, 2);
    saveCanvas(&myTest, 96, TRASH_DIR);
    freeBMP(&myTest);

    return 0;
}
