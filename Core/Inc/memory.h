#ifndef _MEMORY_H_
#define _MEMORY_H_

typedef enum {
    Flash_Memory = 0,    // Flash mem
    EEPROM_Memory = 1,    // EEPROM
    N25Q_Memory = 2 //NOR Flash
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
    struct {
        I2C_HandleTypeDef *i2c_handle;
        uint16_t i2c_dev_addr;
    } Interface;

    Device_type dev_t;
    EEPROM_device_model device_model;

} EEPROM_device_t;

typedef enum
{
	N25Q10,
	N25Q20,
	N25Q40,
	N25Q80,
	N25Q16,
	N25Q32,
	N25Q64,
	N25Q128,
	N25Q256,
	N25Q512,
	
}N25QXX_device_model;

typedef struct {
    struct {
        SPI_HandleTypeDef *spi_handle;
        GPIO_TypeDef *gpio_type;
        uint32_t gpio_pin;
    } Interface;

    Device_type dev_t;
    N25QXX_device_model device_model;
    uint8_t		UniqID[8];
	uint16_t	PageSize;
	uint32_t	PageCount;
	uint32_t	SectorSize;
	uint32_t	SectorCount;
	uint32_t	BlockSize;
	uint32_t	BlockCount;
	uint32_t	CapacityInKiloByte;
	uint8_t		StatusRegister1;
	uint8_t		StatusRegister2;
	uint8_t		StatusRegister3;	
	uint8_t		Lock;

} N25Q_device_t;

void init_mem(void);
HAL_StatusTypeDef readmem(void *device, size_t addr, char *buff, size_t len);
HAL_StatusTypeDef writemem(void *device, size_t addr, char *buff, size_t len);

#endif /* _MEMORY_H_ */