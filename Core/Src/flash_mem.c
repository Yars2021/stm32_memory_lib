#include "flash_mem.h"

HAL_StatusTypeDef flash_writemem(char* buff, size_t len, size_t addr)
{
    //if (addr < FLASH_RESTRICTED_ADDR) return HAL_ERROR;

    HAL_StatusTypeDef hal_status;

	HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase_init;              // Объявляю структуру, необходимую для функции стирания страницы
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;   // Стирать постранично
    erase_init.PageAddress = addr;                  // Адрес страницы для стирания
    erase_init.NbPages = 1;                         // Число страниц = 1

    uint32_t temp;
    HAL_FLASHEx_Erase(&erase_init, &temp);

    for (size_t i = 0; i < len; i += 2) {
        hal_status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t)(addr + i), (uint16_t)(buff[i] | buff[i + 1] << 8));
        if (hal_status != HAL_OK) {
            HAL_FLASH_Lock();
            return hal_status;
        }
    }

    if (len % 2 == 1) {
        uint16_t data = buff[len - 1] << 8;
        hal_status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t)(addr + len - 1), (uint16_t)data);
        if (hal_status != HAL_OK) {
            HAL_FLASH_Lock();
            return hal_status;
        }
    }

    HAL_FLASH_Lock();

    return HAL_OK;
}

HAL_StatusTypeDef flash_readmem(char* buff, size_t len, size_t addr) 
{
    // if (addr < FLASH_RESTRICTED_ADDR) return HAL_ERROR;

	uint32_t *source_adr = (uint32_t*) addr;   // Определяем адрес, откуда будем читать
	uint32_t data;

	for (size_t i = 0; i < len; i++) {
        if (i % 4 == 0) data = *(__IO uint32_t*)(source_adr + i / 4);
        buff[i] = (data & 0xFF << (8 * (i % 4))) >> (8 * (i % 4));
    }

    return HAL_OK;  
}