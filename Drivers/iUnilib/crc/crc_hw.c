//
// Created by user on 09.03.2021.
//

#include "crc_hw.h"

void crc32hw_init(void)
{
    __HAL_RCC_CRC_CLK_ENABLE();
}


static uint32_t CRC_CalcCRC (uint32_t data)
{
    CRC->DR = data;
    return CRC->DR;
}

static uint32_t reverse_32 (uint32_t data)
{
    return __RBIT(data);
}

uint32_t crc32hw (const void *buf, int len, int clear)
{
    const uint32_t *p = (uint32_t*)buf;

    uint32_t crc;
    uint32_t crc_reg = 0xffffffff;
    if (clear) CRC->CR = CRC_CR_RESET;
    while (len >= 4)
    {
        uint32_t help = *p++;
        crc_reg = CRC_CalcCRC(reverse_32(help));
        len -= 4;
    }

    crc = reverse_32(crc_reg);

    if (len)
    {
        uint32_t test = CRC_CalcCRC(crc_reg);

        switch (len)
        {
            case 1:
                crc_reg = CRC_CalcCRC(reverse_32((*p & 0xff) ^ crc) >> 24);
                crc = (crc >> 8) ^ reverse_32(crc_reg);
                break;

            case 2:
                crc_reg = CRC_CalcCRC(reverse_32((*p & 0xffff) ^ crc) >> 16);
                crc = (crc >> 16) ^ reverse_32(crc_reg);
                break;

            case 3:
                crc_reg = CRC_CalcCRC(reverse_32((*p & 0xffffff) ^ crc) >> 8);
                crc = (crc >> 24) ^ reverse_32(crc_reg);
                break;

            default:
                break;

        }
    }

    return ~crc;
}

uint32_t crc32hw_append (uint32_t crc, const void *buf, uint32_t len)
{
    uint8_t *p = (uint8_t*)buf;

    CRC->CR = CRC_CR_RESET;

    uint32_t crc_reg = 0xffffffff;

    crc = ~crc;

    for (uint32_t i = 0; i < len; i++)
    {
        CRC_CalcCRC(crc_reg);
        uint32_t help = *p++;
        crc_reg = CRC_CalcCRC(reverse_32((help & 0xff) ^ crc) >> 24);
        crc = (crc >> 8) ^ reverse_32(crc_reg);
    }

    return ~crc;
}
