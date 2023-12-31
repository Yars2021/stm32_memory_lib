#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "mem_cfg.h"

typedef enum {
    Flash_Memory    = 0,    // Flash mem
    EEPROM_Memory   = 1,    // EEPROM
    N25Q_Memory     = 2     //NOR Flash
} Device_type;

typedef struct {

    Device_type dev_t;

} Flash_device_t;


typedef enum {
    AT24C01  = 1,
    AT24C32  = 32, 
    AT24C64  = 64, 
    AT24C128 = 128, 
    AT24C256 = 256, 
    AT24C512 = 512 
} EEPROM_device_model;

typedef struct {
    Device_type dev_t;
    struct {
        I2C_HandleTypeDef *i2c_handle;
        uint16_t i2c_dev_addr;
    } Interface;
    
    EEPROM_device_model device_model;

} EEPROM_device_t;


typedef enum
{
	N25Q128,
	N25Q256,
	N25Q512
} N25QXX_device_model;

typedef struct {
    Device_type dev_t;
    struct {
        SPI_HandleTypeDef *spi_handle;
        GPIO_TypeDef *gpio_type;
        uint32_t gpio_pin;
    } Interface;
    
    N25QXX_device_model device_model;
    uint8_t		UniqID[8];
	uint16_t	PageSize;
	uint32_t	PageCount;
	uint32_t	SectorSize;
	uint32_t	SectorCount;
	uint32_t	SubSectorSize;
	uint32_t	SubSectorCount;
	uint32_t	CapacityInKiloByte;
	uint8_t		StatusRegister1;
	uint8_t		StatusRegister2;
	uint8_t		StatusRegister3;	
	uint8_t		Lock;

} N25Q_device_t;

#ifdef FLASH_MEM
    extern Flash_device_t flash_dev;
#endif

#ifdef EEPROM_DEV_0
    extern EEPROM_device_t eeprom_dev_0;
#endif 

#ifdef EEPROM_DEV_1
    extern EEPROM_device_t eeprom_dev_1;
#endif 

#ifdef EEPROM_DEV_2
    extern EEPROM_device_t eeprom_dev_2;
#endif 

#ifdef EEPROM_DEV_3
    extern EEPROM_device_t eeprom_dev_3;
#endif 

#ifdef N25Q_DEV_0
    extern N25Q_device_t n25q_dev_0;
#endif 

#ifdef N25Q_DEV_1
    extern N25Q_device_t n25q_dev_1;
#endif 

#ifdef N25Q_DEV_2
    extern N25Q_device_t n25q_dev_2;
#endif 

#ifdef N25Q_DEV_3
    extern N25Q_device_t n25q_dev_3;
#endif 

void init_mem(void);
HAL_StatusTypeDef readmem(void *device, size_t addr, char *buff, size_t len);
HAL_StatusTypeDef writemem(void *device, size_t addr, char *buff, size_t len);

#endif /* _MEMORY_H_ */