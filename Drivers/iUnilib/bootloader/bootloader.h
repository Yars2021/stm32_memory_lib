//
// Created by Dizarker on 16.03.2021.
//

#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

// =============================================================================
// ===                   Настройки модуля bootloader                         ===
// =============================================================================
//! Для контроллеров серии F1 - общее количество страниц памяти
#define BOOTLOADER_F1_TOTAL_PAGES_NUMBER                    128

//! Для контроллеров серии F4 - общее количество секторов памяти
#define BOOTLOADER_F4_TOTAL_SECTORS_NUMBER                  11

//! Начальный адрес прошивки приложения
#define BOOTLOADER_APPLICATION_ADDRESS                      0x08004000
// =============================================================================

#include <stdint.h>
#include "cmsis_gcc.h"
#include "global_macro.h"
#include "crc_hw.h"

#if F1_CHECK
    #include "stm32f1xx_hal.h"
#elif F4_CHECK
    #include "stm32f4xx_hal.h"
#endif

#define BOOTLOADER_F1_PAGE_SIZE                             0x400
#define BOOTLOADER_F4_SECTOR0_SIZE                          0x4000
#define BOOTLOADER_F4_SECTOR1_SIZE                          0x4000
#define BOOTLOADER_F4_SECTOR2_SIZE                          0x4000
#define BOOTLOADER_F4_SECTOR3_SIZE                          0x4000
#define BOOTLOADER_F4_SECTOR4_SIZE                          0x10000
#define BOOTLOADER_F4_SECTOR5_AND_OTHER_SIZE                0x20000
#define BOOTLOADER_F4_SECTORSIZE_SAT                        5
#define BOOTLOADER_FLASH_START_ADDR                         0x08000000


typedef enum
{
    BOOTLOADER_NO_ERROR,
    BOOTLOADER_APPADDR_INVALID,
    BOOTLOADER_FWSIZE_INVALID,
    BOOTLOADER_UNLOCK_ERROR,
    BOOTOLOADER_FLASH_WRITE_ERROR,
    BOOTLOADER_CRC_ERROR
} bootloader_error_t;

typedef struct
{
    int start;
    int stop;
    size_t fwsize;
} bootloader_info_t;

typedef void (*pFunction)(void);

int                     bootloader_check_application                    (void);
void                    bootloader_run_application                      (void);
bootloader_error_t      bootloader_prepare_for_burn                     (size_t fwsize);
bootloader_error_t      bootloader_write_data                           (uint32_t offset, uint8_t *data, size_t size);
bootloader_error_t      bootloader_finalize                             (uint32_t crcExpected);

#endif /* _BOOTLOADER_H_ */
