//
// Created by dizar on 30.03.2022.
//

#include "../interface.h"

#define SX1276_LORA_ASSIGN(N, TX_BUFFER_SIZE, RX_BUFFER_SIZE, SPIX, CS_PORT, CS_PIN, RESET_PORT, RESET_PIN, OSC_FREQ) \
        static volatile uint8_t sx1276_lora##N##_buffer_rx[RX_BUFFER_SIZE];      \
        static volatile uint8_t sx1276_lora##N##_buffer_tx[TX_BUFFER_SIZE];      \
                                                                                \
        sx1276lora_t sx1276_lora##N =                                           \
        {                                                                       \
            .transc =                                                           \
            {                                                                   \
                .Control =                                                      \
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
                &sx1276_lora##N##_buffer_rx[0],                                 \
                RX_BUFFER_SIZE,                                                 \
                0,                                                              \
                0                                                               \
            },                                                                  \
            .txBuffer =                                                         \
            {                                                                   \
                &sx1276_lora##N##_buffer_tx[0],                                 \
                TX_BUFFER_SIZE,                                                 \
                0,                                                              \
                0                                                               \
            }                                                                   \
        }

#if defined (SX1276_LORA1_TX_BUFFER_SIZE) || defined (SX1276_LORA1_RX_BUFFER_SIZE)
#ifndef SX1276_LORA1_SPI
        #error Please set the define SX1276_LORA1_SPI with SPIx use to "talk" with transceiver
    #endif /* SX1276_LORA1_SPI */
    #ifndef SX1276_LORA1_CSPORT
        #error Please set the define SX1276_LORA1_CS_PORT with GPIO port where is CS Pin
    #endif /* SX1276_LORA1_CSPORT */
    #ifndef SX1276_LORA1_CSPIN
        #error Please set the define SX1276_LORA1_CS_PIN with CS Pin number
    #endif /* SX1276_LORA1_CSPIN */
    #ifndef SX1276_LORA1_RESETPORT
        #define SX1276_LORA1_RESETPORT 0
    #endif /* SX1276_LORA1_RESETPORT */
    #ifndef SX1276_LORA1_RESETPIN
        #define SX1276_LORA1_RESETPIN 0
    #endif /* SX1276_LORA1_RESETPIN */
    #ifndef SX1276_LORA1_OSC_FREQ
        #define SX1276_LORA1_OSC_FREQ 32000000
    #endif /* SX1276_LORA1_OSC_FREQ */

    SX1276_LORA_ASSIGN(1, SX1276_LORA1_TX_BUFFER_SIZE, SX1276_LORA1_RX_BUFFER_SIZE, SX1276_LORA1_SPI, SX1276_LORA1_CSPORT, SX1276_LORA1_CSPIN, SX1276_LORA1_RESETPORT, SX1276_LORA1_RESETPIN, SX1276_LORA1_OSC_FREQ);
#endif

#if defined (SX1276_LORA2_TX_BUFFER_SIZE) || defined (SX1276_LORA2_RX_BUFFER_SIZE)
#ifndef SX1276_LORA2_SPI
        #error Please set the define SX1276_LORA2_SPI with SPIx use to "talk" with transceiver
    #endif /* SX1276_LORA2_SPI */
    #ifndef SX1276_LORA2_CSPORT
        #error Please set the define SX1276_LORA2_CS_PORT with GPIO port where is CS Pin
    #endif /* SX1276_LORA2_CSPORT */
    #ifndef SX1276_LORA2_CSPIN
        #error Please set the define SX1276_LORA2_CS_PIN with CS Pin number
    #endif /* SX1276_LORA2_CSPIN */
    #ifndef SX1276_LORA2_RESETPORT
        #define SX1276_LORA2_RESETPORT 0
    #endif /* SX1276_LORA2_RESETPORT */
    #ifndef SX1276_LORA2_RESETPIN
        #define SX1276_LORA2_RESETPIN 0
    #endif /* SX1276_LORA2_RESETPIN */
    #ifndef SX1276_LORA2_OSC_FREQ
        #define SX1276_LORA2_OSC_FREQ 32000000
    #endif /* SX1276_LORA2_OSC_FREQ */

    SX1276_LORA_ASSIGN(2, SX1276_LORA2_TX_BUFFER_SIZE, SX1276_LORA2_RX_BUFFER_SIZE, SX1276_LORA2_SPI, SX1276_LORA2_CSPORT, SX1276_LORA2_CSPIN, SX1276_LORA2_RESETPORT, SX1276_LORA2_RESETPIN, SX1276_LORA2_OSC_FREQ);
#endif


#if defined (SX1276_LORA3_TX_BUFFER_SIZE) || defined (SX1276_LORA3_RX_BUFFER_SIZE)
#ifndef SX1276_LORA3_SPI
        #error Please set the define SX1276_LORA3_SPI with SPIx use to "talk" with transceiver
    #endif /* SX1276_LORA3_SPI */
    #ifndef SX1276_LORA3_CSPORT
        #error Please set the define SX1276_LORA3_CS_PORT with GPIO port where is CS Pin
    #endif /* SX1276_LORA3_CSPORT */
    #ifndef SX1276_LORA3_CSPIN
        #error Please set the define SX1276_LORA3_CS_PIN with CS Pin number
    #endif /* SX1276_LORA3_CSPIN */
    #ifndef SX1276_LORA3_RESETPORT
        #define SX1276_LORA3_RESETPORT 0
    #endif /* SX1276_LORA3_RESETPORT */
    #ifndef SX1276_LORA3_RESETPIN
        #define SX1276_LORA3_RESETPIN 0
    #endif /* SX1276_LORA3_RESETPIN */
    #ifndef SX1276_LORA3_OSC_FREQ
        #define SX1276_LORA3_OSC_FREQ 32000000
    #endif /* SX1276_LORA3_OSC_FREQ */

    SX1276_LORA_ASSIGN(2, SX1276_LORA3_TX_BUFFER_SIZE, SX1276_LORA3_RX_BUFFER_SIZE, SX1276_LORA3_SPI, SX1276_LORA3_CSPORT, SX1276_LORA3_CSPIN, SX1276_LORA3_RESETPORT, SX1276_LORA3_RESETPIN, SX1276_LORA3_OSC_FREQ);
#endif

void sx1276_lora_device_init(sx1276lora_t *trc, struct termios *opt)
{
    sx1276_LoRa_InitTypeDef sLora;

    sLora.frequency = opt->c_cc[V_LORA_FREQUENCY];
    sLora.band = opt->c_cc[V_LORA_FREQUENCY] >= SX1276_HIGH_BAND_FREQUENCY ? SX1276_LORA_BAND_HIGH : SX1276_LORA_BAND_LOW;
    sLora.bandWidth = opt->c_cc[V_LORA_BANDWIDTH];
    sLora.crcPayload = opt->c_cc[V_LORA_CRC];
    sLora.errorCodeRate = opt->c_cc[V_LORA_ERROR_CODE_RATE];
    sLora.headerType = opt->c_cc[V_LORA_HEADER_TYPE];
    sLora.maxPower = opt->c_cc[V_LORA_MAXPOWER_TRIM];
    sLora.outputPower = opt->c_cc[V_LORA_OUTPOWER_TRIM];
    sLora.oscType = SX1276_LORA_OSC_TCXO;
    sLora.outputPin = SX1276_LORA_OUT_RFO;
    sLora.preambleLength = opt->c_cc[V_LORA_PREAMBLE_LENGTH];
    sLora.spreadingFactor = opt->c_cc[V_LORA_SPREADING_FACTOR];
    sLora.oscillatorFrequency = trc->transc.Control.oscFrequency;

    sx1276_InitLoRaMode(&trc->transc, &sLora);

    sx1276_LoRa_startRx(&trc->transc);
}

static ssize_t sx1276_lora_write (sx1276lora_t *trc, char *buffer, size_t len)
{
    fifo_put_block(&trc->txBuffer, buffer, len);
    return (ssize_t)len;
}

static ssize_t sx1276_lora_read (sx1276lora_t *trc, char *buffer, size_t len)
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

static void sx1276_lora_poll (sx1276lora_t *trc)
{
    uint8_t txBuffer[SX1276_LORA_DEVICE_MAX_PACKET_SIZE];

    // Так то тут мы дергаем низкоуровневую функцию опроса состояния трансивера и проводим манипуляции по перемещению
    // байтов на отправку из фифо на передачу в транисвер и принятых байт из трансивера в фифо приема
    sx1276_LoRa_updateState(&trc->transc);

    // Если трансивер не в состоянии busy и у нас есть данные на отправку - отправляем их
    uint32_t qty = fifo_get_qty(&trc->txBuffer);
    if ((!trc->transc.busy) && (qty))
    {
        if (qty > SX1276_LORA_DEVICE_MAX_PACKET_SIZE)
            qty = SX1276_LORA_DEVICE_MAX_PACKET_SIZE;

        for (int k = 0; k < qty; k++)
            fifo_get_byte(&trc->txBuffer, (txBuffer + k));

        sx1276_LoRa_sendPacket(&trc->transc, txBuffer, qty);
    }

    // Если есть данные о приеме - то запихиваем их в фифо и всего делов
    if (trc->transc.receiver.readyRead)
    {
        fifo_put_block(&trc->rxBuffer, trc->transc.receiver.rxBuffer, trc->transc.receiver.bytesReceived);
        trc->transc.receiver.readyRead = 0;
    }
}

ssize_t sx1276_lora1_write (char *buffer, size_t len)
{
    sx1276_lora_write(&sx1276_lora1, buffer, len);
}

ssize_t sx1276_lora1_read (char *buffer, size_t len)
{
    sx1276_lora_read(&sx1276_lora1, buffer, len);
}

void sx1276_lora1_poll (void)
{
    sx1276_lora_poll (&sx1276_lora1);
}

ssize_t sx1276_lora2_write (char *buffer, size_t len)
{
    sx1276_lora_write(&sx1276_lora2, buffer, len);
}

ssize_t sx1276_lora2_read (char *buffer, size_t len)
{
    sx1276_lora_read(&sx1276_lora2, buffer, len);
}

void sx1276_lora2_poll (void)
{
    sx1276_lora_poll (&sx1276_lora2);
}

ssize_t sx1276_lora3_write (char *buffer, size_t len)
{
    sx1276_lora_write(&sx1276_lora3, buffer, len);
}

ssize_t sx1276_lora3_read (char *buffer, size_t len)
{
    sx1276_lora_read(&sx1276_lora3, buffer, len);
}

void sx1276_lora3_poll (void)
{
    sx1276_lora_poll (&sx1276_lora3);
}
