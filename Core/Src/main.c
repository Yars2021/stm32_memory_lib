#define MAIN

#include "include.h"

I2C_HandleTypeDef hi2c1;
EEPROM_device_t eeprom_dev;

char write_mem[12] = "hello world\0";
char read_mem[256] = {0};

size_t addr = 0x0010;

void hi2c_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {

    }

}

int main (void)
{
    initLowLevel();

    hi2c_init();
    eeprom_device_init(&eeprom_dev, AT24C256, &hi2c1, 0x0);

    while (1)
    {
        writemem(EEPROM_Memory, &eeprom_dev, addr, write_mem, 12);
        readmem(EEPROM_Memory, &eeprom_dev, addr, read_mem, 11);
        HAL_Delay(100);
    }
}
