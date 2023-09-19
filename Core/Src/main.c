#define MAIN

#include "include.h"

I2C_HandleTypeDef hi2c1;
EEPROM_device_t eeprom_dev;

HAL_StatusTypeDef w_stat, r_stat;
char write_mem[12] = "hello world\0";
char read_mem[256] = {0};

size_t addr = 0x00AA;

int main (void)
{
    initLowLevel();

    i2c_pins_init(&hi2c1);
    eeprom_device_init(&eeprom_dev, AT24C256, &hi2c1, 0x0010);

    while (1)
    {
        w_stat = eeprom_write_byte(&eeprom_dev, write_mem, addr);
        r_stat = eeprom_read_byte(&eeprom_dev, read_mem, addr);
        HAL_Delay(100);
    }
}
