#include "include.h"

#ifdef FLASH_MEM
    #include "flash_mem.h"
#endif

#ifdef EEPROM_MEM
    #include "eeprom_mem.h"

    extern I2C_HandleTypeDef hi2c1;
    extern I2C_HandleTypeDef hi2c2;
#endif 

#ifdef W25X_MEM
    #include "w25x_mem.h"

    extern SPI_HandleTypeDef spi;
#endif

#ifdef EEPROM_DEV_0
    EEPROM_device_t eeprom_dev_0;
#endif 

#ifdef EEPROM_DEV_1
    EEPROM_device_t eeprom_dev_1;
#endif 

#ifdef EEPROM_DEV_2
    EEPROM_device_t eeprom_dev_2;
#endif 

#ifdef EEPROM_DEV_3
    EEPROM_device_t eeprom_dev_3;
#endif 

#ifdef W25X_DEV_0
    W25x_device_t w25x_dev_0;
#endif 

#ifdef W25X_DEV_1
    W25x_device_t w25x_dev_1;
#endif 

#ifdef W25X_DEV_2
    W25x_device_t w25x_dev_2;
#endif 

#ifdef W25X_DEV_3
    W25x_device_t w25x_dev_3;
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

void init_w25x(){
    #ifdef W25X_DEV_0
        w25x_device_init(&w25x_dev_0, W25X_DEV_0);
    #endif 

    #ifdef W25X_DEV_1
        w25x_device_init(&w25x_dev_1, W25X_DEV_1);
    #endif 

    #ifdef W25X_DEV_2
        w25x_device_init(&w25xdev_2, W25X_DEV_2);
    #endif 

    #ifdef W25X_DEV_3
        w25x_device_init(&w25x_dev_3, W25X_DEV_3);
    #endif 
}

void init_mem(void)
{
    #ifdef EEPROM_MEM
        init_eeprom();
    #endif
    #ifdef W25X_MEM
        init_w25x();
    #endif
}

HAL_StatusTypeDef readmem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            #ifdef FLASH_MEM
                return flash_readmem(buff, len, addr);
            #endif
            break;

        case EEPROM_Memory:
            #ifdef EEPROM_MEM
                return eeprom_readmem((EEPROM_device_t*)device, buff, len, addr);
            #endif
            break;
        case W25x_Memory:
            #ifdef W25X_MEM
                return w25x_readmem(device, buff, len, addr);
            #endif
            break;
        
        default:
            return HAL_OK;
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef writemem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len) {
    switch (dev_t) {
        case Flash_Memory:
            #ifdef FLASH_MEM
                return flash_writemem(buff, len, addr);
            #endif
            break;

        case EEPROM_Memory:
            #ifdef EEPROM_MEM
                return eeprom_writemem((EEPROM_device_t*)device, buff, len, addr);
            #endif
            break;
        case W25x_Memory:
            #ifdef W25X_MEM
                return w25x_writemem(device, buff, len, addr);
            #endif
            break;

        default:
            return HAL_OK;
    }
    return HAL_ERROR;
}