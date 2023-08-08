/**
******************************************************************************
* @file i2c_ll.с
* @author Дружинин А.А.
* @version v1.0
* @date  05-03-2021
* @brief Модуль i2c для работы с модулем интерфейсов.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT STC </center></h2>
******************************************************************************
*/


#include "../interface.h"

#define WRITE 0
#define READ 1

static i2c_error_t i2c_read_byte(i2c_t* const i2c, char* byte);
static i2c_error_t i2c_write_byte(i2c_t* const i2c, char* byte);
static i2c_error_t i2c_check_flag(const volatile uint32_t* reg, uint32_t mask, uint16_t time_value);
static i2c_error_t i2c_send_address(i2c_t* const i2c, char* address, uint8_t rw_bit);


// ========================================================================================================
/**
 * @brief Инициализация тактирования
 * @param i2c - структура данных i2c
 * @return none
 *
 */
static void i2c_init_rcc(i2c_t* const i2c)
{
    switch ((uint32_t)i2c->sfr)
    {

#ifdef SPI1
        case I2C1_BASE: __HAL_RCC_I2C1_CLK_ENABLE(); break;
#endif /* SPI1 */

#ifdef SPI2
        case I2C2_BASE: __HAL_RCC_I2C2_CLK_ENABLE(); break;
#endif /* SPI2 */

        default: break;
    }
}

// ========================================================================================================
/**
 * @brief Инициализация i2c
 * @param i2c - структура данных i2c
 * @return none
 *
 */
void i2c_init(i2c_t *const i2c)
{
    i2c->handler->Instance = i2c->sfr;
    HAL_I2C_DeInit(i2c->handler);
    i2c_init_rcc(i2c);

    HAL_I2C_Init(i2c->handler);
}

// ========================================================================================================
/**
 * @brief Отправка блока данных на устройство
 * @param i2c - структура данных i2c
 * @param src - указатель на буфер для передачи данных (адрес устройства + данные для отправки)
 * @param len - длина сообщения для отправки (адрес устройства + данные для отправки)
 * @return сообщение об ошибке
 */
i2c_error_t i2c_write_block(i2c_t* const i2c, char *src, size_t len)
{
    // Формируем старт условие
    i2c->sfr->CR1 |= I2C_CR1_START;

    // Проверка, что флаг старта выставлен
    if(i2c_check_flag(&(i2c->sfr->SR1), I2C_SR1_SB, i2c->timeout))
        return I2C_ERROR;

    // Отправляем адрес устройства, куда пойдут данные
    if(i2c_send_address(i2c, src, WRITE))
        return I2C_ERROR;

    // смещаем длину и данные
    len--;
    src++;

    // если адресация 10-битная еще раз смещаем длину и данные
    if (i2c->handler->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT)
    {
        len--;
        src++;
    }

    while(len)
    {
        // Пишем очередной байт
        if(i2c_write_byte(i2c, src++))
            return I2C_ERROR;

        len--;
    }

    // формируем стоп условие
    i2c->sfr->CR1 |= I2C_CR1_STOP;

    return I2C_ERROR_NONE;

}


// ========================================================================================================
/**
 * @brief Отправка байта данных на устройство
 * @param i2c - структура данных i2c
 * @param byte - указатель на байт данных
 * @return сообщение об ошибке
 */
static i2c_error_t i2c_write_byte(i2c_t* const i2c, char *byte)
{
    
    // В регистр данных складываем байт для передачи
    i2c->sfr->DR = *byte;

    // Проверяем поднят ли флаг завершения передачи
    if(i2c_check_flag(&i2c->sfr->SR1, I2C_SR1_BTF, i2c->timeout))
        return I2C_ERROR;

    return I2C_ERROR_NONE;
}

/**
 * @brief Чтение блока данных с устройства
 * @param i2c - структура данных i2c
 * @param src - указатель на буфер для приема данных
 * @param len - длина сообщения для приема
 * @return сообщение об ошибке
 */
i2c_error_t i2c_read_block(i2c_t* const i2c, char *src, size_t len)
{
    // Формируем старт условиеы
    i2c->sfr->CR1 |= I2C_CR1_START;

    // Проверка, что флаг старта выставлен
    if(i2c_check_flag(&i2c->sfr->SR1, I2C_SR1_SB, i2c->timeout))
        return I2C_ERROR;

    // Отправляем адрес устройства, с которого надо считать информацию
    if(i2c_send_address(i2c, src, READ))
        return I2C_ERROR;

    while(len)
    {
        // Читаем очередной байт
        if(i2c_read_byte(i2c, src++))
            return I2C_ERROR;

        len--;
    }

    // формируем стоп условие
    i2c->sfr->CR1 |= I2C_CR1_STOP;

    return I2C_ERROR_NONE;
}


// ========================================================================================================
/**
 * @brief Чтение байта данных
 * @param i2c - структура данных i2c
 * @param byte - указатель на байт данных
 * @return сообщение об ошибке
 */
static i2c_error_t i2c_read_byte(i2c_t* const i2c, char *byte)
{
    i2c->sfr->CR1 &= ~I2C_CR1_ACK;

    if(i2c_check_flag(&(i2c->sfr->SR1), I2C_SR1_RXNE, i2c->timeout))
        return I2C_ERROR;

    *byte = i2c->sfr->DR;

    return I2C_ERROR_NONE;
}


// ========================================================================================================
/**
 * @brief Отправка адреса устройства на линию
 * @param i2c - структура данных i2c
 * @param address - указатель на адрес устройства
 * @param rw_bit - бит чтения/записи
 * @return сообщение об ошибке
 */
static i2c_error_t i2c_send_address(i2c_t* const i2c, char* address, uint8_t rw_bit)
{
    // Если у устройства 7-битная адресация
    if (i2c->handler->Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
    {
        i2c->sfr->DR = (*address << 1) | rw_bit;
        // Проверяем поднят ли флаг передачи адреса
        if(i2c_check_flag(&(i2c->sfr->SR1), I2C_SR1_ADDR, i2c->timeout))
            return I2C_ERROR;
    }

    // Если у устройства 10-битная адресация
    else if (i2c->handler->Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT)
    {
        i2c->sfr->DR = ((*address << 1) | rw_bit) << 8 | *(address + 1);
        // Проверяем поднят ли флаг передачи адреса
        if(i2c_check_flag(&(i2c->sfr->SR1), I2C_SR1_ADD10, i2c->timeout))
            return I2C_ERROR;
    }

    // Сбрасываем флаг
    __HAL_I2C_CLEAR_ADDRFLAG(i2c->handler);

    return I2C_ERROR_NONE;
}


// ========================================================================================================
/**
 * @brief Проверка выставления флага
 * @param reg - указатель на регистр, где проверяется флаг
 * @param mask - маска проверки флага
 * @param time_value - время ожидания выставления флага
 * @return сообщение об ошибке
 */
static i2c_error_t i2c_check_flag(const volatile uint32_t *reg, uint32_t mask, uint16_t time_value)
{
    timeout_t i2c_timer;

    software_timer_start(&i2c_timer, time_value);
    while(!((*reg) & mask))
    {
        if(software_timer(&i2c_timer))
        {
            software_timer_stop(&i2c_timer);
            return I2C_ERROR;
        }
    }
    software_timer_stop(&i2c_timer);
    return I2C_ERROR_NONE;
}



#define I2C_ASSIGN(N)                                                   \
                                                                        \
                                                                        \
    static I2C_HandleTypeDef i2c##N##_handler;                          \
                                                                        \
    i2c_t i2c##N =                                                      \
    {                                                                   \
        (I2C_TypeDef*)I2C##N##_BASE,                                    \
        &i2c##N##_handler,                                              \
        0                                                               \
    }


// ========================================================================================================
// Генерация по надобности (указано в interface_conf)
// ========================================================================================================
#if defined I2C1_USE
I2C_ASSIGN(1);
#endif /* I2C1 */

// ========================================================================================================
#if defined I2C2_USE
I2C_ASSIGN(2);
#endif /* I2C2 */