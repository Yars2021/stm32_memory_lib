#include "eeprom_mem.h"

size_t eeprom_get_max_addr(EEPROM_device_model model)
{
    switch(model) {
        case AT24C01:
            return AT24C01_MAX_ADDR;
            break;
        case AT24C32:
            return AT24C32_MAX_ADDR;
            break;
        case AT24C64:
            return AT24C64_MAX_ADDR;
            break;
        case AT24C128:
            return AT24C128_MAX_ADDR;
            break;
        case AT24C256:
            return AT24C256_MAX_ADDR;
            break;
        case AT24C512:
            return AT24C512_MAX_ADDR;
            break;
    }
	return 0;
}

size_t eeprom_get_page_size(EEPROM_device_model model)
{
    switch(model) {
        case AT24C01:
            return AT24C01_PAGE_SIZE;
            break;
        case AT24C32:
            return AT24C32_PAGE_SIZE;
            break;
        case AT24C64:
            return AT24C64_PAGE_SIZE;
            break;
        case AT24C128:
            return AT24C128_PAGE_SIZE;
            break;
        case AT24C256:
            return AT24C256_PAGE_SIZE;
            break;
        case AT24C512:
            return AT24C512_PAGE_SIZE;
            break;
    }
	return 0;
}

HAL_StatusTypeDef eeprom_device_init(EEPROM_device_t *dev, EEPROM_device_model model, I2C_HandleTypeDef *i2c_handle, uint16_t i2c_addr)
{
    if (!dev) return HAL_ERROR;

    dev->Intreface.i2c_handle = i2c_handle;
    dev->Intreface.i2c_dev_addr = i2c_addr;
    dev->device_model = model;

    return HAL_OK;
}

HAL_StatusTypeDef eeprom_writemem(EEPROM_device_t *dev, uint8_t* buff, size_t len, size_t addr)
{
    uint8_t page_size = eeprom_get_page_size(dev->device_model);
	uint8_t first_page_remaining = page_size - addr % page_size;
	uint8_t num_of_pages = (len - first_page_remaining) / page_size;
	uint8_t last_page_remaining = (len - first_page_remaining) % page_size;

    if ((addr + first_page_remaining) >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;

    while (HAL_I2C_Mem_Write(dev->Intreface.i2c_handle,
                             0xA0 | (dev->Intreface.i2c_dev_addr << 1),
                             (uint16_t) addr,
                             I2C_MEMADD_SIZE_16BIT,
                             buff,
                             first_page_remaining,
                             1000) != HAL_OK);

	for (uint8_t current_page = 0; current_page < num_of_pages; current_page++) {
        if ((addr + first_page_remaining + current_page * page_size) >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;

        while (HAL_I2C_Mem_Write(dev->Intreface.i2c_handle,
                                 0xA0 | (dev->Intreface.i2c_dev_addr << 1),
                                 (uint16_t) addr + first_page_remaining + current_page * page_size,
                                 I2C_MEMADD_SIZE_16BIT,
                                 buff + first_page_remaining + current_page * page_size,
                                 page_size,
                                 1000) != HAL_OK);
	}

	if (last_page_remaining) {
        if ((addr + first_page_remaining + num_of_pages * page_size) >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;

        while (HAL_I2C_Mem_Write(dev->Intreface.i2c_handle,
                                 0xA0 | (dev->Intreface.i2c_dev_addr << 1),
                                 (uint16_t) addr + first_page_remaining + num_of_pages * page_size,
                                 I2C_MEMADD_SIZE_16BIT,
                                 buff + first_page_remaining + num_of_pages * page_size,
                                 last_page_remaining,
                                 1000) != HAL_OK);
	}

	return HAL_OK;
}

HAL_StatusTypeDef eeprom_readmem(EEPROM_device_t *dev, uint8_t* buff, size_t len, size_t addr)
{
    if (addr >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;
    
    while (HAL_I2C_Mem_Read(dev->Intreface.i2c_handle, 
                            0xA1 | (dev->Intreface.i2c_dev_addr << 1), 
                            (uint16_t) addr, 
                            I2C_MEMADD_SIZE_16BIT, 
                            buff, 
                            len, 
                            1000) != HAL_OK);

    return HAL_OK;
}