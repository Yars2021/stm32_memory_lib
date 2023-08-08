#define MAIN

#include "include.h"

int main (void)
{
    initLowLevel();
    char mem[12] = "hello world";
    char in[256] = {0};

    while (1)
    {
        writemem(Flash_Memory, mem, 11);
        readmem(Flash_Memory, in, 11);
        HAL_Delay(100);
    }
}
