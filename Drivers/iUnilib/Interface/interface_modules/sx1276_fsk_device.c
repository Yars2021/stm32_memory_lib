//
// Created by dizar on 28.03.2022.
//

#include "../interface.h"

#define SX1276_FSK_ASSIGN(N, TX_BUFFER_SIZE, RX_BUFFER_SIZE, SPIX, CS_PORT, CS_PIN, RESET_PORT, RESET_PIN, OSC_FREQ) \
        static volatile uint8_t sx1276_fsk##N##_buffer_rx[RX_BUFFER_SIZE];      \
        static volatile uint8_t sx1276_fsk##N##_buffer_tx[TX_BUFFER_SIZE];      \
                                                                                \
        sx1276fsk_t sx1276_fsk##N =                                             \
        {                                                                       \
            .transc =                                                           \
            {                                                                   \
                .Interface =                                                    \
                {                                                               \
                    SPIX,                                                       \
                    CS_PORT,                                                    \
                    CS_PIN,                                                     \
                    RESET_PORT,                                                 \
                    RESET_PIN,                                                  \
                    OSC_FREQ                                                    \
                }                                                               \
            },                                                                  \
            .rxBuffer =                                                         \
            {                                                                   \
                &sx1276_fsk##N##_buffer_rx[0],                                  \
                RX_BUFFER_SIZE,                                                 \
                0,                                                              \
                0                                                               \
            },                                                                  \
            .txBuffer =                                                         \
            {                                                                   \
                &sx1276_fsk##N##_buffer_tx[0],                                  \
                TX_BUFFER_SIZE,                                                 \
                0,                                                              \
                0                                                               \
            }                                                                   \
        }

#if defined (SX1276_FSK1_TX_BUFFER_SIZE) || defined (SX1276_FSK1_RX_BUFFER_SIZE)
    #ifndef SX1276_FSK1_SPI
        #error Please set the define SX1276_FSK1_SPI with SPIx use to "talk" with transceiver
    #endif /* SX1276_FSK1_SPI */
    #ifndef SX1276_FSK1_CSPORT
        #error Please set the define SX1276_FSK1_CS_PORT with GPIO port where is CS Pin
    #endif /* SX1276_FSK1_CSPORT */
    #ifndef SX1276_FSK1_CSPIN
        #error Please set the define SX1276_FSK1_CS_PIN with CS Pin number
    #endif /* SX1276_FSK1_CSPIN */
    #ifndef SX1276_FSK1_RESETPORT
        #define SX1276_FSK1_RESETPORT 0
    #endif /* SX1276_FSK1_RESETPORT */
    #ifndef SX1276_FSK1_RESETPIN
        #define SX1276_FSK1_RESETPIN 0
    #endif /* SX1276_FSK1_RESETPIN */
    #ifndef SX1276_FSK1_OSC_FREQ
        #define SX1276_FSK1_OSC_FREQ 32000000
    #endif /* SX1276_FSK1_OSC_FREQ */

    SX1276_FSK_ASSIGN(1, SX1276_FSK1_TX_BUFFER_SIZE, SX1276_FSK1_RX_BUFFER_SIZE, SX1276_FSK1_SPI, SX1276_FSK1_CSPORT, SX1276_FSK1_CSPIN, SX1276_FSK1_RESETPORT, SX1276_FSK1_RESETPIN, SX1276_FSK1_OSC_FREQ);
#endif

#if defined (SX1276_FSK2_TX_BUFFER_SIZE) || defined (SX1276_FSK2_RX_BUFFER_SIZE)
    #ifndef SX1276_FSK2_SPI
        #error Please set the define SX1276_FSK2_SPI with SPIx use to "talk" with transceiver
    #endif /* SX1276_FSK2_SPI */
    #ifndef SX1276_FSK2_CSPORT
        #error Please set the define SX1276_FSK2_CS_PORT with GPIO port where is CS Pin
    #endif /* SX1276_FSK2_CSPORT */
    #ifndef SX1276_FSK2_CSPIN
        #error Please set the define SX1276_FSK2_CS_PIN with CS Pin number
    #endif /* SX1276_FSK2_CSPIN */
    #ifndef SX1276_FSK2_RESETPORT
        #define SX1276_FSK2_RESETPORT 0
    #endif /* SX1276_FSK2_RESETPORT */
    #ifndef SX1276_FSK2_RESETPIN
        #define SX1276_FSK2_RESETPIN 0
    #endif /* SX1276_FSK2_RESETPIN */
    #ifndef SX1276_FSK2_OSC_FREQ
        #define SX1276_FSK2_OSC_FREQ 32000000
    #endif /* SX1276_FSK2_OSC_FREQ */

    SX1276_FSK_ASSIGN(2, SX1276_FSK2_TX_BUFFER_SIZE, SX1276_FSK2_RX_BUFFER_SIZE, SX1276_FSK2_SPI, SX1276_FSK2_CSPORT, SX1276_FSK2_CSPIN, SX1276_FSK2_RESETPORT, SX1276_FSK2_RESETPIN, SX1276_FSK2_OSC_FREQ);
#endif


#if defined (SX1276_FSK3_TX_BUFFER_SIZE) || defined (SX1276_FSK3_RX_BUFFER_SIZE)
#ifndef SX1276_FSK3_SPI
        #error Please set the define SX1276_FSK3_SPI with SPIx use to "talk" with transceiver
    #endif /* SX1276_FSK3_SPI */
    #ifndef SX1276_FSK3_CSPORT
        #error Please set the define SX1276_FSK3_CS_PORT with GPIO port where is CS Pin
    #endif /* SX1276_FSK3_CSPORT */
    #ifndef SX1276_FSK3_CSPIN
        #error Please set the define SX1276_FSK3_CS_PIN with CS Pin number
    #endif /* SX1276_FSK3_CSPIN */
    #ifndef SX1276_FSK3_RESETPORT
        #define SX1276_FSK3_RESETPORT 0
    #endif /* SX1276_FSK3_RESETPORT */
    #ifndef SX1276_FSK3_RESETPIN
        #define SX1276_FSK3_RESETPIN 0
    #endif /* SX1276_FSK3_RESETPIN */
    #ifndef SX1276_FSK3_OSC_FREQ
        #define SX1276_FSK3_OSC_FREQ 32000000
    #endif /* SX1276_FSK3_OSC_FREQ */

    SX1276_FSK_ASSIGN(2, SX1276_FSK3_TX_BUFFER_SIZE, SX1276_FSK3_RX_BUFFER_SIZE, SX1276_FSK3_SPI, SX1276_FSK3_CSPORT, SX1276_FSK3_CSPIN, SX1276_FSK3_RESETPORT, SX1276_FSK3_RESETPIN, SX1276_FSK3_OSC_FREQ);
#endif

void sx1276_fsk_device_init(sx1276fsk_t *trc, struct termios *opt)
{
    trc->transc.settings.general.frequency = opt->c_cc[V_FSK_FREQUENCY];
    trc->transc.settings.general.bitrate = opt->c_cc[V_FSK_BITRATE];
    trc->transc.settings.general.deviation = opt->c_cc[V_FSK_DEVIATION];
    trc->transc.settings.general.modulation = SX1276_FSK_FSK;
    trc->transc.settings.general.mode = SX1276_FSK_PACKET_MODE;
    trc->transc.settings.general.bandwidth = opt->c_cc[V_FSK_BANDWIDTH];
    trc->transc.settings.general.afc_bandwidth = opt->c_cc[V_FSK_AFCBANDWIDTH];
    trc->transc.settings.transmitter.output_pin = SX1276_FSK_OUT_RFO;
    trc->transc.settings.transmitter.maxPower_trim = opt->c_cc[V_FSK_MAXPOWER_TRIM];
    trc->transc.settings.transmitter.outputPower_trim = opt->c_cc[V_FSK_OUTPOWER_TRIM];
    trc->transc.settings.transmitter.output_shape = opt->c_cc[V_FSK_OUTPUTSHAPE];
    trc->transc.settings.packet.preambleLength = opt->c_cc[V_FSK_PREAMBLE_LENGTH];
    trc->transc.settings.packet.syncWordSize = opt->c_cc[V_FSK_SYNCWORD_SIZE];
    memcpy(trc->transc.settings.packet.syncWord, &opt->c_cc[V_FSK_SYNCWORD_A], opt->c_cc[V_FSK_SYNCWORD_SIZE]);
    trc->transc.settings.packet.encode = opt->c_cc[V_FSK_ENCODE];
    trc->transc.settings.packet.crc = SX1276_FSK_PACKET_CRC_ENABLE;
    sx1276_fsk_init(&trc->transc);

    sx1276_fsk_startReceive (&trc->transc);
}

static ssize_t sx1276_fsk_write (sx1276fsk_t *trc, char *buffer, size_t len)
{
    fifo_put_block(&trc->txBuffer, buffer, len);
    return (ssize_t)len;
}

static ssize_t sx1276_fsk_read (sx1276fsk_t *trc, char *buffer, size_t len)
{
    uint32_t count;
    char *pnt = buffer;

    count = fifo_get_qty (&trc->rxBuffer);
    if (count)
    {
        if (len > count) len = count;
        for (; len; len--)
        {
            fifo_get_byte(&trc->rxBuffer, (uint8_t*)pnt);
            pnt++;
        }
    }
    return (ssize_t)count;
}

static void sx1276fsk_poll (sx1276fsk_t *trc)
{
    uint8_t txBuffer[SX1276_FSK_DEVICE_MAX_PACKET_SIZE+1];

    // Так то тут мы дергаем низкоуровневую функцию опроса состояния трансивера и проводим манипуляции по перемещению
    // байтов на отправку из фифо на передачу в транисвер и принятых байт из трансивера в фифо приема
    sx1276_fsk_poll(&trc->transc);

    // Если трансивер не в состоянии busy и у нас есть данные на отправку - отправляем их
    uint32_t qty = fifo_get_qty(&trc->txBuffer);
    if ((!trc->transc.busy) && (qty))
    {
        if (qty > SX1276_FSK_DEVICE_MAX_PACKET_SIZE)
            qty = SX1276_FSK_DEVICE_MAX_PACKET_SIZE;

        for (int k = 0; k < qty; k++)
            fifo_get_byte(&trc->txBuffer, (txBuffer + k));

        sx1276_fsk_sendPacket(&trc->transc, txBuffer, qty);
    }

    // Если есть данные о приеме - то запихиваем их в фифо и всего делов
    if (trc->transc.readyRead)
    {
        fifo_put_block(&trc->rxBuffer, trc->transc.buffer, trc->transc.packetLength);
        trc->transc.readyRead = 0;
    }
}

ssize_t sx1276_fsk1_write (char *buffer, size_t len)
{
    sx1276_fsk_write(&sx1276_fsk1, buffer, len);
}

ssize_t sx1276_fsk1_read (char *buffer, size_t len)
{
    sx1276_fsk_read(&sx1276_fsk1, buffer, len);
}

void sx1276_fsk1_poll (void)
{
    sx1276fsk_poll (&sx1276_fsk1);
}

ssize_t sx1276_fsk2_write (char *buffer, size_t len)
{
    sx1276_fsk_write(&sx1276_fsk2, buffer, len);
}

ssize_t sx1276_fsk2_read (char *buffer, size_t len)
{
    sx1276_fsk_read(&sx1276_fsk2, buffer, len);
}

void sx1276_fsk2_poll (void)
{
    sx1276fsk_poll (&sx1276_fsk2);
}

ssize_t sx1276_fsk3_write (char *buffer, size_t len)
{
    sx1276_fsk_write(&sx1276_fsk3, buffer, len);
}

ssize_t sx1276_fsk3_read (char *buffer, size_t len)
{
    sx1276_fsk_read(&sx1276_fsk3, buffer, len);
}

void sx1276_fsk3_poll (void)
{
    sx1276fsk_poll (&sx1276_fsk3);
}
