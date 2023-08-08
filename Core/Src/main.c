#define MAIN

#include "include.h"

int main (void)
{
    initLowLevel();
    char mem[12] = "hello world";
    char in[256] = {0};
    size_t addr = 0x08003C00;

    while (1)
    {
        writemem(Flash_Memory, mem, 11, addr);
        readmem(Flash_Memory, in, 11, addr);
        HAL_Delay(100);
    }
}
