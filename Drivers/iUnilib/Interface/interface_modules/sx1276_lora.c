#include "sx1276_lora.h"

static uint8_t sx1276ReadReg (sx1276_SPIControl_t *SPIx, uint8_t addr);
static void sx1276WriteReg (sx1276_SPIControl_t *SPIx, uint8_t addr, uint8_t value);
static void sx1276ReadRegs (sx1276_SPIControl_t *SPIx, uint8_t sAddr, uint8_t *buffer, uint8_t len);
static void sx1276WriteRegs (sx1276_SPIControl_t *SPIx, uint8_t sAddr, uint8_t *data, uint8_t len);
static uint8_t sx1276SpiXmit (SPI_TypeDef *SPIx, uint8_t byte);
uint8_t st1, st2;

/* ======================================================================	*/
/* Функция начальной инициализации трансивера в режим LoRa Modem					*/
/* ======================================================================	*/
void sx1276_InitLoRaMode (sx1276_lora_t *modem, sx1276_LoRa_InitTypeDef *config)
{
	float f_freq;
	uint32_t i_freq;
	
	// Вывалимся, если не задана частота осциллятора. Иначе Division by Zero
	if (!(config->frequency)) return;

    // Инитим SPI
    switch ((uint32_t)modem->Control.SPIx)
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
    hspi.Instance = modem->Control.SPIx;
    HAL_SPI_Init(&hspi);
    __HAL_SPI_ENABLE(&hspi);

	// Если есть аппаратный резет - то сбрасываем
	if ((config->RESET_PORT) && (config->Reset_Pin))
	{ 
		modem->Control.RESET_PORT = config->RESET_PORT;
		modem->Control.Reset_Pin = config->Reset_Pin;

		config->RESET_PORT->ODR &=~ config->Reset_Pin;
		delay_ms (250);
		config->RESET_PORT->ODR |= config->Reset_Pin;
	}
	else
	{
		modem->Control.RESET_PORT = 0;
		modem->Control.Reset_Pin = 0;
	}

	// Отправляем трансивер в сон, для того, чтобы он проснулся уже новым чело... трансивером в режиме LoRa Modem
	sx1276WriteReg (&modem->Control, LORA_REGOPMODE, 0x00);//LORA_REGOPMODE_SLEEP);
	sx1276WriteReg (&modem->Control, LORA_REGOPMODE, LORA_REGOPMODE_LORAMODE);
	
	// Считаем конфигурацию чипа в структуру регистров, подправим там все по своему желанию и отправим обратно
	sx1276ReadRegs (&modem->Control, LORA_REGOPMODE, (uint8_t*)&modem->confRegs + 1, sizeof (sx1276_lora_regs) - 1);		// Считываем на 1 меньше, т.к. регистр 0х00 - это ФИФО
	
	// Выставим требуемую частоту
	modem->Control.oscFrequency = config->oscillatorFrequency;
	f_freq = (float)config->frequency / modem->Control.oscFrequency;
	f_freq *= TWO_IN_NINETEEN;
	i_freq = (uint32_t)f_freq;
	modem->confRegs.Reg06_FrMsb = (i_freq & 0x00ff0000) >> 16;
	modem->confRegs.Reg07_FrMid = (i_freq & 0x0000ff00) >> 8;
	modem->confRegs.Reg08_FrLsb = (i_freq & 0x000000ff);
	
	// Фактор расширения спектра
	modem->confRegs.Reg1e_ModemConfig2 &=~ LORA_MODEMCFG2_SPREADING_MASK;
	modem->confRegs.Reg1e_ModemConfig2 |= config->spreadingFactor << LORA_MODEMCFG2_SPREADING_OFF;
	
	// Кодингрэйт
	modem->confRegs.Reg1d_ModemConfig1 &=~ LORA_MODEMCFG1_ERRCODERATE_MASK;
	modem->confRegs.Reg1d_ModemConfig1 |= config->errorCodeRate << LORA_MODEMCFG1_ERRCODERATE_OFF;
	
	// Ширина полосы
	modem->confRegs.Reg1d_ModemConfig1 &=~ LORA_MODEMCFG1_BANDWIDTH_MASK;
	modem->confRegs.Reg1d_ModemConfig1 |= config->bandWidth << LORA_MODEMCFG1_BANDWIDTH_OFF;
	
	// Аппаратный заголовок вкл/выкл
	if (config->headerType == SX1276_LORA_HEADER_IMPLICIT) modem->confRegs.Reg1d_ModemConfig1 |= LORA_MODEMCFG1_HEADERMODE;
	else modem->confRegs.Reg1d_ModemConfig1 &=~ LORA_MODEMCFG1_HEADERMODE;
	
	// Размер пакета задается каждый раз, при передаче пакета.
	modem->confRegs.Reg23_MaxPayloadLength = MAX_PAYLOAD_SIZE;
	modem->confRegs.Reg22_PayloadLength = MAX_PAYLOAD_SIZE;
	
	// Преамбула
	modem->confRegs.Reg20_PreambleMSB = (config->preambleLength & 0xff00) >> 8;
	modem->confRegs.Reg21_PreambleLSB = (config->preambleLength & 0x00ff);
	
	// Включаем оптимизатор и автоподстройку усилителя
	modem->confRegs.Reg26_ModemConfig3 |= (LORA_MODEMCFG3_AGCAUTO); // | LORA_MODEMCFG3_LOWDATARATE);
	
	// Выставим таймаут ожидания синхронизации
	modem->confRegs.Reg1e_ModemConfig2 &=~ LORA_RXTIMEOUT_MSB_MASK;
	modem->confRegs.Reg1e_ModemConfig2 |= (LORA_RXSYNC_TIMEOUT & 0x0300) >> 8;
	modem->confRegs.Reg1f_SymbTimeoutLSB = (LORA_RXSYNC_TIMEOUT & 0x00ff);
	
	// Настройки выходного усилителя
	modem->confRegs.Reg09_PAConfig &=~ (LORA_REG_PACONFIG_MAXPOWER_MASK | LORA_REG_PACONFIG_OUTPOWER_MASK);
	modem->confRegs.Reg09_PAConfig |= config->maxPower << LORA_REG_PACONFIG_MAXPOWER_OFF;
	modem->confRegs.Reg09_PAConfig |= config->outputPower;
	if (config->outputPin == SX1276_LORA_OUT_PABOOST) modem->confRegs.Reg09_PAConfig |= LORA_REG_PACONFIG_PASEL;
	else modem->confRegs.Reg09_PAConfig &=~ LORA_REG_PACONFIG_PASEL;
	
	// Записываем конфигурацию в трансивер
	sx1276WriteRegs (&modem->Control, LORA_REGOPMODE, (uint8_t*)&modem->confRegs + 1, sizeof (sx1276_lora_regs) - 1);
	
	// Пробуждаем трансивер
	modem->confRegs.Reg01_OpMode |= SX1276_LORA_MODE_STDBY;
	sx1276WriteReg (&modem->Control, LORA_REGOPMODE, modem->confRegs.Reg01_OpMode);
	delay_ms (10);
	
	// Сбрасываем все флаги прерываний
	sx1276WriteReg (&modem->Control, LORA_REG_IRQFLAG, 0xff);
	
	modem->receiver.bytesReceived = 0;
	modem->receiver.readyRead = 0;
	modem->curState = SX1276_STATE_IDLE;
	modem->prevState = SX1276_STATE_IDLE;
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция обмена байтами по SPI																					*/
/* ======================================================================	*/
static uint8_t sx1276SpiXmit (SPI_TypeDef *SPIx, uint8_t byte)
{
	SPIx->DR = byte;
	while (!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR;
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция считывания регистра																						*/ 
/* ======================================================================	*/
static uint8_t sx1276ReadReg (sx1276_SPIControl_t *SPIx, uint8_t addr)
{
	uint8_t tmp;
	
	SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
	delay_us (2);
	sx1276SpiXmit (SPIx->SPIx, addr);
	tmp = sx1276SpiXmit (SPIx->SPIx, DUMMY_BYTE);
	SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
	delay_us (2);
	return tmp;
}
/* ======================================================================	*/


/* ======================================================================	*/
/* Функция записи регистра																								*/
/* ======================================================================	*/
static void sx1276WriteReg (sx1276_SPIControl_t *SPIx, uint8_t addr, uint8_t value)
{
	SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
	delay_us (2);
	sx1276SpiXmit (SPIx->SPIx, (addr | SPI_WRITE_SIGN));
	sx1276SpiXmit (SPIx->SPIx, value);
	SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
	delay_us (2);	
}
/* ======================================================================	*/


/* ======================================================================	*/
/* Функция считывания нескольких регистров																*/
/* ======================================================================	*/
static void sx1276ReadRegs (sx1276_SPIControl_t *SPIx, uint8_t sAddr, uint8_t *buffer, uint8_t len)
{
	uint8_t l;
	
	SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
	delay_us (2);
	sx1276SpiXmit (SPIx->SPIx, sAddr);
	for (l = 0; l < len; l++)
		*(buffer + l) = sx1276SpiXmit (SPIx->SPIx, DUMMY_BYTE);
	SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
	delay_us (2);
}
/* ======================================================================	*/


/* ======================================================================	*/
/* Функция записи нескольких регистров																		*/
/* ======================================================================	*/
static void sx1276WriteRegs (sx1276_SPIControl_t *SPIx, uint8_t sAddr, uint8_t *data, uint8_t len)
{
	uint8_t l;
	
	SPIx->CS_PORT->ODR &=~ SPIx->CS_Pin;
	delay_us (2);
	sx1276SpiXmit (SPIx->SPIx, (sAddr | SPI_WRITE_SIGN));
	for (l = 0; l < len; l++)
		sx1276SpiXmit (SPIx->SPIx, *(data + l));
	SPIx->CS_PORT->ODR |= SPIx->CS_Pin;
	delay_us (2);
	
	
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция переключения режимов работы трансивера 												*/
/* Входные аргументы: *modem- указатель на объект LoRa модема							*/
/* 										mode - режим на который необходимо переключиться		*/
/* Возвращаемое значение: нет																							*/
/* ======================================================================	*/
void sx1276_LoRa_SetMode (sx1276_lora_t *modem, sx1276_lora_mode_t mode)
{
	modem->confRegs.Reg01_OpMode = sx1276ReadReg (&modem->Control, LORA_REGOPMODE);
	modem->confRegs.Reg01_OpMode &=~ LORA_REGOPMODE_MODE_MASK;
	modem->confRegs.Reg01_OpMode |= mode;
	sx1276WriteReg (&modem->Control, LORA_REGOPMODE, modem->confRegs.Reg01_OpMode);
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция установки частоты 																							*/
/* Входные аргументы: *modem- указатель на объект LoRa модема							*/
/*                    frequency - частота в герцах												*/
/* ======================================================================	*/
void sx1276_LoRa_SetFrequency (sx1276_lora_t *modem, uint32_t frequency)
{
	uint32_t i_freq;
	float f_freq;
	uint8_t tmp;
	
	// Переводим в стэндбай
	tmp = modem->confRegs.Reg01_OpMode;
	tmp &=~ LORA_REGOPMODE_MODE_MASK;
	sx1276WriteReg (&modem->Control, LORA_REGOPMODE, tmp);
	delay_ms (10);
	
	// Выставим требуемую частоту
	f_freq = (float)frequency / modem->Control.oscFrequency;
	f_freq *= TWO_IN_NINETEEN;
	i_freq = (uint32_t)f_freq;
	modem->confRegs.Reg06_FrMsb = (i_freq & 0x00ff0000) >> 16;
	modem->confRegs.Reg07_FrMid = (i_freq & 0x0000ff00) >> 8;
	modem->confRegs.Reg08_FrLsb = (i_freq & 0x000000ff);

	// Запишем значение в трансивер
	sx1276WriteRegs (&modem->Control, LORA_REG_FREQ_MSB, &modem->confRegs.Reg06_FrMsb, 3);
	
	// Восстановим предыдущий режим
	//sx1276WriteReg (&modem->Control, LORA_REGOPMODE, modem->confRegs.Reg01_OpMode);
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция запроса текущей частоты работы 																*/
/* Входные аргументы: *modem- указатель на объект LoRa модема							*/
/* Возвращаемое значение: частота в герцах																*/
/* ======================================================================	*/
uint32_t sx1276_LoRa_GetFrequency	(sx1276_lora_t *modem)
{
	uint32_t i_freq;
	uint32_t f_freq;
	
	i_freq = (modem->confRegs.Reg06_FrMsb << 16) | (modem->confRegs.Reg07_FrMid << 8) | (modem->confRegs.Reg08_FrLsb);
	f_freq = (float)i_freq / TWO_IN_NINETEEN;
	f_freq *= modem->Control.oscFrequency;
	i_freq = (uint32_t)f_freq;
	
	return i_freq;
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функиция отправки пакета в режиме LoRa modem														*/
/* Входные аргументы: *modem - указатель на объект LoRa modem							*/
/*										*data - указатель на буфер с данными								*/
/*										length - длина данных на отправку										*/
/* Возвращаемое значение: нет																							*/
/* ======================================================================	*/
void sx1276_LoRa_sendPacket (sx1276_lora_t *modem, uint8_t *data, uint8_t length)
{
	modem->prevState = modem->curState;
	modem->curState = SX1276_STATE_TX;

	// Установим указатель ФИФо на начало буфера для отправки
	sx1276WriteReg (&modem->Control, LORA_REG_FIFOPTR, LORA_FIFO_BASETX);
	
	// Записываем послание в фифо
	sx1276WriteRegs (&modem->Control, LORA_REGFIFO_IO, data, length);
	
	// Выставляем длину послания
	sx1276WriteReg (&modem->Control, LORA_REG_PAYLOADLENGTH, length);
	
	// Включаем передачу
	modem->busy = 1;
	sx1276_LoRa_SetMode (modem, SX1276_LORA_MODE_TX);
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция разрешения приема в режиме LoRa modem													*/
/* Входные аргументы: *modem - указатель на объект LoRa modem							*/
/* Возвращаемое значение: нет																							*/
/* ======================================================================	*/
void sx1276_LoRa_startRx (sx1276_lora_t *modem)
{
	modem->curState = SX1276_STATE_RX;
	sx1276_LoRa_SetMode (modem, SX1276_LORA_MODE_RXCONTINUOUS);
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция запрета приема в режиме LoRa modem															*/
/* Входные аргументы: *modem - указатель на объект LoRa modem							*/
/* Возвращаемое значение: нет																							*/
/* ======================================================================	*/
void sx1276_LoRa_stopRx (sx1276_lora_t *modem)
{
	modem->curState = SX1276_STATE_IDLE;
	sx1276_LoRa_SetMode (modem, SX1276_LORA_MODE_STDBY);
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция для переодического опроса состояния трансивера									*/
/* Происходит опрос флагов прерываний, для определения - был ли принят		*/
/* пакет данных и завершена ли передача пакета														*/
/* В случае завершения передачи - параметр curState переключится на сос-	*/
/* тояние, предшествующее началу передачи данных													*/
/* В случае приема пакета - вывесится флаг readyRead, сам пакет будет ско	*/
/* пирован в буфер приема, а в параметре bytesReceived будет записано			*/
/* число принятых байт. Также в параметр packRssi запишется rssi приня-		*/
/* того пакета.																														*/
/* Входные аргументы: *modem - указатель на объект LoRa modem							*/
/* Возвращаемое значение: нет																							*/
/* ======================================================================	*/
void sx1276_LoRa_updateState (sx1276_lora_t *modem)
{
	uint8_t tmp, fifoAddr;
	uint8_t *rPnt, rCnt;
	uint8_t rawRssi;
	int8_t rawSnr;
	int8_t rawNoise;
	
	// Как-то мимоходом свелась на нет все аппаратная часть с прерываниями.
	// Можно все легко обработать программно, без запарки с ремапом ног и внешними прерываниями
	// Запросим состояние регистра прерываний
	tmp = sx1276ReadReg (&modem->Control, LORA_REG_IRQFLAG);
		
	if (tmp & LORA_INTFLAG_TX)
	{
		// Завершена передача - выставляем обратно трансивер в прием или ожидание
		if (modem->prevState != SX1276_STATE_IDLE)
		{
			// До этого трансивер находился на приеме - туда его и возвратим. 
			sx1276_LoRa_SetMode (modem, SX1276_LORA_MODE_RXCONTINUOUS);
			modem->curState = SX1276_STATE_RX;
		}
		else
		{
			// Если он находился в ожидании - туда он сам вернется
			modem->curState = SX1276_STATE_IDLE;
		}		
		modem->busy = 0;
		sx1276WriteReg (&modem->Control, LORA_REG_FIFOPTR, LORA_FIFO_BASERX);
		sx1276WriteReg (&modem->Control, LORA_REG_IRQFLAG, LORA_INTFLAG_TX);
	}
	
	if (tmp & LORA_INTFLAG_RX)
	{
		sx1276WriteReg (&modem->Control, LORA_REG_IRQFLAG, LORA_INTFLAG_RX);
		// Приняли что-то. Считаем данные из фифо и вывесим флаг готовности
		rPnt = modem->receiver.rxBuffer;
		
		// Запросим число принятых байт
		rCnt = sx1276ReadReg (&modem->Control, LORA_REG_NUM_RECEIVED_BYTES);
		modem->receiver.bytesReceived = rCnt;
		
		fifoAddr = sx1276ReadReg (&modem->Control, LORA_REG_FIFOCURADDR);
		sx1276WriteReg (&modem->Control, LORA_REG_FIFOPTR, fifoAddr);
		
		// И считаем их из фифо
		for (; rCnt; rCnt--)
			*rPnt++ = sx1276ReadReg (&modem->Control, LORA_REGFIFO_IO);
		
		// Запросим RSSI принятого пакета
		rawRssi = sx1276ReadReg (&modem->Control, LORA_PKTRSSI);
		rawSnr = sx1276ReadReg (&modem->Control, LORA_PKTSNR);
		rawNoise = sx1276ReadReg (&modem->Control, LORA_SIGRSSI);
		
		rawRssi *= 1.06;
		
		if (rawSnr >= 0) modem->receiver.rssi = 156 - rawRssi;
		else modem->receiver.rssi = 156 - rawRssi - rawSnr / 4;		
		
//		modem->receiver.snr =  ((rawRssi - rawNoise) > 127)?(127):(modem->receiver.rssi - rawNoise);
		
		//modem->receiver.rssi = sx1276ReadReg (&modem->Control, LORA_PKTRSSI);
		
		modem->receiver.readyRead = 1;
		
		//sx1276WriteReg (&modem->Control, LORA_REG_FIFOPTR, LORA_FIFO_BASERX);
	}
}
/* ======================================================================	*/

/* ======================================================================	*/
/* Функция получения значения RSSI в текущий момент времени для заданной	*/
/* частоты																																*/
/* Входные аргументы: *modem - указатель на объект LoRa modem							*/
/* Возвращаемое значение: RSSI сигнала на текущей частоте									*/
/* ======================================================================	*/
uint8_t sx1276_LoRa_getRssi (sx1276_lora_t *modem)
{
	return sx1276ReadReg (&modem->Control, LORA_SIGRSSI);
}
/* ======================================================================	*/
