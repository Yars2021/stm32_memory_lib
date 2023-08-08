//
// Created by dizar on 28.03.2022.
//

#ifndef ROVER_STM_SX1276_FSK_DEVICE_H
#define ROVER_STM_SX1276_FSK_DEVICE_H

#define SX1276_FSK_DEVICE_MAX_PACKET_SIZE           63

typedef struct
{
    sx1276_fsk_t transc;
    fifo_t rxBuffer;
    fifo_t txBuffer;
} sx1276fsk_t;

extern sx1276fsk_t sx1276_fsk1;
extern sx1276fsk_t sx1276_fsk2;
extern sx1276fsk_t sx1276_fsk3;

ssize_t sx1276_fsk1_write (char *buffer, size_t len);
ssize_t sx1276_fsk1_read (char *buffer, size_t len);
void sx1276_fsk1_poll (void);
ssize_t sx1276_fsk2_write (char *buffer, size_t len);
ssize_t sx1276_fsk2_read (char *buffer, size_t len);
void sx1276_fsk2_poll (void);
ssize_t sx1276_fsk3_write (char *buffer, size_t len);
ssize_t sx1276_fsk3_read (char *buffer, size_t len);
void sx1276_fsk3_poll (void);

void sx1276_fsk_device_init(sx1276fsk_t *trc, struct termios *opt);


#endif //ROVER_STM_SX1276_FSK_DEVICE_H
