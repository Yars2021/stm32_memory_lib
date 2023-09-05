#include "eeprom_mem.h"

size_t min_data_len(size_t a, size_t b) 
{
    return a < b ? a : b;
}

void i2c_pins_init(I2C_HandleTypeDef *i2c_handle)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();
    HAL_Delay(100);
    __HAL_RCC_I2C1_FORCE_RESET();
    HAL_Delay(100);
    __HAL_RCC_I2C1_RELEASE_RESET();
    HAL_Delay(100);

    CLEAR_BIT(i2c_handle->Instance->CR1, I2C_CR1_PE);
    HAL_I2C_DeInit(i2c_handle);

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9; // SCL | SDA
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    SET_BIT(i2c_handle->Instance->CR1, I2C_CR1_SWRST);
    asm("nop");
    CLEAR_BIT(i2c_handle->Instance->CR1, I2C_CR1_SWRST);
    asm("nop");
    SET_BIT(i2c_handle->Instance->CR1, I2C_CR1_PE);
    asm("nop");
    

    __HAL_RCC_GPIOB_CLK_ENABLE();

    i2c_handle->Instance = I2C1;
    i2c_handle->Init.ClockSpeed = 100000;
    i2c_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2c_handle->Init.OwnAddress1 = 0;
    i2c_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_handle->Init.OwnAddress2 = 0;
    i2c_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    if (HAL_I2C_Init(i2c_handle) != HAL_OK)
    {

    }
}

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
    HAL_StatusTypeDef status = HAL_OK;

    uint8_t page_size = eeprom_get_page_size(dev->device_model);                            // Размер страницы для данной модели устройства
    uint8_t first_page_remaining = min_data_len(page_size - addr % page_size, len);         // Сколько данных запишем на 1-ю страницу
	uint8_t num_of_pages = (len - first_page_remaining) / page_size;                        // Количество страниц, на которые будут записаны данные (без учета 1-й и остатка)
	uint8_t last_page_remaining = (len - first_page_remaining) % page_size;                 // Сколько данных запишем на последнюю страницу (остаток)

    if ((addr + first_page_remaining) >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;

    status |= HAL_I2C_Mem_Write(dev->Intreface.i2c_handle,
                                0xA0 | (dev->Intreface.i2c_dev_addr << 1),
                                (uint16_t) addr,
                                I2C_MEMADD_SIZE_16BIT,
                                buff,
                                first_page_remaining,
                                1000);                                              // Записываем данные на 1-ю страницу

	for (uint8_t current_page = 0; current_page < num_of_pages; current_page++) {
        if ((addr + first_page_remaining + current_page * page_size) >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;

        status |= HAL_I2C_Mem_Write(dev->Intreface.i2c_handle,
                                    0xA0 | (dev->Intreface.i2c_dev_addr << 1),
                                    (uint16_t) addr + first_page_remaining + current_page * page_size,
                                    I2C_MEMADD_SIZE_16BIT,
                                    buff + first_page_remaining + current_page * page_size,
                                    page_size,
                                    1000);                                        // Записываем данные на страницу с номером current_page
	}

	if (last_page_remaining) {
        if ((addr + first_page_remaining + num_of_pages * page_size) >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;

        status |= HAL_I2C_Mem_Write(dev->Intreface.i2c_handle,
                                    0xA0 | (dev->Intreface.i2c_dev_addr << 1),
                                    (uint16_t) addr + first_page_remaining + num_of_pages * page_size,
                                    I2C_MEMADD_SIZE_16BIT,
                                    buff + first_page_remaining + num_of_pages * page_size,
                                    last_page_remaining,
                                    1000);                                         // Если остаток есть, записываем его на последнюю страницу
	}

	return status;
}

HAL_StatusTypeDef eeprom_readmem(EEPROM_device_t *dev, uint8_t* buff, size_t len, size_t addr)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (addr >= eeprom_get_max_addr(dev->device_model)) return HAL_ERROR;
    
    status = HAL_I2C_Mem_Read(dev->Intreface.i2c_handle, 
                              0xA1 | (dev->Intreface.i2c_dev_addr << 1), 
                              (uint16_t) addr, 
                              I2C_MEMADD_SIZE_16BIT, 
                              buff, 
                              len, 
                              1000);

    return status;
}