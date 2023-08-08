#include "include.h"

HAL_StatusTypeDef flash_writemem(char* buff, size_t len, size_t addr)
{
	HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef ef; // Объявляю структуру, необходимую для функции стирания страницы
    HAL_StatusTypeDef stat;
    ef.TypeErase = FLASH_TYPEERASE_PAGES; // Стирать постранично
    ef.PageAddress = addr; // Адрес страницы для стирания
    ef.NbPages = 1; //Число страниц = 1
    uint32_t temp; // Временная переменная для результата стирания (не использую)
    HAL_FLASHEx_Erase(&ef, &temp); // Вызов функции стирания
    for(int i=0; i<len; i+=2){
        stat = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t)(addr + i), (uint16_t)(buff[i] | buff[i+1]<<8));
        if(stat != HAL_OK) {
            HAL_FLASH_Lock();
            return stat;
        }
    }
    if(len%2 == 1){
        uint16_t data = buff[len-1]<<8;
        stat = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t)(addr + len-1), (uint16_t)data);
        if(stat != HAL_OK) {
            HAL_FLASH_Lock();
            return stat;
        }
    }
    HAL_FLASH_Lock();
    return HAL_OK;
}

HAL_StatusTypeDef flash_readmem(char* buff, size_t len, size_t addr) 
{
	uint32_t *source_adr = (uint32_t*)addr;   // Определяем адрес, откуда будем читать
	uint32_t data;
	for(int i=0; i< len; i++){
        if(i%4 == 0)data = *(__IO uint32_t*)(source_adr + i/4);
        buff[i] = (data & 0xFF<< (8 * (i%4))) >> (8 * (i%4));
    }
    return HAL_OK;  
}

HAL_StatusTypeDef readmem(Device_type dev_t, char *buff, size_t len, size_t addr){
    switch (dev_t)
    {
    case Flash_Memory:
        return flash_readmem(buff, len, addr);
    
    default:
        break;
    }
}

HAL_StatusTypeDef writemem(Device_type dev_t, char *buff, size_t len, size_t addr){
    switch (dev_t)
    {
    case Flash_Memory:
        return flash_writemem(buff, len, addr);
    
    default:
        break;
    }
}