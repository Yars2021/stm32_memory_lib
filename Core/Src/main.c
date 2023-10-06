#define MAIN

#include "include.h"
#include "n25q_mem.h"

extern Flash_device_t flash_dev;
// extern EEPROM_device_t eeprom_dev_0;
extern N25Q_device_t n25q_dev_0;

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

    while (1)
    {
        // writemem(&flash_dev, 0x08011000, write_long_long_mem, 2048);
        // HAL_Delay(100);
        // readmem(&flash_dev, 0x08011000, read_mem, 2048);

        // writemem(&eeprom_dev_0, addr, write_long_mem, 255);
        // HAL_Delay(100);
        // readmem(&eeprom_dev_0, addr, read_mem, 255);

        writemem(&n25q_dev_0, addr, write_short_mem, 13);
        HAL_Delay(100);
        readmem(&n25q_dev_0, addr, read_mem, 13);
    }
}
