//
// Created by Dizarker on 28.01.2021.
//
#include <string.h>
#include "interface_collector.h"
#include "interface_conf.h"

static const interface_instance_t *interface_instances;
static int interface_count;

/*!
 * \brief Функция сбора данных о подключенных к проекту интерфейсах
 * \details В файде interface_conf.h указаны размеры приемных и передающих буферов
 * для каждого из интерфейсов. Если буфер заявлен - то считается, что этот интерфейс
 * будет использован в проекте и для него создается экземпляр хэндлера.
 */
void interfaces_collect (void)
{
    static const interface_instance_t iface_table[] = {
#if defined (UART1_TX_BUFFER_SIZE) || defined (UART1_RX_BUFFER_SIZE)
        { .ifname = "uart1", .handler.uart = &uart1, .type = IF_TYPES_UART, .read = uart1_read, .write = uart1_write },
#endif

#if defined (UART2_TX_BUFFER_SIZE) || defined (UART2_RX_BUFFER_SIZE)
        { .ifname = "uart2", .handler.uart = &uart2, .type = IF_TYPES_UART, .read = uart2_read, .write = uart2_write },
#endif

#if defined (UART3_TX_BUFFER_SIZE) || defined (UART3_RX_BUFFER_SIZE)
        { .ifname = "uart3", .handler.uart = &uart3, .type = IF_TYPES_UART, .read = uart3_read, .write = uart3_write },
#endif

#if defined (UART4_TX_BUFFER_SIZE) || defined (UART4_RX_BUFFER_SIZE)
        { .ifname = "uart4", .handler.uart = &uart4, .type = IF_TYPES_UART, .read = uart4_read, .write = uart4_write },
#endif

#if defined (UART5_TX_BUFFER_SIZE) || defined (UART5_RX_BUFFER_SIZE)
        { .ifname = "uart5", .handler.uart = &uart5, .type = IF_TYPES_UART, .read = uart5_read, .write = uart5_write },
#endif

#if defined (SPI1_TX_BUFFER_SIZE) || defined (SPI1_RX_BUFFER_SIZE)
        { .ifname = "spi1", .handler.spi = &spi1, .type = IF_TYPES_SPI, .read = spi1_read, .write = spi1_write },
#endif

#if defined (SPI2_TX_BUFFER_SIZE) || defined (SPI2_RX_BUFFER_SIZE)
        { .ifname = "spi2", .handler.spi = &spi2, .type = IF_TYPES_SPI, .read = spi2_read, .write = spi2_write },
#endif

#if defined (SPI3_TX_BUFFER_SIZE) || defined (SPI3_RX_BUFFER_SIZE)
        { .ifname = "spi3", .handler.spi = &spi3, .type = IF_TYPES_SPI, .read = spi3_read, .write = spi3_write },
#endif

#if defined (I2C1_USE)
        { .ifname = "i2c1", .handler.i2c = &i2c1, .type = IF_TYPES_I2C, .read = i2c1_read, .write = i2c1_write },
#endif

#if defined I2C2_USE
        { .ifname = "i2c2", .handler.i2c = &i2c2, .type = IF_TYPES_I2C, .read = i2c2_read, .write = i2c2_write }
#endif

#if defined (SX1276_FSK1_TX_BUFFER_SIZE) || defined (SX1276_FSK1_RX_BUFFER_SIZE)
            { .ifname = "sx1276_fsk1", .handler.sx1276_fsk = &sx1276_fsk1, .type = IF_TYPES_SX1276_FSK, .read = sx1276_fsk1_read, .write = sx1276_fsk1_write, .poll = sx1276_fsk1_poll },
#endif

#if defined (SX1276_FSK2_TX_BUFFER_SIZE) || defined (SX1276_FSK2_RX_BUFFER_SIZE)
            { .ifname = "sx1276_fsk2", .handler.sx1276_fsk = &sx1276_fsk2, .type = IF_TYPES_SX1276_FSK, .read = sx1276_fsk2_read, .write = sx1276_fsk2_write, .poll = sx1276_fsk2_poll },
#endif

#if defined (SX1276_FSK3_TX_BUFFER_SIZE) || defined (SX1276_FSK3_RX_BUFFER_SIZE)
            { .ifname = "sx1276_fsk3", .handler.sx1276_fsk = &sx1276_fsk3, .type = IF_TYPES_SX1276_FSK, .read = sx1276_fsk3_read, .write = sx1276_fsk3_write, .poll = sx1276_fsk3_poll },
#endif

#if defined (SX1276_LORA1_TX_BUFFER_SIZE) || defined (SX1276_LORA1_RX_BUFFER_SIZE)
            { .ifname = "sx1276_lora1", .handler.sx1276_lora = &sx1276_lora1, .type = IF_TYPES_SX1276_LORA, .read = sx1276_lora1_read, .write = sx1276_lora1_write, .poll = sx1276_lora1_poll },
#endif

#if defined (SX1276_LORA2_TX_BUFFER_SIZE) || defined (SX1276_LORA2_RX_BUFFER_SIZE)
            { .ifname = "sx1276_lora2", .handler.sx1276_lora = &sx1276_lora2, .type = IF_TYPES_SX1276_LORA, .read = sx1276_lora2_read, .write = sx1276_lora2_write, .poll = sx1276_lora2_poll },
#endif

#if defined (SX1276_LORA3_TX_BUFFER_SIZE) || defined (SX1276_LORA3_RX_BUFFER_SIZE)
            { .ifname = "sx1276_lora3", .handler.sx1276_lora = &sx1276_lora3, .type = IF_TYPES_SX1276_LORA, .read = sx1276_lora3_read, .write = sx1276_lora3_write, .poll = sx1276_lora3_poll },
#endif

    };

    interface_count = sizeof (iface_table) / sizeof (interface_instance_t);
    interface_instances = iface_table;
}

/*!
 * \brief Функция привязки хэндлера интерфейса по его имени
 * \details По стандарту POSIX каждый интерфейс это файл, который открывается по
 * пути к этому файлу, т.е. имя интерфейса - это строка. Данная функция совершает привязку
 * хэндлера интерфейса к структуре. А также возвращает тип интерфейса или ошибку, если
 * интерфейса с таким именем не нашлось
 * \return возвращает тип интерфейса, к которому была совершена привязка. IF_INTERFACE_NONE в случае ошибки
 * \param[in] ifname - строка с именем интерфейса
 * \param[in,out] - указатель на структуру экземпляра интерфейса. Туда будет записан адрес соответствующего хэндлера
 * в случае успеха
 */
interface_types_t interface_assign (char* ifname, interface_t *interface)
{
    interface_types_t  res = IF_TYPES_NONE;

    for (int i = 0; i < interface_count; i++)
    {
        if ((strcmp(ifname, interface_instances[i].ifname)) == 0)
        {
            interface->handler = interface_instances[i].handler;
            interface->type = interface_instances[i].type;
            interface->syscalls.read = interface_instances[i].read;
            interface->syscalls.write = interface_instances[i].write;
            interface->syscalls.poll = interface_instances[i].poll;
            res = interface->type;
            break;
        }
    }

    return res;
}
