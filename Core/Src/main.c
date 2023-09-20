#define MAIN

#include "include.h"

I2C_HandleTypeDef hi2c1;
EEPROM_device_t eeprom_dev;

HAL_StatusTypeDef w_stat, r_stat;

// write_long_mem   Нужен для проверки многостраничной записи
char write_short_mem[13] = "hello world\0";
char write_long_mem[256] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890987654321!@#$^&*()_++_)(*&^$#@!\0";
char read_mem[256] = {0};

size_t addr = 0xA;

void i2c_interface_init(I2C_HandleTypeDef *i2c_handle)
{
    if (!i2c_handle) return;

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9; // SCL | SDA
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    __HAL_AFIO_REMAP_I2C1_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();

    i2c_handle->Instance = I2C1;
    i2c_handle->Init.ClockSpeed = 100000;
    i2c_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2c_handle->Init.OwnAddress1 = 0;
    i2c_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_handle->Init.OwnAddress2 = 0;
    i2c_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    if (HAL_I2C_Init(i2c_handle) != HAL_OK)
    {

    }
}

void test_1_byte(void)
{
    w_stat = eeprom_write_byte(&eeprom_dev, write_short_mem, addr);
    HAL_Delay(250);
    r_stat = eeprom_read_byte(&eeprom_dev, read_mem, addr);
}

int main (void)
{
    initLowLevel();

    i2c_interface_init(&hi2c1);

    eeprom_device_init(&eeprom_dev, AT24C256, &hi2c1, 0x0010);

    while (1)
    {
        w_stat = writemem(EEPROM_Memory, &eeprom_dev, addr, write_long_mem, 93);
        HAL_Delay(250);
        r_stat = readmem(EEPROM_Memory, &eeprom_dev, addr, read_mem, 93);
        HAL_Delay(100);
    }
}
