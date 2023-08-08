#include "include.h"

#define FIRMWARE_PAGE_OFFSET 	0x08003C00

void WriteToFlash(uint32_t Value)
{
	uint32_t pageAdr;

	pageAdr = FIRMWARE_PAGE_OFFSET;                    // Адрес страницы памяти

	HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef ef; // Объявляю структуру, необходимую для функции стирания страницы
    HAL_StatusTypeDef stat;
    ef.TypeErase = FLASH_TYPEERASE_PAGES; // Стирать постранично
    ef.PageAddress = pageAdr; // Адрес страницы для стирания
    ef.NbPages = 1; //Число страниц = 1
    uint32_t temp; // Временная переменная для результата стирания (не использую)
    HAL_FLASHEx_Erase(&ef, &temp); // Вызов функции стирания
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(pageAdr), (uint32_t)Value);
	HAL_FLASH_Lock();
}

void Flash_ReadParams(void) 
{
	uint32_t *source_adr = (uint32_t*)FIRMWARE_PAGE_OFFSET;   // Определяем адрес, откуда будем читать
	uint32_t dest_adr;                                        // Определяем адрес, куда будем писать
	
    dest_adr = *(__IO uint32_t*)(source_adr);  
}