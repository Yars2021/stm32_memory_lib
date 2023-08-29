#define MAIN

#include "include.h"

extern uint32_t __fini_array_end;

uint32_t program_end;

char write_mem[12] = "hello world\0";
char read_mem[256] = {0};

size_t addr = 0x08003C00;

int main (void)
{
    initLowLevel();

    while (1)
    {
        program_end = __fini_array_end;

        writemem(Flash_Memory, addr, write_mem, 12);
        readmem(Flash_Memory, addr, read_mem, 11);
        HAL_Delay(100);
    }
}
