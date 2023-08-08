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

#ifndef _I2C_LL_H
#define _I2C_LL_H

typedef enum
{
    I2C_ERROR_NONE  = 0,
    I2C_ERROR       = 1
} i2c_error_t;

typedef struct
{
    I2C_TypeDef *const sfr;
    I2C_HandleTypeDef *const handler;
    uint16_t timeout;
} i2c_t;

extern i2c_t i2c1;
extern i2c_t i2c2;

void i2c_init (i2c_t* const i2c);

i2c_error_t i2c_write_block(i2c_t* const i2c, char *src, size_t len);

i2c_error_t i2c_read_block(i2c_t* const i2c, char *src, size_t len);



#endif //_I2C_LL_H