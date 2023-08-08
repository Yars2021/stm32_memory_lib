#ifndef ROVER_STM_INTERFACE_COLLECTOR_H
#define ROVER_STM_INTERFACE_COLLECTOR_H

#include <stdint.h>
#include "../interface.h"

typedef struct
{
    uint8_t uart;
    uint8_t spi;
    uint8_t i2c;
    uint8_t sx1276_fsk;
    uint8_t sx1276_lora;
} interfaces_present_t;

typedef struct
{
    char ifname[32];
    interface_handler_t handler;
    interface_types_t type;
    ssize_t (*read)(char* buf, size_t length);
    ssize_t (*write)(char* buf, size_t length);
    void (*poll)(void);
} interface_instance_t;

void interfaces_collect (void);
interface_types_t interface_assign (char* ifname, interface_t *interface);

#endif //ROVER_STM_INTERFACE_COLLECTOR_H
