#include "crc32_software.h"

/*
*	2015.05.26 - v.1.
*	The source code take from: 
*	http://we.easyelectronics.ru/STM32/crc32-na-stm32-kak-na-kompe-ili-na-kompe-kak-na-stm32.html
*/

static uint32_t crc32r_table[256];


#define CRC32_POLY_R (0xEDB88320U)

//=============================================================================
/*!
 * \brief Функция подготовки программного модуля расчета контрольной суммы CRC32 STM32
 * \details Функция заполняет таблицы остатков для быстрого расчета суммы
 */
void crc32sftwr_init(void)
{
	uint32_t cr, i, j;

	for (i = 0; i < 256; ++i)
	{
		cr = i;
		for (j = 8; j > 0; --j)
		{
			cr = cr & 0x00000001 ? (cr >> 1) ^ CRC32_POLY_R : (cr >> 1);
		}
		crc32r_table[i] = cr;
	}
}

//=============================================================================
/*!
 * \brief Функция расчета контрольной суммы по алгоритму CRC-32 STM32
 * \details Программная реализация расчета контрольной суммы CRC32 также, как это
 * делает аппаратный модуль расчета
 * \param[in] начальное состояние контрольной суммы (для сброса передается ноль)
 * \param[in] uint8_t* buf - указатель на буфер, откуда начнется расчет суммы
 * \param[in] size_t len - длина данных, которые подлежат обсчету
 * \return - возвращается значение контрольной суммы
 */
uint32_t crc32_sftwr(uint32_t init_crc, const uint8_t *buf, size_t len)
{
	uint32_t v;
	uint32_t crc;
	crc = ~init_crc;
	while(len > 0)
	{
		v = *buf++;
		crc = ( crc >> 8 ) ^ crc32r_table[( crc ^ (v ) ) & 0xff];
		len--;
	}
	return ~crc;
}
