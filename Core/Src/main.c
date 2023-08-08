#define MAIN

#include "include.h"

extern uint32_t __fini_array_end;

uint32_t program_end;

int main (void)
{
    initLowLevel();
    char mem[12] = "hello world";
    char in[256] = {0};
    size_t addr = 0x08003C00;

    while (1)
    {
        program_end = __fini_array_end;

        writemem(Flash_Memory, addr, mem, 11);
        readmem(Flash_Memory, addr, in, 11);
        HAL_Delay(100);
    }
}
