//
// Created by dizar on 30.03.2022.
//

#ifndef RTK_BASE_TRANSMITTER_SX1276_LORA_DEVICE_H
#define RTK_BASE_TRANSMITTER_SX1276_LORA_DEVICE_H

#define SX1276_LORA_DEVICE_MAX_PACKET_SIZE          255

typedef struct
{
    sx1276_lora_t transc;
    fifo_t rxBuffer;
    fifo_t txBuffer;
} sx1276lora_t;

extern sx1276lora_t sx1276_lora1;
extern sx1276lora_t sx1276_lora2;
extern sx1276lora_t sx1276_lora3;

ssize_t sx1276_lora1_write (char *buffer, size_t len);
ssize_t sx1276_lora1_read (char *buffer, size_t len);
void sx1276_lora1_poll (void);
ssize_t sx1276_lora2_write (char *buffer, size_t len);
ssize_t sx1276_lora2_read (char *buffer, size_t len);
void sx1276_lora2_poll (void);
ssize_t sx1276_lora3_write (char *buffer, size_t len);
ssize_t sx1276_lora3_read (char *buffer, size_t len);
void sx1276_lora3_poll (void);

void sx1276_lora_device_init(sx1276lora_t *trc, struct termios *opt);

#endif //RTK_BASE_TRANSMITTER_SX1276_LORA_DEVICE_H
