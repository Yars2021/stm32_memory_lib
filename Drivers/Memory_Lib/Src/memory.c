#include "include.h"

#ifdef FLASH_MEM
    #include "flash_mem.h"

    Flash_device_t flash_dev;
#endif

#ifdef EEPROM_MEM
    #include "eeprom_mem.h"

    extern I2C_HandleTypeDef hi2c1;
    extern I2C_HandleTypeDef hi2c2;
#endif 

#ifdef N25Q_MEM
    #include "n25q_mem.h"

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

#ifdef N25Q_DEV_0
    N25Q_device_t n25q_dev_0;
#endif 

#ifdef N25Q_DEV_1
    N25Q_device_t n25q_dev_1;
#endif 

#ifdef N25Q_DEV_2
    N25Q_device_t n25q_dev_2;
#endif 

#ifdef N25Q_DEV_3
    N25Q_device_t n25q_dev_3;
#endif 

void init_flash(void) {
    flash_dev.dev_t = Flash_Memory;
}

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

void init_N25Q() {
    #ifdef N25Q_DEV_0
        N25Q_device_init(&n25q_dev_0, N25Q_DEV_0);
    #endif 

    #ifdef N25Q_DEV_1
        N25Q_device_init(&n25q_dev_1, N25Q_DEV_1);
    #endif 

    #ifdef N25Q_DEV_2
        N25Q_device_init(&n25qdev_2, N25Q_DEV_2);
    #endif 

    #ifdef N25Q_DEV_3
        N25Q_device_init(&n25q_dev_3, N25Q_DEV_3);
    #endif 
}

void init_mem(void)
{
    #ifdef FLASH_MEM
        init_flash();
    #endif
    #ifdef EEPROM_MEM
        init_eeprom();
    #endif
    #ifdef N25Q_MEM
        init_N25Q();
    #endif
}

HAL_StatusTypeDef readmem(void *device, size_t addr, char *buff, size_t len) {
    switch (((Flash_device_t*)device)->dev_t) {
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
            
        case N25Q_Memory:
            #ifdef N25Q_MEM
                return N25Q_readmem(device, buff, len, addr);
            #endif
            break;
        
        default:
            return HAL_OK;
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef writemem(void *device, size_t addr, char *buff, size_t len) {
    switch (((Flash_device_t*)device)->dev_t) {
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

        case N25Q_Memory:
            #ifdef N25Q_MEM
                return N25Q_writemem(device, buff, len, addr);
            #endif
            break;

        default:
            return HAL_OK;
    }
    return HAL_ERROR;
}