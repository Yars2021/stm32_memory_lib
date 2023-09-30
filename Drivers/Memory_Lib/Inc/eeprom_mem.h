#ifndef _EEPROM_MEM_H_
#define _EEPROM_MEM_H_

#include "stm32f1xx_hal.h"

#include "memory.h"

#define EEPROM_BASE_ADDR_R	0xA1
#define EEPROM_BASE_ADDR_W	0xA0

#define AT24C01_MAX_ADDR    0x007F
#define AT24C32_MAX_ADDR    0x0FFF
#define AT24C64_MAX_ADDR    0x1FFF
#define AT24C128_MAX_ADDR   0x3FFF
#define AT24C256_MAX_ADDR   0x7FFF
#define AT24C512_MAX_ADDR   0xFFFF

#define AT24C01_PAGE_SIZE   8
#define AT24C32_PAGE_SIZE   32
#define AT24C64_PAGE_SIZE   32
#define AT24C128_PAGE_SIZE  64
#define AT24C256_PAGE_SIZE  64
#define AT24C512_PAGE_SIZE  128

size_t eeprom_get_max_addr(EEPROM_device_model model);
size_t eeprom_get_page_size(EEPROM_device_model model);
HAL_StatusTypeDef eeprom_device_init(EEPROM_device_t *dev, EEPROM_device_model model, I2C_HandleTypeDef *i2c_handle, uint16_t i2c_addr);
HAL_StatusTypeDef eeprom_write_byte(EEPROM_device_t *dev, uint8_t *buff, size_t addr);
HAL_StatusTypeDef eeprom_read_byte(EEPROM_device_t *dev, uint8_t *buff, size_t addr);
HAL_StatusTypeDef eeprom_writemem(EEPROM_device_t *dev, uint8_t *buff, size_t len, size_t addr);
HAL_StatusTypeDef eeprom_readmem(EEPROM_device_t *dev, uint8_t *buff, size_t len, size_t addr);

#endif /* _EEPROM_MEM_H_ */