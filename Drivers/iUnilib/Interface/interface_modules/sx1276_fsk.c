//
// Created by User on 10.03.2022.
//

#include "sx1276_fsk.h"
#include <string.h>

static const uint8_t bandwidth_conf[21][2] =
        {
                {0x02, 0x07},
                {0x01, 0x07},
                {0x00, 0x07},
                {0x02, 0x06},
                {0x01, 0x06},
                {0x00, 0x06},
                {0x02, 0x05},
                {0x01, 0x05},
                {0x00, 0x05},
                {0x02, 0x04},
                {0x01, 0x04},
                {0x00, 0x04},
                {0x02, 0x03},
                {0x01, 0x03},
                {0x00, 0x03},
                {0x02, 0x02},
                {0x01, 0x02},
                {0x00, 0x02},
                {0x02, 0x01},
                {0x01, 0x01},
                {0x00, 0x01}
        };

/* ======================================================================	*/
/* Функция обмена байтами по SPI    										*/
/* ======================================================================	*/
static uint8_t spiXmit (SPI_TypeDef *SPIx, uint8_t byte)
{
    SPIx->DR = byte;
    while (!(SPIx->SR & SPI_SR_RXNE));
    return SPIx->DR;
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция считывания регистра												*/
/* ======================================================================	*/
static uint8_t sx1276ReadReg (sx1276_SPIControl_t *SPIx, uint8_t addr)
{
    uint8_t tmp;

    SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
    spiXmit (SPIx->SPIx, addr);
    tmp = spiXmit (SPIx->SPIx, DUMMY_BYTE);
    SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
    return tmp;
}
/* ======================================================================	*/


/* ======================================================================	*/
/* Функция записи регистра													*/
/* ======================================================================	*/
static void sx1276WriteReg (sx1276_SPIControl_t *SPIx, uint8_t addr, uint8_t value)
{
    SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
    spiXmit (SPIx->SPIx, (addr | SPI_WRITE_SIGN));
    spiXmit (SPIx->SPIx, value);
    SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
}
/* ======================================================================	*/


/* ======================================================================	*/
/* Функция считывания нескольких регистров									*/
/* ======================================================================	*/
static void sx1276ReadRegs (sx1276_SPIControl_t *SPIx, uint8_t sAddr, uint8_t *buffer, uint8_t len)
{
    uint8_t l;

    SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
    spiXmit (SPIx->SPIx, sAddr);
    for (l = 0; l < len; l++)
        *(buffer + l) = spiXmit (SPIx->SPIx, DUMMY_BYTE);
    SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
}
/* ======================================================================	*/


/* ======================================================================	*/
/* Функция записи нескольких регистров										*/
/* ======================================================================	*/
static void sx1276WriteRegs (sx1276_SPIControl_t *SPIx, uint8_t sAddr, uint8_t *data, uint8_t len)
{
    uint8_t l;

    SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
    spiXmit (SPIx->SPIx, (sAddr | SPI_WRITE_SIGN));
    for (l = 0; l < len; l++)
        spiXmit (SPIx->SPIx, *(data + l));
    SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
}
/* ======================================================================	*/

void sx1276_fsk_init (sx1276_fsk_t *trc)
{
    // Инитим SPI
    switch ((uint32_t)trc->Interface.SPIx)
    {
        case SPI1_BASE: __HAL_RCC_SPI1_CLK_ENABLE(); break;
        case SPI2_BASE: __HAL_RCC_SPI2_CLK_ENABLE(); break;
#ifdef SPI3_BASE
            case SPI3_BASE: __HAL_RCC_SPI3_CLK_ENABLE(); break;
#endif /* SPI3_BASE */
        default: break;
    }

    SPI_HandleTypeDef hspi;
    hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi.Init.Direction = SPI_DIRECTION_2LINES;
    hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi.Init.Mode = SPI_MODE_MASTER;
    hspi.Init.NSS = SPI_NSS_SOFT;
    hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi.Init.CRCPolynomial = 10;
    hspi.Instance = trc->Interface.SPIx;
    HAL_SPI_Init(&hspi);
    __HAL_SPI_ENABLE(&hspi);

    // Сбрасываем трансивер
    if (trc->Interface.RESET_PORT)
    {
        trc->Interface.RESET_PORT->ODR &=~ trc->Interface.Reset_Pin;
        delay_ms(250);
        trc->Interface.RESET_PORT->ODR |= trc->Interface.Reset_Pin;
    }

    // Отправляем трансивер гарантировано в сон, чтобы можно  было его перенастроить
    sx1276WriteReg (&trc->Interface, SX1276_FSK_REGOPMODE, 0x00);
    delay_ms(10);

    /* ======================================================================	*/
    /* Общие настройки										                    */
    /* ======================================================================	*/
    uint8_t regOpMode = SX1276_FSK_OPMODE;
    if (trc->settings.general.modulation == SX1276_FSK_OOK) regOpMode |= SX1276_FSK_OPMODE_OOK;
    if (trc->settings.general.frequency <= SX1276_FSK_BAND1_HIGH) regOpMode |= SX1276_FSK_OPMODE_LOWFREQ;
    sx1276WriteReg (&trc->Interface, SX1276_FSK_REGOPMODE, regOpMode);   // Задаем режим работы

    // Задаем битрейт
    // Значение регистров вычисляется по формуле:
    // BR = XOSC / (BRreg + BRRegFraq / 16)
    // XOSC - частота задающего кварца
    float br = (float)trc->Interface.oscFrequency / trc->settings.general.bitrate;
    uint16_t br_int = (int)br;
    br = (br - (float)br_int) * 100;
    uint8_t br_frac = (int)(br * 16 / 100);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_BITRATE_MSB, (br_int & 0xff00) >> 8);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_BITRATE_LSB, (br_int & 0xff));
    sx1276WriteReg(&trc->Interface, SX1276_FSK_BITRATE_FRAC, br_frac);

    // Задаем ширину девиации частоты
    // Fdev = Fstep * FdevReg
    // Fstep = XOSC / 2^19
    // FdevReg = Fdev * 2^19 / XOSC
    float Fdev_f = (float)trc->settings.general.deviation * TWOINNINETEEN / (float)trc->Interface.oscFrequency;
    uint16_t Fdev = (int)roundf(Fdev_f);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_DEVIATION_MSB, (Fdev & 0xff00) >> 8);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_DEVIATION_LSB, (Fdev & 0xff));

    // Задаем несущую частоту
    // Fcar = Fstep * FcarReg
    // Fstep = XOSC / 2^19
    // FcarReg = Fcar * 2^19 / XOSC
    float Fcar_f = (float)trc->settings.general.frequency * TWOINNINETEEN / (float)trc->Interface.oscFrequency;
    uint32_t Fcar = (int)roundf(Fcar_f);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_FCARRIER_MSB, (Fcar & 0xff0000) >> 16);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_FCARRIER_MID, (Fcar & 0xff00) >> 8);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_FCARRIER_LSB, (Fcar & 0xff));

    /* ======================================================================	*/
    /* Настройки выхода										                    */
    /* ======================================================================	*/
    uint8_t paConf;
    if (trc->settings.transmitter.output_pin == SX1276_FSK_OUT_PA_BOOST)
        paConf = SX1276_FSK_PACONF_PABOOST;
    else
        paConf = 0;

    paConf |= ((trc->settings.transmitter.maxPower_trim & SX1276_FSK_PACONF_MAXPOWER_MASK) << SX1276_FSK_PACONF_MAXPOWER_SHIFT);
    paConf |= (trc->settings.transmitter.outputPower_trim & SX1276_FSK_PACONF_OUTPOWER_MASK);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PACONF, paConf);

    // Установка формы модуляции
    uint8_t paRamp = SX1276_FSK_PARAMP_DEFAULT;
    paRamp |= (uint8_t)trc->settings.transmitter.output_shape << SX1276_FSK_PARAM_SHAPE_SHIFT;
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PARAMP, paRamp);

    /* ======================================================================	*/
    /* Настройки приемника  								                    */
    /* ======================================================================	*/
    uint8_t regRxConf = 0;

    // Тут пока все по дефолту
    regRxConf |= SX1276_FSK_RXCONF_AUTORESTART;             // В случае коллизии или насыщения (чего?) происходит авто рестарт режима приема
    regRxConf |= SX1276_FSK_RXCONF_AFC;                     // Включена автоподстройка частоты
    regRxConf |= SX1276_FSK_RXCONF_AGC;                     // Включено авторегулирование усиления
    regRxConf |= SX1276_FSK_RXCONF_TRIGGER_PREAMBLE;        // Триггер приема по детектированию преамбулы
    sx1276WriteReg(&trc->Interface, SX1276_FSK_RXCONF, regRxConf);

    // Критерий выбора ширины полосы является BitRate < 2 * RxBw
    uint8_t regBwRx = bandwidth_conf[trc->settings.general.bandwidth][0] << SX1276_FSK_BW_MANT_SHIFT;
    regBwRx |= bandwidth_conf[trc->settings.general.bandwidth][1];
    sx1276WriteReg(&trc->Interface, SX1276_FSK_BWRX, regBwRx);

    // Ширина полосы для AFC обычно делается пошире, чем полоса канала
    uint8_t regAfcBwRx = bandwidth_conf[trc->settings.general.afc_bandwidth][0] << SX1276_FSK_BW_MANT_SHIFT;
    regAfcBwRx |= bandwidth_conf[trc->settings.general.afc_bandwidth][1];
    sx1276WriteReg(&trc->Interface, SX1276_FSK_BWAFCRX, regAfcBwRx);

    // Включаем автосброс регистра AFC
    sx1276WriteReg(&trc->Interface, SX1276_FSK_AFC_CLEAR_REG, SX1276_FSK_AFC_CLEAR_REG_ENABLE);

    // Настройка детектора преамбулы. По дефолту - детектор включен, 3 байта нужно поймать, допускается 10 ошибок
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PREAMBLE_DETECTOR, SX1276_FSK_PREAMBLE_DETECTOR_DEFAULT);

    // Таймауты на всякие переходы и прерывания пока занулим (они в общемто и по дефолту нулевые)
    sx1276WriteReg(&trc->Interface, SX1276_FSK_RSSI_TIMEOUT, 0x00);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PREAMBLE_TIMEOUT, 0x00);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_SYNC_TIMEOUT, 0x00);

    // Выставим минимальную задержку между приемом пакета и рестартом приема
    sx1276WriteReg(&trc->Interface, SX1276_FSK_RESTART_DELAY, SX1276_FSK_RESTART_DELAY_VALUE);

    // Установим заданную длину преамбулы
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PREAMBLE_LEN_MSB, (trc->settings.packet.preambleLength & 0xff00) >> 8);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PREAMBLE_LEN_LSB, (trc->settings.packet.preambleLength & 0xff));

    // Задаем синхронизирующее слово
    uint8_t regSyncWord = SX1276_FSK_SYNC_WORD_DEFAULT;
    regSyncWord |= ((trc->settings.packet.syncWordSize - 1) & 0x07);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_SYNC_WORD, regSyncWord);
    sx1276WriteRegs(&trc->Interface, SX1276_FSK_SYNC_WORD_START, trc->settings.packet.syncWord, trc->settings.packet.syncWordSize);

    // Задаем настройки пакета
    uint8_t regPackConf1 = SX1276_FSK_PCKCONF1_VARIABLE_LENGTH;
    regPackConf1 |= trc->settings.packet.encode << SX1276_FSK_PCKCONF1_ENCODE_SHIFT;
    if (trc->settings.packet.crc) regPackConf1 |= SX1276_FSK_PCKCONF1_CRC_EN;
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PCKCONF1, regPackConf1);

    uint8_t regPackConf2 = SX1276_FSK_PCKCONF2_DEFAULT;
    if (trc->settings.general.mode == SX1276_FSK_PACKET_MODE) regPackConf2 |= SX1276_FSK_PCKCONF2_PACKET_MODE;
    sx1276WriteReg(&trc->Interface, SX1276_FSK_PCKCONF2, regPackConf2);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_MAXPAYLOAD, SX1276_FSK_MAXPAYLOAD_DEFAULT);

    // Задаем настройки отправки по фифо - если есть хоть один байт
    sx1276WriteReg(&trc->Interface, SX1276_FSK_FIFO_THRESHOLD, SX1276_FSK_FIFO_THRESHOLD_DEFAULT);

    // Переводим в режим ожидания
    delay_ms (10);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_REGOPMODE, (regOpMode | 0x01));
    delay_ms(100);

    trc->curState = SX1276_STATE_IDLE;
    trc->prevState = SX1276_STATE_IDLE;
}

static void sx1276_fsk_setMode (sx1276_fsk_t *trc, sx1276_fsk_mode_t mode)
{
    uint8_t temp;

    temp = sx1276ReadReg(&trc->Interface, SX1276_FSK_REGOPMODE);
    temp &=~ SX1276_FSK_OPMODE_MASK;
    temp |= (uint8_t)mode;
    sx1276WriteReg(&trc->Interface, SX1276_FSK_REGOPMODE, temp);
}

static int sx1276ModeReady (sx1276_fsk_t *trc)
{
    uint8_t temp;
    temp = sx1276ReadReg(&trc->Interface, SX1276_FSK_IRQ1_FLAGS);
    if (temp & SX1276_FSK_IRQ1_MODE_READY) return 1;
    else return 0;
}

static void restartRx (sx1276_fsk_t *trc)
{
    uint8_t tmp = sx1276ReadReg(&trc->Interface, SX1276_FSK_RXCONF);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_IRQ1_FLAGS, SX1276_FSK_IRQ1_PREAMBLE_DETECT);
    sx1276WriteReg(&trc->Interface, SX1276_FSK_RXCONF, tmp | 0x40);
}

sx1276_fsk_mode_t sx1276_fsk_getMode (sx1276_fsk_t *trc)
{
    uint8_t temp;
    temp = sx1276ReadReg(&trc->Interface, SX1276_FSK_REGOPMODE);
    temp &= SX1276_FSK_OPMODE_MASK;
    return (sx1276_fsk_mode_t)temp;
}

int sx1276_fsk_startReceive (sx1276_fsk_t *trc)
{
    if (trc->busy) return -1;
    sx1276_fsk_setMode(trc, SX1276_FSK_MODE_RECEIVE);

    trc->curState = SX1276_STATE_RX;
    return 1;
}

int sx1276_fsk_sendPacket (sx1276_fsk_t *trc, uint8_t* buffer, size_t length)
{
    // Переводим трансивер в режим синтезатора частоты для передачи
    sx1276_fsk_setMode (trc, SX1276_FSK_MODE_STANDBY);
    while (sx1276ModeReady(trc) == 0);

    // Заталкиваем сообщение в FIFO
    trc->Interface.CS_PORT->ODR &=~ trc->Interface.CS_Pin;
    spiXmit(trc->Interface.SPIx, 0x80);
    spiXmit(trc->Interface.SPIx, length);
    for (int l = 0; l < length; l++)
        spiXmit(trc->Interface.SPIx, buffer[l]);
    trc->Interface.CS_PORT->ODR |= trc->Interface.CS_Pin;

    sx1276_fsk_setMode (trc, SX1276_FSK_MODE_TRANSMIT);

    trc->prevState = trc->curState;
    trc->curState = SX1276_STATE_TX;
    trc->busy = 1;

    return 1;
}

static void readRssi (sx1276_fsk_t *trc)
{
    uint8_t tmpRssi;

    tmpRssi = sx1276ReadReg(&trc->Interface, SX1276_FSK_REG_RSSI);

    trc->rssi = (int)tmpRssi / -2;
}

static void sx1276_getReceivedPacket (sx1276_fsk_t *trc)
{
    uint8_t *p = trc->buffer;
    uint8_t len;

    trc->Interface.CS_PORT->ODR &=~ trc->Interface.CS_Pin;
    spiXmit(trc->Interface.SPIx, 0x00);
    len = spiXmit(trc->Interface.SPIx, 0x00);
    trc->packetLength = len;
    for (; len; len--)
    {
        *p = spiXmit(trc->Interface.SPIx, 0x00);
        p++;
    }
    trc->Interface.CS_PORT->ODR |= trc->Interface.CS_Pin;
}

void sx1276_fsk_poll(sx1276_fsk_t *trc)
{
    // Считываем флаги прерываний
    uint8_t temp1, temp2;

    temp1 = sx1276ReadReg(&trc->Interface, SX1276_FSK_IRQ1_FLAGS);
    temp2 = sx1276ReadReg(&trc->Interface, SX1276_FSK_IRQ2_FLAGS);
    if (trc->curState == SX1276_STATE_TX)
    {
        if (temp2 & SX1276_FSK_IRQ2_PACKET_SEND)
        {
            // Передача закончена - переводим трансивер обратно или в прием или в ожидание
            if (trc->prevState == SX1276_STATE_RX)
            {
                sx1276_fsk_setMode(trc, SX1276_FSK_MODE_RECEIVE);
                trc->curState = SX1276_STATE_RX;
            }
            else
            {
                sx1276_fsk_setMode(trc, SX1276_FSK_MODE_STANDBY);
                trc->curState = SX1276_STATE_IDLE;
            }
            trc->busy = 0;
        }
    }
    else if (trc->curState == SX1276_STATE_RX)
    {
        if (temp1 & SX1276_FSK_IRQ1_SYNC_MATCH)
        {
            if (trc->rssiReady == 0)
            {
                readRssi(trc);
                trc->rssiReady = 1;
            }
        }
        if (temp2 & SX1276_FSK_IRQ2_PAYLOAD_READY)
        {
            sx1276_getReceivedPacket (trc);
            trc->rssiReady = 0;
            trc->readyRead  = 1;
            restartRx(trc);
        }
    }
}
