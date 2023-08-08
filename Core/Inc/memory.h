#ifndef _MEMORY_H_
#define _MEMORY_H_

typedef enum    {
    Flash_Memory = 0
}   Device_type;

HAL_StatusTypeDef readmem(Device_type dev_t, char *buff, size_t len);
HAL_StatusTypeDef writemem(Device_type dev_t, char *buff, size_t len);

void Flash_ReadParams(void);
void WriteToFlash(uint32_t Value);

#endif /* _MEMORY_H_ */