#include "../interface.h"

/**
 * @brief Запись по i2c1
 * @param buffer - буфер данных для передачи
 * @param len - длина данных для передачи
 * @return длину сообщения при отсутствии ошибок и 0 при наличии
 */
ssize_t i2c1_write (char *buffer, size_t len)
{
    if(i2c_write_block(&i2c1, buffer, len))
        return 0;
    return len;
}

/**
 * @brief Чтение по i2c1
 * @param buffer - буфер для записи сообщения
 * @param len - необходимая длина для чтения
 * @return длину сообщения при отсутствии ошибок и 0 при наличии
 */
ssize_t i2c1_read (char *buffer, size_t len)
{
    if(i2c_write_block(&i2c1, buffer, 2) || i2c_read_block(&i2c1, buffer, len))
        return 0;
    return len;
}


/**
 * @brief Запись по i2c2
 * @param buffer - буфер данных для передачи
 * @param len - длина данных для передачи
 * @return длину сообщения при отсутствии ошибок и 0 при наличии
 */
ssize_t i2c2_write (char *buffer, size_t len)
{
    if(i2c_write_block(&i2c2, buffer, len))
        return 0;
    return len;
}


/**
 * @brief Чтение по i2c2
 * @param buffer - буфер для записи сообщения
 * @param len - необходимая длина для чтения
 * @return длину сообщения при отсутствии ошибок и 0 при наличии
 */
ssize_t i2c2_read (char *buffer, size_t len)
{
    if(i2c_write_block(&i2c2, buffer, 2) || i2c_read_block(&i2c2, buffer, len))
        return 0;
    return len;
}


/**
 * @brief Настройка i2c и вызов функции инициализации
 * @param i2c - структура данных i2c
 * @param opt - структура с настройками
 * @return none
 *
 */
void i2c_device_init(i2c_t* const i2c, struct termios *opt)
{
    i2c->timeout = opt->c_cc[V_I2C_TIMEOUT];
    i2c->handler->Instance = i2c->sfr;

    if (HAL_I2C_DeInit(i2c->handler) == HAL_ERROR) return;

    // Clock
    i2c->handler->Init.ClockSpeed = opt->c_cc[V_I2C_CLOCKSPEED];

    if(opt->c_cc[V_I2C_OWNADRESS_1])
        i2c->handler->Init.OwnAddress1 = opt->c_cc[V_I2C_OWNADRESS_1];

    if(opt->c_cc[V_I2C_OWNADRESS_2])
        i2c->handler->Init.OwnAddress1 = opt->c_cc[V_I2C_OWNADRESS_2];


    // AddressingMode
    if (opt->c_cc[V_I2C_ADDRESSINGMODE] == V_I2C_ADDRESSINGMODE_7BIT)
        i2c->handler->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    else if (opt->c_cc[V_I2C_ADDRESSINGMODE] == V_I2C_ADDRESSINGMODE_10BIT)
        i2c->handler->Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;

    // DutyCycle
    if (opt->c_cc[V_I2C_DUTYCYCLE] == V_I2C_DUTYCYCLE_2)
        i2c->handler->Init.DutyCycle = I2C_DUTYCYCLE_2;
    else if (opt->c_cc[V_I2C_DUTYCYCLE] == V_I2C_DUTYCYCLE_16_9)
        i2c->handler->Init.DutyCycle = I2C_DUTYCYCLE_16_9;

    // DualAddress
    if (opt->c_cc[V_I2C_DUALADDRESS] == V_I2C_DUALADDRESS_DISABLE)
        i2c->handler->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    else if (opt->c_cc[V_I2C_DUALADDRESS] == V_I2C_DUALADDRESS_ENABLE)
        i2c->handler->Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;

    // DualAddress
    if (opt->c_cc[V_I2C_GENERALCALL] == V_I2C_GENERALCALL_DISABLE)
        i2c->handler->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    else if (opt->c_cc[V_I2C_GENERALCALL] == V_I2C_GENERALCALL_ENABLE)
        i2c->handler->Init.GeneralCallMode = I2C_GENERALCALL_ENABLE;

    // Sretch
    if (opt->c_cc[V_I2C_NOSTRETCH] == V_I2C_NOSTRETCH_DISABLE)
        i2c->handler->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (opt->c_cc[V_I2C_NOSTRETCH] == V_I2C_NOSTRETCH_ENABLE)
        i2c->handler->Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;


    i2c_init(i2c);
}
