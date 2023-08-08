#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global_macro.h"
#include "fifo.h"

#include "interface_conf.h"

#include "interface_modules/termios.h"
#include "interface_modules/uart_it.h"
#include "interface_modules/uart_device.h"
#include "interface_modules/spi_it.h"
#include "interface_modules/spi_device.h"
#include "interface_modules/i2c_ll.h"
#include "interface_modules/i2c_device.h"
#include "interface_modules/sx1276_fsk.h"
#include "interface_modules/sx1276_fsk_device.h"
#include "interface_modules/sx1276_lora.h"
#include "interface_modules/sx1276_lora_device.h"

typedef enum
{
    IF_TYPES_NONE,
    IF_TYPES_UART,
    IF_TYPES_SPI,
    IF_TYPES_I2C,
    IF_TYPES_SX1276_FSK,
    IF_TYPES_SX1276_LORA
} interface_types_t;

typedef union
{
    uart_t *uart;
    spi_t *spi;
    i2c_t *i2c;
    sx1276fsk_t *sx1276_fsk;
    sx1276lora_t *sx1276_lora;
} interface_handler_t;

typedef struct
{
    interface_handler_t handler;
    interface_types_t type;
    void* next;
    int desc;
    struct
    {
        ssize_t (*read)(char* buf, size_t length);
        ssize_t (*write)(char* buf, size_t length);
        void (*poll)(void);
    } syscalls;
    struct termios options;
} interface_t;

void interface_init (void);
int open (char* ifname, int flags);
void close (int desc);
ssize_t read (int desc, char* buffer, size_t size);
ssize_t write (int desc, char* buffer, size_t size);
int poll (int desc);

int tcgetattr (int fd, struct termios *opt);
int tcsetattr (int fd, int optional, struct termios *opt);
int tcsetiospeed (struct termios *tc, speed_t baud);

#endif /* _INTERFACE_H_ */
