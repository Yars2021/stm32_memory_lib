#define MAIN

#include "include.h"

extern N25Q_device_t *n25q_dev_0;

HAL_StatusTypeDef w_stat, r_stat;

// write_long_mem   Нужен для проверки многостраничной записи
char write_short_mem[13] = "hello world\0";
char write_long_mem[256] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890987654321!@#$^&*()_++_)(*&^$#@!\0";
char read_mem[256] = {0};

size_t addr = 0xA;

int main (void)
{
    initLowLevel();

    init_mem();

    while (1)
    {
        writemem(n25q_dev_0, addr, write_long_mem, 255);
        HAL_Delay(100);
        readmem(n25q_dev_0, addr, read_mem, 255);
    }
}
