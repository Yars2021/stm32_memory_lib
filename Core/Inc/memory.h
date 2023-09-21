#ifndef _MEMORY_H_
#define _MEMORY_H_

typedef enum {
    Flash_Memory = 0,    // Flash mem
    EEPROM_Memory = 1    // EEPROM
} Device_type;

void init_mem(void);
HAL_StatusTypeDef readmem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len);
HAL_StatusTypeDef writemem(Device_type dev_t, void *device, size_t addr, char *buff, size_t len);

#endif /* _MEMORY_H_ */