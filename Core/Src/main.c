#define MAIN

#include "include.h"

I2C_HandleTypeDef hi2c1;
EEPROM_device_t eeprom_dev;

HAL_StatusTypeDef w_stat, r_stat;
char write_mem[12] = "hello world\0";
char read_mem[256] = {0};

size_t addr = 0x0010;

int main (void)
{
    initLowLevel();

    i2c_pins_init(&hi2c1);
    eeprom_device_init(&eeprom_dev, AT24C256, &hi2c1, 0x0);

    while (1)
    {
        w_stat = writemem(EEPROM_Memory, &eeprom_dev, addr, write_mem, 12);
        r_stat = readmem(EEPROM_Memory, &eeprom_dev, addr, read_mem, 11);
        HAL_Delay(100);
    }
}
