#ifndef TL_PROTOCOL_H
#define TL_PROTOCOL_H

#include <stdint.h>
#include "data_transmission.h"
#include "crc8.h"
#include "software_timer.h"

#include <stdio.h>
#include "interface.h"

// Размер заголовка
#define TL_HEAD_SIZE               (9U)

#define TL_PREFIX                  (0x4F)

typedef enum
{
    TL_CONNECTION_REQUEST           =  0,                      // Запрос на обмен данными
    TL_CONNECTION_CONFIRM           =  1,                      // Подтверждение обмена данными
    TL_SEND_START                   =  2,                      // Начать передачу
    TL_SEND_DATA                    =  3,                      // Отправка данных
    TL_SEND_DATA_CONFIRM            =  4,                      // Получение данных подтверждено
    TL_SEND_STOP                    =  5,                      // Окончена передача данных
    TL_SEND_STOP_CONFIRM            =  6,                      // Подтверждение, что информация об окончании получена
    TL_ERROR_ACK                    =  7,                      // Неверные данные
} tl_opcode_t;

typedef enum
{
    TL_ERROR                        =  0,                      // Ошибка
    TL_PROCESS_SEND                 =  1,                      // Идет передача
    TL_PROCESS_END                  =  2,                      // Передача завершена
} tl_process_t;


typedef struct __attribute__((packed))
{
    uint8_t prefix;                                            // Префикс транспортного уровня протокола
    uint16_t address_src;                                      // Адрес источника данных
    uint16_t address_dest;                                     // Адрес назначения данных
    uint8_t opcode;                                            // Код операции
    uint16_t current_pckt;                                     // Текущий пакет
    uint8_t tl_crc;                                            // Контрольная сумма транспортного уровня
    dt_message_t dt_data;                                      // Данные
} tl_message_t;

typedef struct
{
    timeout_t* repeat_timer;                                   // Указатель на таймер для повторной отправки сообщения
    uint16_t timeout;                                          // Таймаут таймера
}tl_timer_t;

typedef struct
{
    int socket;                                                 // Сокет модуля интерфейсов
    buffer_data_t data_info;                                    // Информационная структура модуля data_transmission
    tl_timer_t timer;                                           // Структура таймера
    uint8_t device_id;                                          // ID текущего устройства
    uint16_t current_pckt;                                      // Текущий пакет
    tl_message_t out_msg;                                       // Структура для передачи данных текущего устройства
} tl_info_t;


parcer_status tl_process_rx_pckt(tl_message_t* msg);
void tl_repeat_pckt(tl_info_t* setting);
tl_opcode_t tl_write_data(tl_info_t* setting, tl_message_t* msg);
tl_process_t tl_protocol_parser_rx(tl_message_t* in_msg, tl_info_t* tl_setting);
tl_process_t tl_protocol_parser_tx(tl_message_t* in_msg, tl_info_t* tl_setting);
void tl_info_msg(tl_info_t* setting, tl_message_t* msg, tl_opcode_t info_msg);
void tl_send_request(tl_info_t* tl_setting, uint16_t dest_device_id);
void tl_set_tx_file(tl_info_t* tl_setting, uint8_t* out_data, size_t data_size);
void tl_set_rx_file(tl_info_t* tl_setting, uint8_t* buffer_data);
void tl_task(tl_info_t* tl_setting);

#endif //TRANSPORT_LEVEL_PROTOCOL_H
