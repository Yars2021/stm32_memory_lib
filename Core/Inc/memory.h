#ifndef _MEMORY_H_
#define _MEMORY_H_

typedef enum {
    Flash_Memory = 0,    // Flash mem
    EEPROM_Memory = 1    // EEPROM
} Device_type;


typedef enum {
    AT24C01  = 1,
    AT24C32  = 32, 
    AT24C64  = 64, 
    AT24C128 = 128, 
    AT24C256 = 256, 
    AT24C512 = 512 
} EEPROM_device_model;

typedef struct {
    struct {
        I2C_HandleTypeDef *i2c_handle;
        uint16_t i2c_dev_addr;
    } Interface;

    EEPROM_device_model device_model;

} EEPROM_device_t;

void init_mem(void);
HAL_StatusTypeDef readmem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len);
HAL_StatusTypeDef writemem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len);

#endif /* _MEMORY_H_ */