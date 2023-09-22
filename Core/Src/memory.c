#include "include.h"

#ifdef FLASH_MEM
    #include "flash_mem.h"
#endif

#ifdef EEPROM_MEM
    #include "eeprom_mem.h"

    extern I2C_HandleTypeDef hi2c1;
    extern I2C_HandleTypeDef hi2c2;
#endif 

#ifdef EEPROM_DEV_0
    EEPROM_device_t eeprom_dev_0;
#endif 

#ifdef EEPROM_DEV_1
    EEPROM_device_t eeprom_dev_0;
#endif 

#ifdef EEPROM_DEV_2
    EEPROM_device_t eeprom_dev_0;
#endif 

#ifdef EEPROM_DEV_3
    EEPROM_device_t eeprom_dev_0;
#endif 

void init_eeprom(void)
{
    #ifdef EEPROM_DEV_0
        eeprom_device_init(&eeprom_dev_0, EEPROM_DEV_0);
    #endif 

    #ifdef EEPROM_DEV_1
        eeprom_device_init(&eeprom_dev_1, EEPROM_DEV_1);
    #endif 

    #ifdef EEPROM_DEV_2
        eeprom_device_init(&eeprom_dev_2, EEPROM_DEV_2);
    #endif 

    #ifdef EEPROM_DEV_3
        eeprom_device_init(&eeprom_dev_3, EEPROM_DEV_3);
    #endif 
}

void init_mem(void)
{
    #ifdef EEPROM_MEM
        init_eeprom();
    #endif
}

HAL_StatusTypeDef readmem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            #ifdef FLASH_MEM
            return flash_readmem(buff, len, addr);
            #endif

        case EEPROM_Memory:
            #ifdef EEPROM_MEM
            return eeprom_readmem((EEPROM_device_t*)device, buff, len, addr);
            #endif
        
        default:
            return HAL_OK;
    }
}

HAL_StatusTypeDef writemem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            #ifdef FLASH_MEM
            return flash_writemem(buff, len, addr);
            #endif

        case EEPROM_Memory:
            #ifdef EEPROM_MEM
            return eeprom_writemem((EEPROM_device_t*)device, buff, len, addr);
            #endif

        default:
            return HAL_OK;
    }
}