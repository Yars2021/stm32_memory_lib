/*
 *  crc8.h rev.2.2.0 2010-06-07
 *
 *  CRC-8-ANSI (IBM) algorithm:
 *
 *  Polynomial: x^8 + x^5 + x^4 + 1 (0x31)
 *  Initial value: 0xff
 *
 *  Check : 0xF7 ("123456789")
 *  MaxLen: 15 байт (127 бит) - обнаружение одинарных, двойных, тройных и всех нечетных ошибок
 *
 *  Contributors:
 *                Ivanov Anton (c) www.automatix.ru
 *
 */

#ifndef _CRC8_H_
#define _CRC8_H_

#include <stdlib.h>
#include <inttypes.h>

uint8_t crc8(const void* buf, size_t len);
uint8_t crc8_append(uint8_t crc, const void* buf, size_t len);

#endif 
