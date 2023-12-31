#define MAIN

#include "include.h"
#include "memory.h"

HAL_StatusTypeDef w_stat, r_stat;

// write_long_mem   Нужен для проверки многостраничной записи
char write_short_mem[13] = "hello world\0";
char write_long_mem[256] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890987654321!@#$^&*()_++_)(*&^$#@!\0";
char write_long_long_mem[2048] = {1};
char read_mem[2048];

size_t addr = 0x1A;

int main (void)
{
    write_long_long_mem[2047] = 'f';
    initLowLevel();

    init_mem();

    HAL_StatusTypeDef status;

    while (1)
    {
        status |= writemem(&flash_dev, 0x08011000, write_long_long_mem, 2048);
        HAL_Delay(100);
        status |= readmem(&flash_dev, 0x08011000, read_mem, 2048);

        status |= writemem(&eeprom_dev_0, addr, write_long_mem, 255);
        HAL_Delay(100);
        status |= readmem(&eeprom_dev_0, addr, read_mem, 255);

        status |= writemem(&n25q_dev_0, addr, write_long_long_mem, 255);
        HAL_Delay(100);
        status |= readmem(&n25q_dev_0, addr, read_mem, 255);
    }
}
