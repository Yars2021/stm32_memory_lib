#include "include.h"

#include "flash_mem.h"
#include "eeprom_mem.h"

HAL_StatusTypeDef readmem(Device_type dev_t, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            return flash_readmem(buff, len, addr);
        
        default:
            return HAL_OK;
    }
}

HAL_StatusTypeDef writemem(Device_type dev_t, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            return flash_writemem(buff, len, addr);
        
        default:
            return HAL_OK;
    }
}