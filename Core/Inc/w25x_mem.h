#ifndef _NORFLASH_MEM_H_
#define _NORFLASH_MEM_H_

#include <stdio.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"

#include "memory.h"

#define _W25QXX_USE_FREERTOS     0

#define INIT_DEBUG               1

#define W25_WRITE_DISABLE     0x04
#define W25_WRITE_ENABLE      0x06
#define W25_CHIP_ERASE        0xC7 //0x60
#define W25_SECTOR_ERASE      0x20
#define W25_BLOCK_ERASE       0xD8
#define W25_FAST_READ         0x0B
#define W25_PAGE_PROGRAMM     0x02
#define W25_GET_JEDEC_ID      0x9F
#define W25_READ_STATUS_1     0x05
#define W25_READ_STATUS_2     0x35
#define W25_READ_STATUS_3     0x15
#define W25_WRITE_STATUS_1    0x01
#define W25_WRITE_STATUS_2    0x31
#define W25_WRITE_STATUS_3    0x11
#define W25_READ_UNIQUE_ID    0x4B

#define W25QXX_DUMMY_BYTE        0xA5

HAL_StatusTypeDef w25x_device_init(W25x_device_t *dev, SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *gpio_type, uint32_t gpio_pin);
HAL_StatusTypeDef w25x_writemem(W25x_device_t *dev, uint8_t *buff, size_t len, size_t addr);
HAL_StatusTypeDef w25x_readmem(W25x_device_t *dev, uint8_t *buff, size_t len, size_t addr);

#endif