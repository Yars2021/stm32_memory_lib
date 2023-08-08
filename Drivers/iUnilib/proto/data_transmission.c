/**
******************************************************************************
* @file data_transmission.с
* @author Дружинин А.А.
* @version v1.0
* @date  19-01-2021
* @brief Модуль передачи данных.
* ****************************************************************************
*
* Передача файла происходит по следующему алгоритму:
* 1) Приемник запрашивает файл у передатчика
* 2) Передатик формирует сообщение с информацией о длине всего файла с помощью функции dt_create_filesize_info
* 3) Приемник обрабатывает сообщение с помощью функции dt_process_rx_pckt
* 4) Приемник складывает в информационную структуру buffer_data_t информацию о полной длине файла с помощью функции dt_process_info
* 5) Приемник отправляет сообщение с информацией о длине, которую необходимо ему выслать,
* а также, какое надо задать смещение в файле с помощью функции dt_confirm_data
* 6) Передатчик обрабатывает сообщение с помощью функции dt_process_rx_pckt
* 7) Передатчик складывает в информационную структуру buffer_data_t информацию о запрашиваемой длине и смещении с помощью функции dt_process_info
* 8) Передатчик начинает отправку сообщений с помощью функции dt_write_data в соответвии с устновленным размером буфера в h файле.
* Когда передача закончена, функция возвращает флаг PROCESS_END. В процессе передачи возвращается PROCESS_SEND.
* Если была обнаружина ошибка возвращается PROCESS_ERROR.
*
* Для проверки сообщения на целостность, используется функция process_packet. Которая возвращает значение:
* PRCR_STATUS_IGNORE - сообщение не прошло хотя бы одну проверку
* PRCR_STATUS_ANSWER - сообщение прошло все проверки
*
*
******************************************************************************
* @attention
* В h файле необходимо задать следующие настройки:
* 1) величину внешней обертки для отправляемого сообщения (перфикс, операция и тд), за это отвечает INFO_MSG_SIZE
* 2) размер буфера BUFFER_SIZE
* Перед использованием необходимо вызвать crc32sftwr_init()
*
* <h2><center>&copy; COPYRIGHT STC </center></h2>
******************************************************************************
*/

//================================================================
#include "data_transmission.h"

/**
* @brief обработка принятого сообщения
* @param msg принятые данные
* @return возвращает статус:
* PRCR_STATUS_IGNORE - сообщение не прошло хотя бы одну проверку
* PRCR_STATUS_ANSWER - сообщение прошло все проверки
*
*/
parcer_status dt_process_rx_pckt(dt_message_t *msg)
{

    uint32_t crc = 0;

    msg->len = read_word_lendian(&msg->len);
    msg->len_inv = read_word_lendian(&msg->len_inv);

    // Если длина не совпала возвращаем ошибку
    if(msg->len != (~(msg->len_inv)&0xffff))
        return PRCR_STATUS_IGNORE;


    crc = crc32_sftwr(0, msg->data, msg->len);
    msg->crc32 = read_dword_lendian(&msg->crc32);

    // Если crc не совпала возвращаем ошибку
    if(crc != msg->crc32)
        return PRCR_STATUS_IGNORE;

    return PRCR_STATUS_ANSWER;

}


void dt_gluing_data(dt_message_t *msg, buffer_data_t *data_info)
{
	memcpy(&data_info->data_file[data_info->result_offset],
		   msg->data,
		   msg->len);

	data_info->result_offset += msg->len;
	data_info->current_pack++;
}


/**
* @brief запись информации из буфера в информационную структуру file_volume_t или file_info_t
* @param msg принятые данные
* @param data_info информационная структура
* @return none
*
*/
void dt_process_info(void *data_info, dt_message_t *msg)
{

    memcpy(data_info, msg->data, msg->len);

}


/**
* @brief формирование информации о размере всего файла для отправки устройству, которое отправило запрос
* @details перед вызовом в информационной структуре необходимо задать указатель на файл, а также длину этого файла
* @param data_info структура с информацией о данных, которые необходимо отправить
* @param msg данные для передачи
*/
void dt_create_filesize_info(dt_message_t *msg, buffer_data_t *data_info)
{

    // Если данные для отправки отсутсвуют
    if(data_info->data_file == NULL)
    {
        data_info->error = ERROR_EMPTY;

        return;
    }

    memset(msg->data, 0, P_MESSAGE_DATA_SIZE);

    // обнуляем количество посланных пакетов
    data_info->current_pack = 0;

    data_info->error = ERROR_NONE;

    // формирование информации о длине всего файла
    data_info->file_info.full_len = read_word_lendian(&data_info->file_info.full_len);
    write_word_lendian(&data_info->file_info.full_len_inv, ~(data_info->file_info.full_len));
    write_dword_lendian(&msg->len, sizeof(data_info->file_info));
    write_dword_lendian(&msg->len_inv, (~(msg->len)&0xffff));
    memcpy(msg->data, &data_info->file_info, msg->len);
    write_dword_lendian(&msg->crc32, crc32_sftwr(0, msg->data, msg->len));
    data_info->result_offset = data_info->file_volume.request_offset;

    // подготовка со стороны передатчика выполнена
    data_info->preparation = 1;

}


/**
* @brief формирование части файла
* @param data_info структура с внутренними данными
* @param msg данные для передачи
* @return возвращает статус:
* PROCESS_ERROR - ошибка формирования посылки
* PROCESS_SEND - посылка сформирована, но файл не полностью отправлен
* PROCESS_END - посылка сформирована, файл полностью отправлен
*/
send_status dt_write_data(dt_message_t *msg, buffer_data_t *data_info)
{

    // если подготовка не была выполнена возвращаем ошибку
    if(data_info->preparation != 1)
        return PROCESS_ERROR;

    size_t packet_size = 0;

	data_info->result_offset = data_info->file_volume.request_offset + data_info->current_pack*P_MESSAGE_DATA_SIZE;

    data_info->current_pack++;

    if(data_info->file_volume.request_size > P_MESSAGE_DATA_SIZE)
    {
        packet_size = P_MESSAGE_DATA_SIZE;
        data_info->file_volume.request_size -= P_MESSAGE_DATA_SIZE;
    }
    else
    {
        memset(msg->data, 0, P_MESSAGE_DATA_SIZE);
        packet_size = data_info->file_volume.request_size;
        data_info->file_volume.request_size = 0;
    }

    // если будет обращение за пределы массива, возвращаем ошибку
    if((data_info->result_offset + packet_size) > data_info->file_info.full_len)
    {
        data_info->current_pack--;
        data_info->error = ERROR_FILE_LEN;
        return PROCESS_ERROR;
    }

    memcpy(msg->data, &data_info->data_file[data_info->result_offset], packet_size);
    write_dword_lendian(&msg->len, packet_size);
    write_dword_lendian(&msg->len_inv, (~msg->len&0xffff));
    write_dword_lendian(&msg->crc32, crc32_sftwr(0, msg->data, msg->len));

    // если не был передан весь файл, возвращаем флаг передачи
    if(data_info->file_volume.request_size>0)
        return PROCESS_SEND;

    // файл передан полностью, обнуляем значение подготовки и возвращаем флаг окончания передачи
    data_info->preparation = 0;
    return PROCESS_END;

}


/**
* @brief формирование подтверждения на отправку файла
* @param data_info структура с информацией о данных, которые необходимо отправить
* @param msg данные для передачи
* @return none
*/
void dt_confirm_data(dt_message_t *msg, buffer_data_t *data_info)
{

    // Если установлен необходимый размер для передачи равный нулю, то выставляем ошибку.
    if(data_info->file_volume.request_size == 0)
    {
        data_info->error = ERROR_EMPTY;
        return;
    }

    memset(msg->data, 0, P_MESSAGE_DATA_SIZE);

    // формирование информации о длине всего файла
    data_info->file_volume.request_offset = read_word_lendian(&data_info->file_volume.request_offset);
    data_info->file_volume.request_size = read_word_lendian(&data_info->file_volume.request_size);
    data_info->result_offset = 0;

    write_dword_lendian(&msg->len, sizeof(data_info->file_volume));
    write_dword_lendian(&msg->len_inv, (~msg->len&0xffff));
    memcpy(msg->data, &data_info->file_volume, msg->len);
    write_dword_lendian(&msg->crc32, crc32_sftwr(0, msg->data, msg->len));

	// Подготовка со стороны приемника выполнена
	data_info->preparation = 1;

    data_info->error = ERROR_NONE;

}

/**
 * @brief Повторная отправка пакета
 * @param data_info структура с информацией о данных, которые необходимо отправить
 * @param msg данные для передачи
 * @return none
 */
send_status dt_repeat_pckt(dt_message_t *msg, buffer_data_t *data_info)
{
    data_info->preparation = 1;
    data_info->file_volume.request_size += P_MESSAGE_DATA_SIZE;
    data_info->current_pack -= 1;
    data_info->result_offset -= (data_info->current_pack-1)*P_MESSAGE_DATA_SIZE;
    dt_write_data(msg, data_info);
}
