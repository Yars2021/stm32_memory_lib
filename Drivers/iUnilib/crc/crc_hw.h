//
// Created by user on 09.03.2021.
//

#ifndef _CRC_HW_H
#define _CRC_HW_H

#include "stm32f1xx_hal.h"

void crc32hw_init		(void);
uint32_t crc32hw		(const void *buf, int len, int clear);
uint32_t crc32hw_append	(uint32_t crc, const void *buf, uint32_t len);

#endif /* _CRC_HW_H */
