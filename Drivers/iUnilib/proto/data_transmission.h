#ifndef DATA_TRANSMISSION_H
#define DATA_TRANSMISSION_H

#include <stdint.h>
#include "endianes.h"
#include "CAssert.h"
#include "crc32_software.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

// Возможные размеры буферов
#define BUFF_SIZE_64                    64U
#define BUFF_SIZE_128                   128U
#define BUFF_SIZE_256                   256U
#define BUFF_SIZE_512                   512U
#define BUFF_SIZE_1024                  1024U
#define BUFF_SIZE_2048                  2048U
#define BUFF_SIZE_4096                  4096U

// Номер последнего байта crc
#define P_POS_CRC_END                   (8U)

// Размер информации добавляемой к сообщению (перфикс, операция и тд)
#define INFO_MSG_SIZE                   (9U)

// Используемый буфер
#define BUFFER_SIZE                     BUFF_SIZE_64

// Длина сообщения для передачи данных
#define P_MESSAGE_DATA_SIZE             (BUFFER_SIZE - (P_POS_CRC_END + INFO_MSG_SIZE))

typedef enum
{

    PROCESS_ERROR              = 0,                      // ошибка передачи
    PROCESS_SEND               = 1,                      // процесс передачи файла не завершен
    PROCESS_END                = 2                       // переданы все куски

} send_status;

typedef enum
{

    PRCR_STATUS_IGNORE         = 0,                      // не совпала длина или контрольная сумма
    PRCR_STATUS_ANSWER         = 1                       // все проверки совпали

} parcer_status;


// Коды ошибок
typedef enum
{

    ERROR_NONE                  = 0,                      // ошибки нет
    ERROR_EMPTY                 = 1,                      // указатель на файл отсутсвует
    ERROR_FILE_LEN              = 2,                      // ошибка, при запрашиваемой длине и смещении обращение будет выполнено к элементу за пределами массива

} dt_errors;


typedef struct __attribute__((packed))
{

    uint16_t request_offset;                              // смещение в запрошенном файле
    uint16_t request_size;                                // часть файла, которую необходимо передать

} dt_file_volume_t;

typedef struct __attribute__((packed))
{

    uint16_t full_len;                                    // полная длина файла
    uint16_t full_len_inv;                                // полная длина файла инвертированная

} dt_file_info_t;

typedef struct __attribute__((packed))
{

    dt_errors error;                                      // ошибка
    uint8_t preparation;                                  // флаг подготовки к передаче
    size_t result_offset;
    uint8_t *data_file;                                   // указатель на запрошенный файл
    uint16_t current_pack;                                // номер отправляемого пакета
    dt_file_volume_t file_volume;                         // информация о необходимом размере передачи
    dt_file_info_t file_info;                             // полный размер файла

} buffer_data_t;

typedef struct __attribute__((packed))
{

    uint16_t len;                                         // длина поля данных
    uint16_t len_inv;                                     // инверсия длины поля данных
    uint32_t crc32;                                       // контрольная сумма поля данных
    uint8_t data[P_MESSAGE_DATA_SIZE];                    // поле данных

} dt_message_t;


parcer_status dt_process_rx_pckt(dt_message_t *msg);
void dt_gluing_data(dt_message_t *msg, buffer_data_t *data_info);
send_status dt_write_data(dt_message_t *msg, buffer_data_t *data_info);
send_status dt_repeat_pckt(dt_message_t *msg, buffer_data_t *data_info);
void dt_confirm_data(dt_message_t *msg, buffer_data_t *data_info);
void dt_create_filesize_info(dt_message_t *msg, buffer_data_t *data_info);
void dt_process_info(void *data_info, dt_message_t *msg);




#ifdef __cplusplus
}
#endif

#endif //DATA_TRANSMISSION_H