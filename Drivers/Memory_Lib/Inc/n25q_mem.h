#ifndef _NORFLASH_MEM_H_
#define _NORFLASH_MEM_H_

#include <stdio.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"

#include "memory.h"
#include "n25q512a.h"

HAL_StatusTypeDef N25Q_device_init(N25Q_device_t *dev, SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *gpio_type, uint32_t gpio_pin);
HAL_StatusTypeDef N25Q_writemem(N25Q_device_t *dev, uint8_t *buff, size_t len, size_t addr);
HAL_StatusTypeDef N25Q_readmem(N25Q_device_t *dev, uint8_t *buff, size_t len, size_t addr);

#endif