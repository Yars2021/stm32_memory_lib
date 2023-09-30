#ifndef _FLASH_MEM_H_
#define _FLASH_MEM_H_

#include "stm32f1xx_hal.h"

#include "memory.h"

HAL_StatusTypeDef flash_writemem(char *buff, size_t len, size_t addr);
HAL_StatusTypeDef flash_readmem(char *buff, size_t len, size_t addr);

#endif /* _FLASH_MEM_H_ */