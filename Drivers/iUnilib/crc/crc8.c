#include "crc8.h"

//=============================================================================
static uint_fast8_t crc8_update(uint_fast8_t crc, uint_fast8_t data)
{
	static const uint_fast8_t CRC_POLY = 0x31;

	crc ^= data;

	for (int_fast8_t i=0; i<8; i++)
	{
        crc = (crc & 0x80) ?  (crc<<1)^CRC_POLY : crc<<1;
	}

	return (crc);
}

//=============================================================================
/*!
 * \brief Функция расчета контрольной суммы по алгоритму IBM CRC-8-ANSI
 * \details Функция расчитывает контрольную сумму с начального значения
 * \param[in] void* buf - указатель на буфер, откуда начнется расчет суммы
 * \param[in] size_t len - длина данных, которые подлежат обсчету
 * \return - возвращается значение контрольной суммы
 */
uint8_t crc8(const void* buf, size_t len)
{
	const uint8_t* ptr = buf;

	uint_fast8_t crc = 0xFFU;
 
    while (len--)
    {
		crc = crc8_update(crc, *ptr++);
    }
 
    return crc;
}

//=============================================================================
/*!
 * \brief Функция продолжения расчета контрольной суммы по алгоритму IBM CRC-8-ANSI
 * \details Функция расчитывает контрольную сумму с уже некоторой позиции
 * \param[in] uint8_t crc - начальное значение контрольной суммы
 * \param[in] void* buf - указатель на буфер, откуда начнется расчет суммы
 * \param[in] size_t len - длина данных, которые подлежат обсчету
 * \return - возвращается значение контрольной суммы
 */
uint8_t crc8_append(uint8_t crc, const void* buf, size_t len)
{
	const uint8_t* ptr = buf;

    while (len--)
	{
		crc = crc8_update(crc, *ptr++);
	}
 
    return (crc);
}
