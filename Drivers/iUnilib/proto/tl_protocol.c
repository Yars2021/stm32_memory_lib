/**
******************************************************************************
* @file tl_protocol.с
* @author Дружинин А.А.
* @version v1.0
* @date  19-01-2021
* @brief Транспортный уровень передачи данных.
* ****************************************************************************
*
* Для использования со стороны передатчика:
* 1) Определить структуру с настройками tl_info_t (socket, device_id, out_msg);
* 2) Устанавливаем файл для передачи через tl_set_tx_file;
* 3) При получении сообщения вызываем tl_protocol_parser_tx
*
* Для использования со стороны приемника:
* 1) Определить структуру с настройками tl_info_t (socket, device_id, timer, out_msg);
* 2) Установить буфер, куда будет складываться файл передачи после всех обработок;
* 3) Отправляем запрос на подключение через tl_send_request
* 4) При получении сообщения вызываем tl_protocol_parser_rx
*
******************************************************************************
* @attention
* В данном протоколе используется модуль data_transmission, перед использованием необходимо:
* 1) В h файле data_transmission задать величину внешней обертки (перфикс, операция и тд) через INFO_MSG_SIZE
* 2) размер буфера BUFFER_SIZE
* Перед использованием необходимо вызвать crc32sftwr_init()
* <h2><center>&copy; COPYRIGHT STC </center></h2>
******************************************************************************
*/
#include "tl_protocol.h"

static void tl_gen_header(tl_message_t* msg, uint16_t src, uint16_t dest, tl_opcode_t opcode, uint16_t num_pckt);

/**
* @brief Обработка сообщения со стороны передающего устройства
* @param[in] in_msg - указатель на поступившее сообщение
* @param[in] tl_setting - указатель на структуру с настройками
* @return Статус передачи:\n
*TL_ERROR - ошибка передачи\n
*TL_PROCESS_SEND - идет передача\n
*TL_PROCESS_END - передача завершена\n
*/
tl_process_t tl_protocol_parser_tx(tl_message_t* in_msg, tl_info_t* tl_setting) {

	// Проверяем, что сообщение для данного устройства и используется транспортный уровень протокола
	if ((in_msg->address_src == tl_setting->device_id) && (in_msg->prefix == TL_PREFIX)) {

		// Проверяем совпала ли контрольная сумма
		if (!(tl_process_rx_pckt(in_msg)))
			tl_info_msg(tl_setting, in_msg, TL_ERROR_ACK);

		// Если пакет с таким номером уже был отправлен, просто повторяем сообщение
		else if(in_msg->current_pckt == tl_setting->current_pckt)
			tl_repeat_pckt(tl_setting);

		else {
			switch (in_msg->opcode) {
			// Пришел запрос на отправку данных
			case TL_CONNECTION_REQUEST:
				tl_setting->current_pckt = 0;
				// Формируем подтверждение на подключение
				tl_info_msg(tl_setting, in_msg, TL_CONNECTION_CONFIRM);
				break;

				// Пришло подтверждение полученных данных
			case TL_SEND_DATA_CONFIRM:

				// Отправляем пакет данных, если нет ошибок
				if (tl_write_data(tl_setting, in_msg) == TL_ERROR_ACK)
				{
					tl_info_msg(tl_setting, in_msg, TL_ERROR_ACK);
					return TL_ERROR;
				}
				break;

				// Подтверждение, что последняя посылка получена
			case TL_SEND_STOP_CONFIRM:
				return TL_PROCESS_END;
				break;

				// Ошибка передачи
			case TL_ERROR_ACK:
				tl_repeat_pckt(tl_setting);
				break;

			default:
				break;
			}
			return TL_PROCESS_SEND;
		}
	}

	return TL_ERROR;
}


/**
* @brief Обработка сообщения со стороны принимающего устройства
* @param[in] in_msg - указатель на поступившее сообщение
* @param[in] tl_setting - указатель на структуру с настройками
* @return Статус передачи:\n
*TL_ERROR - ошибка передачи\n
*TL_PROCESS_SEND - идет передача\n
*TL_PROCESS_END - передача завершена\n
*/
tl_process_t tl_protocol_parser_rx(tl_message_t* in_msg, tl_info_t* tl_setting)
{

	// Проверяем, что сообщение для данного устройства и используется транспортный уровень протокола
	if((in_msg->address_src == tl_setting->device_id) && (in_msg->prefix == TL_PREFIX))
	{

		// Проверяем совпала ли контрольная сумма
		if(!(tl_process_rx_pckt(in_msg)))
			tl_info_msg(tl_setting, in_msg, TL_ERROR_ACK);


		else {
			switch (in_msg->opcode) {

			// Пришло подтверждение готовности отправить данные
			case TL_CONNECTION_CONFIRM:
				// Сбрасываем таймер приемника
				software_timer_start(tl_setting->timer.repeat_timer, tl_setting->timer.timeout);
				tl_setting->data_info.result_offset = 0;
				// Отправляет сообщение о старте передачи
				tl_info_msg(tl_setting, in_msg, TL_SEND_DATA_CONFIRM);
				break;

				// Пришла очередная посылка данных
			case TL_SEND_DATA:
				// Сбрасываем таймер приемника
				software_timer_start(tl_setting->timer.repeat_timer, tl_setting->timer.timeout);

				// Обрабатываем сообщение
				if(dt_process_rx_pckt(&in_msg->dt_data) == PRCR_STATUS_IGNORE)
				{
					// Отправляем сообщение об ошибке
					tl_info_msg(tl_setting, in_msg, TL_ERROR_ACK);
					return TL_ERROR;
				}
				dt_gluing_data(&in_msg->dt_data, &tl_setting->data_info);

				// Если ошибки нет, отправляем подтверждение о получении
				tl_info_msg(tl_setting, in_msg, TL_SEND_DATA_CONFIRM);

				break;


				// Пришла последняя посылка данных
			case TL_SEND_STOP:
				// Останавливаем таймер приемника
				software_timer_stop(tl_setting->timer.repeat_timer);

				if(dt_process_rx_pckt(&in_msg->dt_data) == PRCR_STATUS_IGNORE)
				{
					// Отправляем сообщение об ошибке
					tl_info_msg(tl_setting, in_msg, TL_ERROR_ACK);
					return TL_ERROR;
				}
				dt_gluing_data(&in_msg->dt_data, &tl_setting->data_info);

				// Формируем подтверждение окончания передачи
				tl_info_msg(tl_setting, in_msg, TL_SEND_STOP_CONFIRM);

				return TL_PROCESS_END;
				break;

				// Ошибка передачи
			case TL_ERROR_ACK:
				// Обновляем таймер
				software_timer_start(tl_setting->timer.repeat_timer, tl_setting->timer.timeout);
				tl_repeat_pckt(tl_setting);
				break;

			default:
				break;
			}
			return TL_PROCESS_SEND;
		}
	}

	return TL_ERROR;

}

/**
* @brief Формирование заголовка
* @param[in] msg - указатель на сообщение для отправки
* @param[in] src - от кого передаем
* @param[in] dest - кому передаем
* @param[in] opcode - код операции:\n
*TL_CONNECTION_REQUEST - Запрос на обмен данными\n
*TL_CONNECTION_CONFIRM - Подтверждение обмена данными\n
*TL_SEND_START - Начать передачу\n
*TL_SEND_DATA - Отправка данных\n
*TL_SEND_DATA_CONFIRM - Получение данных подтверждено\n
*TL_SEND_STOP - Окончена передача данных\n
*TL_SEND_STOP_CONFIRM - Подтверждение, что информация об окончании получена\n
*TL_ERROR_ACK - Неверные данные
* @param[in] num_pckt - номер пакета
*/
static void tl_gen_header(tl_message_t* msg, uint16_t src, uint16_t dest, tl_opcode_t opcode, uint16_t num_pckt)
{
	msg->prefix = TL_PREFIX;
	msg->address_dest = dest;
	msg->address_src = src;
	msg->opcode = opcode;
	msg->current_pckt = num_pckt;
	msg->tl_crc = (uint8_t)crc8(&msg->prefix, TL_HEAD_SIZE-1);
}

/**
* @brief Формирование заголовка
* @param[in] in_msg - указатель на полученое сообщение
* @param[in] opcode - код операции
* @param[in] dest - кому передаем
* @return Статус передачи:\n
*TL_ERROR - ошибка передачи\n
*TL_PROCESS_SEND - идет передача\n
*TL_PROCESS_END - передача завершена
*/
void tl_info_msg(tl_info_t* tl_setting, tl_message_t* in_msg, tl_opcode_t opcode)
{
	tl_setting->current_pckt++;
	tl_gen_header(&tl_setting->out_msg, in_msg->address_dest, in_msg->address_src, opcode, tl_setting->current_pckt);
	write (tl_setting->socket, (char*)&tl_setting->out_msg, TL_HEAD_SIZE);
}


/**
* @brief Формирование запроса на подключение
* @param[in] tl_setting - указатель на структуру с настройками
* @param[in] dest - кому передаем
*/
void tl_send_request(tl_info_t* tl_setting, uint16_t dest)
{
	tl_setting->current_pckt = 1;
	tl_setting->data_info.result_offset = 0;
	// Запускаем таймер приемника
	software_timer_start(tl_setting->timer.repeat_timer, tl_setting->timer.timeout);
	// Отправляем запрос на подключение
	tl_gen_header(&tl_setting->out_msg, dest, tl_setting->device_id, TL_CONNECTION_REQUEST, tl_setting->current_pckt);
	write(tl_setting->socket, (char*)&tl_setting->out_msg, TL_HEAD_SIZE);
}

/**
* @brief Установка файла для передачи
* @param[in] tl_setting - указатель на структуру с настройками
* @param[in] out_data - указатель на данные, которые надо передать
* @param[in] data_size - размер данных для передачи
*/
void tl_set_tx_file(tl_info_t* tl_setting, uint8_t* out_data, size_t data_size)
{
	tl_setting->data_info.data_file = out_data;
	tl_setting->data_info.file_info.full_len = data_size;
	tl_setting->data_info.file_volume.request_size = data_size;
	tl_setting->data_info.preparation = 1;
	tl_setting->data_info.result_offset = 0;
}

/**
* @brief Установка буфера, в который будет сохраняться передаваемый файл
* @param[in] tl_setting - указатель на структуру с настройками
* @param[in] buffer_data - указатель на буфер
*/
void tl_set_rx_file(tl_info_t* tl_setting, uint8_t* buffer_data)
{
	tl_setting->data_info.data_file = buffer_data;
}

/**
* @brief Отправка куска данных
* @param[in] tl_setting - указатель на структуру с настройками
* @param[in] in_msg - указатель на полученное сообщение
*/
tl_opcode_t tl_write_data(tl_info_t* tl_setting, tl_message_t* in_msg)
{
	tl_opcode_t answer_opcode;

	switch (dt_write_data(&tl_setting->out_msg.dt_data, &tl_setting->data_info)) {
	case PROCESS_SEND:
		answer_opcode = TL_SEND_DATA;
		break;
	case PROCESS_ERROR:
		return TL_ERROR_ACK;
		break;
	case PROCESS_END:
		answer_opcode = TL_SEND_STOP;
		break;
	default:
		break;
	}
	tl_setting->current_pckt++;
	size_t file_size = TL_HEAD_SIZE + P_POS_CRC_END + tl_setting->out_msg.dt_data.len;
	tl_gen_header(&tl_setting->out_msg, in_msg->address_dest, in_msg->address_src, answer_opcode, tl_setting->current_pckt);
	write (tl_setting->socket, (char*)&tl_setting->out_msg, file_size);

	return answer_opcode;
}


/**
* @brief Повторная отправка сообщения
* @param[in] tl_setting - указатель на структуру с настройками
*/
void tl_repeat_pckt(tl_info_t* tl_setting)
{
	size_t file_size = 0;
	if(tl_setting->out_msg.opcode==TL_SEND_DATA || tl_setting->out_msg.opcode==TL_SEND_STOP)
		file_size = TL_HEAD_SIZE + P_POS_CRC_END + tl_setting->out_msg.dt_data.len;
	else
		file_size = TL_HEAD_SIZE;

	write (tl_setting->socket, (char*)&tl_setting->out_msg, file_size);
}

/**
* @brief Обработка принятного сообщения
* @param[in] in_msg - указатель на полученное сообщение
*/
parcer_status tl_process_rx_pckt(tl_message_t* in_msg)
{
	uint8_t tl_crc = (uint8_t)crc8(&in_msg->prefix, TL_HEAD_SIZE-1);
	if(tl_crc == in_msg->tl_crc)
		return PRCR_STATUS_ANSWER;

	return PRCR_STATUS_IGNORE;
}

/**
* @brief Таск проверки таймера на повторную отправку последнего сообщения
* @param[in] tl_setting - указатель на структуру с настройками
*/
void tl_task(tl_info_t* tl_setting)
{
	if(software_timer(tl_setting->timer.repeat_timer))
		tl_repeat_pckt(tl_setting);

}

