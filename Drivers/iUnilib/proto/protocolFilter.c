//
// Created by Dizarker on 08.10.2021.
//

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "protocolFilter.h"

/*!
 * @brief Функция сброса автомата приемника
 * @param[in] protoF - указатель на экземпляр модуля фильтра протокола
 */
static void resetReceiver (protocolFilter_t *protoF)
{
    protoF->state = PROTOCOL_FILTER_STATE_WAIT_FOR_START_CONDITION;
    protoF->protoLock = -1;
    protoF->lenRx = 0;
    protoF->lenToRx = 0;
    protoF->r7 = 0;
}

/*!
 * @brief Функция инициализация экземпляра модуля фильтра протокола
 * @param[in,out] protoF - указатель на объект модуля фильтра протоколов
 * @param[in] protocolsNum - количество протоколов, которое будет фильтроваться
 * @return возвращает 1 в случае успеха
 */
int protocolFilter_init (protocolFilter_t *protoF, int protocolsNum)
{
    protoF->state = PROTOCOL_FILTER_STATE_WAIT_FOR_START_CONDITION;
    protoF->protocolsNum = protocolsNum;
    protoF->protoLock = -1;
    protoF->lenRx = 0;
    return 1;
}

/*!
 * @brief Функция обработки принятого байта
 * @details Каждый принятый байт с интерфейса, куда приходят данные, которые необходимо прогонять через фильтр,
 * надо обрабатывать с помощью данной функции
 * @param[in, out] protoF - указатель на объект модуля фильтра протокола
 * @param[in] byte - принятый байт
 * @return - возвращает -1 в случае нештатной ситуации
 */
int protocolFilter_task (protocolFilter_t *protoF, char byte)
{
    int i;
    int finish = 0;
    char *ptr;

    // Приняли байт - обработаем его
    protoF->buffer[protoF->lenRx] = byte;

    switch (protoF->state)
    {
        case PROTOCOL_FILTER_STATE_WAIT_FOR_START_CONDITION:
            for (i = 0; i < protoF->protocolsNum; i++)
            {
                if (memcmp (protoF->buffer, protoF->protocol[i].prefix, protoF->lenRx+1)== 0)
                {
                    // Чтото где-то совпало, двигаем указатель
                    protoF->lenRx++;

                    // Если у нас сошлась вся старт кондиция - то лочим приемник на данный тип протокола
                    if (protoF->protocol[i].prefixSize == protoF->lenRx)
                    {
                        protoF->protoLock = i;
                        if (protoF->protocol[i].type == PROTOCOL_FILTER_TYPE_BINARY)
                        {
                            if (protoF->protocol[i].lengthSize)
                            {
                                protoF->state = PROTOCOL_FILTER_STATE_RECEIVING_LENGTH;
                                protoF->lenToRx = protoF->protocol[i].lengthSize + protoF->protocol[i].lengthOffset;
                                protoF->r7 = 0;
                            }
                            else
                            {
                                protoF->state = PROTOCOL_FILTER_STATE_WAITING_FOR_POSTFIX;
                            }
                        }
                        else if (protoF->protocol[i].type == PROTOCOL_FILTER_TYPE_TEXT)
                        {
                            protoF->state = PROTOCOL_FILTER_STATE_WAITING_FOR_POSTFIX;
                        }
                    }
                    break;
                }
            }
            if (i >= protoF->protocolsNum)
            {
                // Ничего хорошего не нашли - сбрасываем состояние приемника
                resetReceiver (protoF);
            }
            break;

        case PROTOCOL_FILTER_STATE_RECEIVING_LENGTH:
            protoF->lenRx++;
            protoF->lenToRx--;

            // Сначала пропустим байты смещения поля длины
            if (protoF->lenToRx < protoF->protocol[protoF->protoLock].lengthSize)
            {
                // Собираем поле длины, в зависимости от установленного в протоколе порядка байт
                if (protoF->protocol[protoF->protoLock].byteorder == PROTOCOL_FILTER_BYTEORDER_BIG_ENDIAN)
                {
                    protoF->r7 <<= 8;
                    protoF->r7 |= byte;
                }
                else if (protoF->protocol[protoF->protoLock].byteorder == PROTOCOL_FILTER_BYTEORDER_LITTLE_ENDIAN)
                {
                    protoF->r7 >>= 8;
                    protoF->r7 |= byte << (protoF->protocol[protoF->protoLock].lengthSize - 1) * 8;
                }
            }

            // Если приняли все поле длины то идем далее
            if (protoF->lenToRx == 0)
            {
                // Приняли всю длину, начинаем прием тела сообщения
                protoF->lenToRx = protoF->r7 + protoF->protocol[protoF->protoLock].lengthExtra;
                protoF->state = PROTOCOL_FILTER_STATE_RECEIVING_MESSAGE_BODY;
            }
            break;

        case PROTOCOL_FILTER_STATE_RECEIVING_MESSAGE_BODY:
            protoF->lenRx++;
            protoF->lenToRx--;

            // Если протокол текстовый, то проверим на то, что принятый символ действительно символьный
            if (protoF->protocol[protoF->protocolsNum].type == PROTOCOL_FILTER_TYPE_TEXT)
            {
                if ((byte < PROTOCOL_FILTER_ASCII_SYMBOL_MIN_VALUE) || (byte > PROTOCOL_FILTER_ASCII_SYMBOL_MAX_VALUE))
                {
                    // Приняли не символ, а бинарные данные -- сбрасываем приемник
                    resetReceiver(protoF);
                }
            }

            if (protoF->lenToRx == 0)
            {
                // Приняли все тело сообщения. Если есть контрольная сумма - принимаем далее ее
                if (protoF->protocol[protoF->protoLock].crcSize)
                {
                    protoF->lenToRx = protoF->protocol[protoF->protoLock].crcSize;
                    protoF->state = PROTOCOL_FILTER_STATE_RECEIVING_CRC;
                }
                // Если нет, то может нам надо дождать стоп кондиции?
                else if (protoF->protocol[protoF->protoLock].postfixSize)
                {
                    protoF->lenToRx = protoF->protocol[protoF->protoLock].postfixSize;
                    protoF->state = PROTOCOL_FILTER_STATE_WAITING_FOR_POSTFIX;
                }
                else
                {
                    finish = 1;
                }
            }
            break;

        case PROTOCOL_FILTER_STATE_RECEIVING_CRC:
            protoF->lenRx++;
            protoF->lenToRx--;
            if (protoF->lenToRx == 0)
            {
                // Если нет, то может нам надо дождать стоп кондиции?
                if (protoF->protocol[protoF->protoLock].postfixSize)
                {
                    protoF->lenToRx = protoF->protocol[protoF->protoLock].postfixSize;
                    protoF->state = PROTOCOL_FILTER_STATE_WAITING_FOR_POSTFIX;
                }
                else
                {
                    // Нет конечной кондиции - значит мы приняли пакет
                    finish = 1;
                }
            }
            break;

        case PROTOCOL_FILTER_STATE_WAITING_FOR_POSTFIX:
            // Ждем просто завершающей кондиции
            protoF->lenRx++;
            ptr = &protoF->buffer[protoF->lenRx] - protoF->protocol[protoF->protoLock].postfixSize;
            if (memcmp(ptr, protoF->protocol[protoF->protoLock].postfix, protoF->protocol[protoF->protoLock].postfixSize) == 0)
            {
                // Есть завершающая кондиция. Надо вызвать соответствующий обработчик и сбросить статус приемника
                finish = 1;
            }
            break;

        default: break;
    }

    if (finish)
    {
        // Нет ни CRC, ни конечной кондиции - значит мы приняли пакет
        protoF->protocol[protoF->protoLock].protocolParser(protoF->buffer, protoF->lenRx);
        resetReceiver (protoF);
    }

    if (protoF->lenRx == sizeof (protoF->buffer))
    {
        // Если вдруг чтото пошло не так и приемный буфер переполнился - сбрасываем приемник
        resetReceiver(protoF);
    }
}
