#define MAIN

#include "include.h"

extern I2C_HandleTypeDef hi2c1;

EEPROM_device_t eeprom_dev;

HAL_StatusTypeDef w_stat, r_stat;

// write_long_mem   Нужен для проверки многостраничной записи
char write_short_mem[13] = "hello world\0";
char write_long_mem[256] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890987654321!@#$^&*()_++_)(*&^$#@!\0";
char read_mem[256] = {0};

size_t addr = 0xA;

void test_1_byte(void)
{
    w_stat = eeprom_write_byte(&eeprom_dev, write_short_mem, addr);
    HAL_Delay(250);
    r_stat = eeprom_read_byte(&eeprom_dev, read_mem, addr);
}

int main (void)
{
    initLowLevel();

    eeprom_device_init(&eeprom_dev, AT24C256, &hi2c1, 0x0010);

    while (1)
    {
        w_stat = writemem(EEPROM_Memory, &eeprom_dev, addr, write_long_mem, 93);
        HAL_Delay(250);
        r_stat = readmem(EEPROM_Memory, &eeprom_dev, addr, read_mem, 93);
        HAL_Delay(100);
    }
}
