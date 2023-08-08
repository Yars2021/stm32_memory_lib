#ifndef _SX1276_TRANSC_H_
#define _SX1276_TRANSC_H_

#include "sx1276_common.h"

/* ======================================================================	*/
/* = Дефайны всяких очень нужных констант											     				*/
/* ======================================================================	*/
#define TWO_IN_NINETEEN											524288            // Два в девятьнадцатой степени. Для вычисления значений регистра синтезатора частоты
#define LORA_RXSYNC_TIMEOUT									((uint16_t)0x03ff)// Время таймаута для ожидания синхронизации при приеме (в аппнотах и экзамплах этот параметр ставится тупо на максимум, не будем ломать традиции)
#define DUMMY_BYTE													((uint8_t)0x55)		// Байт, который будет отправляться для считывания данных по SPI
#define SPI_WRITE_SIGN											((uint8_t)0x80)		// Старший бит в адресе определяет режим обращения к регистру - запись
#define MAX_PAYLOAD_SIZE										((uint8_t)0xff)		// Максимальный размер пакета передаваемый/принимаемый трансивером
/* ======================================================================	*/

/* ======================================================================	*/
/* = Адреса и битовые маски регистров трансивера								   				*/
/* ======================================================================	*/
#define LORA_REGFIFO_IO											((uint8_t)0x00)
#define LORA_REGOPMODE											((uint8_t)0x01)
#define LORA_REGOPMODE_LORAMODE							((uint8_t)0x80)
#define LORA_REGOPMODE_ACCESSSHARE					((uint8_t)0x40)
#define LORA_REGOPMODE_LOWBAND							((uint8_t)0x08)
#define LORA_REGOPMODE_SLEEP								((uint8_t)0x00)
#define LORA_REGOPMODE_MODE_MASK						((uint8_t)0x07)
#define LORA_REG_FREQ_MSB										((uint8_t)0x06)
#define LORA_REG_FREQ_MID										((uint8_t)0x07)
#define LORA_REG_FREQ_LSB										((uint8_t)0x08)
#define LORA_REG_PACONFIG										((uint8_t)0x09)
#define LORA_REG_FIFOCURADDR								((uint8_t)0x10)
#define LORA_REG_PACONFIG_PASEL							((uint8_t)0x80)
#define LORA_REG_PACONFIG_MAXPOWER_OFF			4
#define LORA_REG_PACONFIG_MAXPOWER_MASK			((uint8_t)0x70)
#define LORA_REG_PACONFIG_OUTPOWER_MASK			((uint8_t)0x0f)
#define LORA_REG_OCP												((uint8_t)0x0b)
#define LORA_OCP_OCPON											((uint8_t)0x20)
#define LORA_OCP_OCPTRIM_MASK								((uint8_t)0x1f)
#define LORA_REG_LNA												((uint8_t)0x0c)
#define LORA_LNA_GAIN_MASK									((uint8_t)0xe0)
#define LORA_LNA_GAIN_OFF										5
#define LORA_LNA_LFBOOST_MASK								((uint8_t)0x18)
#define LORA_LNA_HFBOOST_MASK								((uint8_t)0x03)
#define LORA_REG_MODEMCFG1									((uint8_t)0x1d)
#define LORA_MODEMCFG1_BANDWIDTH_MASK				((uint8_t)0xf0)
#define LORA_MODEMCFG1_BANDWIDTH_OFF				4
#define LORA_MODEMCFG1_ERRCODERATE_MASK			((uint8_t)0x0e)
#define LORA_MODEMCFG1_ERRCODERATE_OFF			1
#define LORA_MODEMCFG1_HEADERMODE						((uint8_t)0x01)
#define LORA_REG_MODEMCFG2									((uint8_t)0x1e)
#define LORA_MODEMCFG2_SPREADING_MASK				((uint8_t)0xf0)
#define LORA_MODEMCFG2_SPREADING_OFF				4
#define LORA_MODEMCFG2_TXCONTINUE						((uint8_t)0x08)
#define LORA_MODEMCFG2_CRCPAYLOAD						((uint8_t)0x04)
#define LORA_RXTIMEOUT_MSB_MASK							((uint8_t)0x03)
#define LORA_REG_RXTIME											((uint8_t)0x1f)
#define LORA_REG_PREAMBLE_MSB								((uint8_t)0x20)
#define LORA_REG_PREAMBLE_LSB								((uint8_t)0x21)
#define LORA_REG_PAYLOADLENGTH							((uint8_t)0x22)
#define LORA_REG_MODEMCFG3									((uint8_t)0x26)
#define LORA_MODEMCFG3_LOWDATARATE					((uint8_t)0x08)
#define LORA_MODEMCFG3_AGCAUTO							((uint8_t)0x04)
#define LORA_REG_FIFOPTR										((uint8_t)0x0d)
#define LORA_FIFO_BASETX										((uint8_t)0x80)
#define LORA_FIFO_BASERX										((uint8_t)0x00)
#define LORA_REG_LASTPACKET_FIFOADDR				((uint8_t)0x10)
#define LORA_REG_IRQMASK										((uint8_t)0x11)
#define LORA_REG_IRQFLAG										((uint8_t)0x12)
#define LORA_INTFLAG_TX											((uint8_t)0x08)
#define LORA_INTFLAG_RX											((uint8_t)0x40)
#define LORA_INTFLAG_TIMEOUT								((uint8_t)0x80)
#define LORA_REG_NUM_RECEIVED_BYTES					((uint8_t)0x13)
#define LORA_REG_DIOMAP1										((uint8_t)0x40)
#define LORA_REG_DIOMAP2										((uint8_t)0x41)
#define LORA_INTRX_MAP											((uint8_t)0x00)
#define LORA_INTTX_MAP											((uint8_t)0x40)
#define LORA_REG_TCXO												((uint8_t)0x4b)
#define LORA_TCXO_USE_TCXO									((uint8_t)0x19)
#define LORA_PKTSNR													((uint8_t)0x19)
#define LORA_PKTRSSI												((uint8_t)0x1a)
#define LORA_SIGRSSI												((uint8_t)0x1b)

/* ======================================================================	*/

typedef struct
{
	uint8_t rxBuffer[MAX_PAYLOAD_SIZE];
	uint8_t readyRead;
	uint8_t bytesReceived;
	int8_t rssi;
	int8_t noise;
} sx1276_lora_receiver_t;

typedef struct 
{
	uint8_t Reg00_Fifo;													// FIFO read/write access
	uint8_t Reg01_OpMode;												// Operation modes & LORA / FSK selection
	uint8_t Reg02_UnusedLORA;
	uint8_t Reg03_UnusedLORA;
	uint8_t Reg04_UnusedLORA;
	uint8_t Reg05_UnusedLORA;
	uint8_t Reg06_FrMsb;												// RF Carrier frequency. Most significant bits
	uint8_t Reg07_FrMid;												// RF Carrier frequency. Intermediate bits
	uint8_t Reg08_FrLsb;												// RF Carrier frequency. Least significant bits
	uint8_t Reg09_PAConfig;											// PA selection and output power control
	uint8_t Reg0A_PARamp;												// Control of PA ramp time, low phase noise PLL
	uint8_t Reg0B_Ocp;													// Over current protection control
	uint8_t Reg0C_LNA;													// LNA settings
	uint8_t Reg0D_FIFOAddrPtr;									// FIFO SPI Pointer
	uint8_t Reg0E_FIFOTxBaseAddr;								// Start TX data
	uint8_t Reg0F_FIFORxBaseAddr;								// Start RX data
	uint8_t Reg10_FIFORxCurrentAddr;						// Start address of last packet received
	uint8_t Reg11_IRQFlagMask;									// Optional interrupts mask
	uint8_t Reg12_IRQFlags;											// Interrupt flags
	uint8_t Reg13_RxNumBytes;										// Number of received bytes
	uint8_t Reg14_RxHeadersCntMsb;							// Number of valid headers received (MSB)
	uint8_t Reg15_RxHeadersCntLsb;							// Number of valid headers received (LSB)
	uint8_t Reg16_RxPacketsCntMsb;							// Number of valid packets received (MSB)
	uint8_t Reg17_RxPacketsCntLsb;							// Number of valid packets received (LSB)
	uint8_t Reg18_ModemStat;										// Live LORA modem status
	uint8_t Reg19_PktSNRValue;									// Signal to Noise Ration (SNR) espimation of last packet
	uint8_t Reg1a_PktRSSIValue;									// Received Signal Streinght Indication (RSSI) of last packet
	uint8_t Reg1b_RSSIValue;										// Current Received Signal Streinght Indication (RSSI) value
	uint8_t Reg1c_HOPChannel;										// Frequency Hoping Spread Spectrum (FHSS) start channel
	uint8_t Reg1d_ModemConfig1;									// Modem PHY config 1
	uint8_t Reg1e_ModemConfig2;									// Modem PHY config 2
	uint8_t Reg1f_SymbTimeoutLSB;								// Receiver timeout value
	uint8_t Reg20_PreambleMSB;									// Size of preamble (MSB)
	uint8_t Reg21_PreambleLSB;									// Size of preamble (LSB)
	uint8_t Reg22_PayloadLength;								// LORA payload length
	uint8_t Reg23_MaxPayloadLength;							// LORA maximum payload length
	uint8_t Reg24_HOPPeriod;										// Frequency Hoping Spread Spectrum (FHSS) period
	uint8_t Reg25_FIFORxByteAddr;								// Address of last byte written to FIFO
	uint8_t Reg26_ModemConfig3;									// Modem PHY config 3
	uint8_t Reg27_Reserved;											
	uint8_t Reg28_FeiMsb;												// Estimated frequency error (MSB)
	uint8_t Reg29_FeiMid;												// Estimated frequency error (MID)
	uint8_t Reg2a_FeiLsb;												// Estimated frequency error (LSB)
	uint8_t Reg2b_Reserved;
	uint8_t Reg2c_RSSIWideBandMeasure;					// Wideband RSSI measurement
	uint8_t Reg2d_Reserved;
	uint8_t Reg2e_Reserved;
	uint8_t Reg2f_Reserved;
	uint8_t Reg30_Reserved;
	uint8_t Reg31_DetectOptimize;								// LORA detection optimize for spread spectrum factor = 6
	uint8_t Reg32_Reserved;
	uint8_t Reg33_InvertIQ;											// Inver LORA I and Q signals
	uint8_t Reg34_Reserved;
	uint8_t Reg35_Reserved;
	uint8_t Reg36_Reserved;
	uint8_t Reg37_DetectionThreshold;						// LORA detection threshold for spread spectrum factor = 6
	uint8_t Reg38_Reserved;
	uint8_t Reg39_SyncWord;											// LORA Sync word
	uint8_t Reg3a_Reserved;
	uint8_t Reg3b_Reserved;
	uint8_t Reg3c_Reserved;
	uint8_t Reg3d_Reserved;
	uint8_t Reg3e_Reserved;
	uint8_t Reg3f_Reserved;
	uint8_t Reg40_DIOMapping1;									// Digital IN/OUT pins mapping 1
	uint8_t Reg41_DIOMapping2;									// Digital IN/OUT pins mapping 2
	uint8_t Reg42_Version;											// Semtech ID relating the silicon revesion
	uint8_t Reg43_None;
	uint8_t Reg44_UnusedLORA;
	uint8_t Reg45_None;
	uint8_t Reg46_None;
	uint8_t Reg47_None;
	uint8_t Reg48_None;
	uint8_t Reg49_None;
	uint8_t Reg4a_None;
	uint8_t Reg4b_TCXO;													// TCXO or XTAL input selector
	uint8_t Reg4c_None;
	uint8_t Reg4d_PADac;												// Higher power setting of the PA
	uint8_t Reg4e_None;
	uint8_t Reg4f_None;
	uint8_t Reg50_None;
	uint8_t Reg51_None;
	uint8_t Reg52_None;
	uint8_t Reg53_None;
	uint8_t Reg54_None;
	uint8_t Reg55_None;
	uint8_t Reg56_None;
	uint8_t Reg57_None;
	uint8_t Reg58_None;
	uint8_t Reg59_None;
	uint8_t Reg5a_None;
	uint8_t Reg5b_FormerTemp;										// Stored temperature during the former IQ Calibration
	uint8_t Reg5c_None;
	uint8_t Reg5d_UnusedLORA;
	uint8_t Reg5e_None;
	uint8_t Reg5f_None;
	uint8_t Reg60_None;
	uint8_t Reg61_AgcRef;												// Adjustment of the Auto Gain Control (AGC) thresholds
	uint8_t Reg62_AgcThresh1;										// Adjustment of the Auto Gain Control (AGC) thresholds
	uint8_t Reg63_AgcThresh2;										// Adjustment of the Auto Gain Control (AGC) thresholds
	uint8_t Reg64_AgcThresh3;										// Adjustment of the Auto Gain Control (AGC) thresholds
	uint8_t Reg65_None;
	uint8_t Reg66_None;
	uint8_t Reg67_None;
	uint8_t Reg68_None;
	uint8_t Reg69_None;
	uint8_t Reg6a_None;
	uint8_t Reg6b_None;
	uint8_t Reg6c_None;
	uint8_t Reg6d_None;
	uint8_t Reg6e_None;
	uint8_t Reg6f_None;
	uint8_t Reg70_PLL;													// Control of the PLL bandwidth
} sx1276_lora_regs;

/* ======================================================================	*/
/* Структуры для режима LoRa Modem																				*/
/* ======================================================================	*/
typedef enum {
	SX1276_LORA_BAND_HIGH,
	SX1276_LORA_BAND_LOW
} sx1276_lora_band_t;

typedef enum {
	SX1276_LORA_MODE_SLEEP,
	SX1276_LORA_MODE_STDBY,
	SX1276_LORA_MODE_FSTX,
	SX1276_LORA_MODE_TX,
	SX1276_LORA_MODE_FSRX,
	SX1276_LORA_MODE_RXCONTINUOUS,
	SX1276_LORA_MODE_RXSINGLE,
	SX1276_LORA_MODE_CAD
} sx1276_lora_mode_t;

typedef enum {
	SX1276_LORA_OUT_RFO,
	SX1276_LORA_OUT_PABOOST
} sx1276_lora_outputPin_t;

typedef enum {
	SX1276_LORA_BANDWIDTH_7800,
	SX1276_LORA_BANDWIDTH_10400,
	SX1276_LORA_BANDWIDTH_15600,
	SX1276_LORA_BANDWIDTH_20800,
	SX1276_LORA_BANDWIDTH_31250,
	SX1276_LORA_BANDWIDTH_41700,
	SX1276_LORA_BANDWIDTH_62500,
	SX1276_LORA_BANDWIDTH_125000,
	SX1276_LORA_BANDWIDTH_250000,
	SX1276_LORA_BANDWIDTH_500000
} sx1276_lora_bandwidth_t;

typedef enum {
	SX1276_LORA_ECR0_0,
	SX1276_LORA_ECR4_5,
	SX1276_LORA_ECR4_6,
	SX1276_LORA_ECR4_7,
	SX1276_LORA_ECR4_8
} sx1276_lora_error_code_t;

typedef enum {
	SX1276_LORA_HEADER_EXPLICIT,
	SX1276_LORA_HEADER_IMPLICIT
} sx1276_lora_header_t;

typedef enum {
	SX1276_LORA_CRCPAYLOAD_DISABLE,
	SX1276_LORA_CRCPAYLOAD_ENABLE
} sx1276_lora_crc_t;

typedef enum {
	SX1276_LORA_OSC_CRYSTAL,
	SX1276_LORA_OSC_TCXO
} sx1276_lora_osctype_t;

/* ======================================================================	*/
/* Конфигурация LoRa модема, так сказать хай левел, в отличии от регистров*/
/* ======================================================================	*/
typedef struct 
{
	SPI_TypeDef *SPIx;
	GPIO_TypeDef *CS_PORT;
	uint32_t CS_Pin;
	GPIO_TypeDef *RESET_PORT;
	uint32_t Reset_Pin;
	sx1276_lora_osctype_t oscType;
	sx1276_lora_band_t band;
	uint32_t frequency;
	sx1276_lora_outputPin_t outputPin;
	uint8_t maxPower; 													// 0 - 7 (10.8 - 15 dBm, step 0.6 dBm)
	uint8_t outputPower;												// 0 - 15 (Pmax - (15 - REGVALUE) dBm)
	sx1276_lora_bandwidth_t bandWidth;
	sx1276_lora_error_code_t errorCodeRate;
	sx1276_lora_header_t headerType;
	uint8_t spreadingFactor;										// 6 - 12
	sx1276_lora_crc_t crcPayload;
	uint16_t preambleLength;
	uint32_t oscillatorFrequency;
} sx1276_LoRa_InitTypeDef;
/* ======================================================================	*/

/* ======================================================================	*/
/* Структура для экземпляра объекта	для работы в режиме LoRa Modem				*/
/* ======================================================================	*/
typedef struct
{
	sx1276_SPIControl_t Control;
	sx1276_lora_regs confRegs;
	sx1276_lora_receiver_t receiver;
    uint8_t busy;
    sx1276_state_t curState;
    sx1276_state_t prevState;
} sx1276_lora_t;
/* ======================================================================	*/

void sx1276_InitLoRaMode (sx1276_lora_t *modem, sx1276_LoRa_InitTypeDef *config);
void sx1276_LoRa_SetMode (sx1276_lora_t *modem, sx1276_lora_mode_t mode);
void sx1276_LoRa_SetFrequency (sx1276_lora_t *modem, uint32_t frequency);
uint32_t sx1276_LoRa_GetFrequency	(sx1276_lora_t *modem);
void sx1276_LoRa_sendPacket (sx1276_lora_t *modem, uint8_t *data, uint8_t length);
void sx1276_LoRa_startRx (sx1276_lora_t *modem);
void sx1276_LoRa_stopRx (sx1276_lora_t *modem);
uint8_t sx1276_LoRa_getRssi (sx1276_lora_t *modem);
void sx1276_LoRa_updateState (sx1276_lora_t *modem);

#endif /* _SX1276_TRANSC_H_ */
