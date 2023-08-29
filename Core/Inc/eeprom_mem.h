#ifndef _EEPROM_MEM_H_
#define _EEPROM_MEM_H_

#include "include.h"

HAL_StatusTypeDef eeprom_writemem(char* buff, size_t len, size_t addr);
HAL_StatusTypeDef eeprom_readmem(char* buff, size_t len, size_t addr);

#endif /* _EEPROM_MEM_H_ */