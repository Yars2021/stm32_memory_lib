/*!
 * \file
 * В данном файле создаются интерфейсы, которые будут использоваться в проекте.
 * Для того, чтобы создать интерфейс - необходимо просто объявить для него буфер
 * \warning Внимание! Размер буфера должен быть степенью числа 2
 * \details дефайном INTERFACE_STATIC_MODE задается статичный режим работы модуля, т.е. создается конечный пул
 * элементов интерфейсов, количество которых задается с помощью дефайна INTERFACE_STATIC_MAX_INTERFACES
 * \details дефайном INTERFACE_DINAMIC_MODE задается динамический режим работы модуля, т.е. используется динамическое
 * выделение памяти под элементы интерфейса.
 */
#ifndef _INTERFACE_CONF_H_
#define _INTERFACE_CONF_H_

#if F1_CHECK
#include "stm32f1xx_hal.h"
#elif F3_CHECK
#include "stm32f3xx_hal.h"
#elif F4_CHECK
#include "stm32f4xx_hal.h"
#endif

#define INTERFACE_STATIC_MODE
//#define INTERFACE_DYNAMIC_MODE

#define INTERFACE_STATIC_MAX_INTERFACES                 10

#if !defined (INTERFACE_STATIC_MODE) && !defined (INTERFACE_DYNAMIC_MODE)
#error "Please select the interface work mode - dynamic or static"
#endif
#if defined (INTERFACE_STATIC_MODE) && defined (INTERFACE_DYNAMIC_MODE)
#error "Please select only one work mode of interface module"
#endif


#define SX1276_FSK1_TX_BUFFER_SIZE                      1024
#define SX1276_FSK1_RX_BUFFER_SIZE                      1024
#define SX1276_FSK1_SPI                                 SPI1
#define SX1276_FSK1_CSPORT                              GPIOA
#define SX1276_FSK1_CSPIN                               GPIO_PIN_4
#define SX1276_FSK1_RESETPORT                           GPIOB
#define SX1276_FSK1_RESETPIN                            GPIO_PIN_8


/*
#define SX1276_LORA1_TX_BUFFER_SIZE                     512
#define SX1276_LORA1_RX_BUFFER_SIZE                     512
#define SX1276_LORA1_SPI                                SPI1
#define SX1276_LORA1_CSPORT                             GPIOA
#define SX1276_LORA1_CSPIN                              GPIO_PIN_4
#define SX1276_LORA1_RESETPORT                          GPIOB
#define SX1276_LORA1_RESETPIN                           GPIO_PIN_8
*/
#define UART1_TX_BUFFER_SIZE                            256
#define UART1_RX_BUFFER_SIZE                            256

#define UART2_TX_BUFFER_SIZE                            256
#define UART2_RX_BUFFER_SIZE                            256

#define UART3_TX_BUFFER_SIZE                            512
#define UART3_RX_BUFFER_SIZE                            512


#endif /* _INTERFACE_CONF_H_ */
