#include "include.h"

#include "flash_mem.h"
#include "eeprom_mem.h"

extern I2C_HandleTypeDef hi2c1;

#ifdef EEPROM_AT24C_I2C1
EEPROM_device_t eeprom_dev_i2c1;
#endif 

void init_mem(void)
{
    #ifdef EEPROM_AT24C_I2C1
    eeprom_device_init(&eeprom_dev_i2c1, EEPROM_AT24C_I2C1);
    #endif 
}

HAL_StatusTypeDef readmem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            return flash_readmem(buff, len, addr);
        case EEPROM_Memory:
            return eeprom_readmem((EEPROM_device_t*)device, buff, len, addr);
        
        default:
            return HAL_OK;
    }
}

HAL_StatusTypeDef writemem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            return flash_writemem(buff, len, addr);
        case EEPROM_Memory:
            return eeprom_writemem((EEPROM_device_t*)device, buff, len, addr);

        default:
            return HAL_OK;
    }
}