#ifndef _FLASH_MEM_H_
#define _FLASH_MEM_H_

#include <stdio.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"

#define FLASH_RESTRICTED_ADDR 0x0    // End address of the restricted zone (TODO: set an actual value)

HAL_StatusTypeDef flash_writemem(char *buff, size_t len, size_t addr);
HAL_StatusTypeDef flash_readmem(char *buff, size_t len, size_t addr);

#endif /* _FLASH_MEM_H_ */