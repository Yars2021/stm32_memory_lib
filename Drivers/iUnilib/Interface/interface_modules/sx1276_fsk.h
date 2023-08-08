//
// Created by Kharitonenko Vladimir on 10.03.2022.
//

#ifndef DYNAMIC_STAND_BASE_SX1276_FSK_H
#define DYNAMIC_STAND_BASE_SX1276_FSK_H

#include "sx1276_common.h"

#define DUMMY_BYTE                                              ((uint8_t)0x55)
#define SPI_WRITE_SIGN											((uint8_t)0x80)		// Старший бит в адресе определяет режим обращения к регистру - запись

#define TWOINNINETEEN                                           ((uint32_t)524288)

#define SX1276_FSK_BAND1_LOW                                    137000000
#define SX1276_FSK_BAND1_HIGH                                   175000000
#define SX1276_FSK_BAND2_LOW                                    410000000
#define SX1276_FSK_BAND2_HIGH                                   525000000
#define SX1276_FSK_BAND3_LOW                                    862000000
#define SX1276_FSK_BAND3_HIGH                                   1020000000

#define SX1276_FSK_MAX_TRY                                      5

#define SX1276_FSK_REG_FIFO                                     ((uint8_t)0x00)
#define SX1276_FSK_REGOPMODE                                    ((uint8_t)0x01)
#define SX1276_FSK_OPMODE                                       ((uint8_t)0x00)
#define SX1276_FSK_OPMODE_OOK                                   ((uint8_t)0x20)
#define SX1276_FSK_OPMODE_LOWFREQ                               ((uint8_t)0x08)
#define SX1276_FSK_OPMODE_MASK                                  ((uint8_t)0x07)
#define SX1276_FSK_BITRATE_MSB                                  ((uint8_t)0x02)
#define SX1276_FSK_BITRATE_LSB                                  ((uint8_t)0x03)
#define SX1276_FSK_BITRATE_FRAC                                 ((uint8_t)0x5d)
#define SX1276_FSK_DEVIATION_MSB                                ((uint8_t)0x04)
#define SX1276_FSK_DEVIATION_LSB                                ((uint8_t)0x05)
#define SX1276_FSK_FCARRIER_MSB                                 ((uint8_t)0x06)
#define SX1276_FSK_FCARRIER_MID                                 ((uint8_t)0x07)
#define SX1276_FSK_FCARRIER_LSB                                 ((uint8_t)0x08)
#define SX1276_FSK_PACONF                                       ((uint8_t)0x09)
#define SX1276_FSK_PACONF_PABOOST                               ((uint8_t)0x80)
#define SX1276_FSK_PACONF_MAXPOWER_SHIFT                        4
#define SX1276_FSK_PACONF_MAXPOWER_MASK                         ((uint8_t)0x07)
#define SX1276_FSK_PACONF_OUTPOWER_MASK                         ((uint8_t)0x0f)
#define SX1276_FSK_PARAMP                                       ((uint8_t)0x0a)
#define SX1276_FSK_PARAMP_DEFAULT                               ((uint8_t)0x09)
#define SX1276_FSK_PARAM_SHAPE_SHIFT                            5
#define SX1276_FSK_RXCONF                                       ((uint8_t)0x0d)
#define SX1276_FSK_RXCONF_AUTORESTART                           ((uint8_t)0x80)
#define SX1276_FSK_RXCONF_RESTART_NOPLL                         ((uint8_t)0x40)
#define SX1276_FSK_RXCONF_RESTART_WITHPLL                       ((uint8_t)0x20)
#define SX1276_FSK_RXCONF_AFC                                   ((uint8_t)0x10)
#define SX1276_FSK_RXCONF_AGC                                   ((uint8_t)0x08)
#define SX1276_FSK_RXCONF_TRIGGER_MASK                          ((uint8_t)0x07)
#define SX1276_FSK_RXCONF_TRIGGER_NONE                          ((uint8_t)0x00)
#define SX1276_FSK_RXCONF_TRIGGER_RSSI                          ((uint8_t)0x01)
#define SX1276_FSK_RXCONF_TRIGGER_PREAMBLE                      ((uint8_t)0x06)
#define SX1276_FSK_RXCONF_TRIGGER_RSSI_AND_PREAMBLE             ((uint8_t)0x07)
#define SX1276_FSK_REG_RSSI                                     ((uint8_t)0x11)
#define SX1276_FSK_BWRX                                         ((uint8_t)0x12)
#define SX1276_FSK_BWAFCRX                                      ((uint8_t)0x13)
#define SX1276_FSK_BW_MANT_SHIFT                                3
#define SX1276_FSK_AFC_CLEAR_REG                                ((uint8_t)0x1a)
#define SX1276_FSK_AFC_CLEAR_REG_ENABLE                         ((uint8_t)0x01)
#define SX1276_FSK_PREAMBLE_DETECTOR                            ((uint8_t)0x1f)
#define SX1276_FSK_PREAMBLE_DETECTOR_DEFAULT                    ((uint8_t)0xca)
#define SX1276_FSK_RSSI_TIMEOUT                                 ((uint8_t)0x20)
#define SX1276_FSK_PREAMBLE_TIMEOUT                             ((uint8_t)0x21)
#define SX1276_FSK_SYNC_TIMEOUT                                 ((uint8_t)0x22)
#define SX1276_FSK_RESTART_DELAY                                ((uint8_t)0x23)
#define SX1276_FSK_RESTART_DELAY_VALUE                          8
#define SX1276_FSK_PREAMBLE_LEN_MSB                             ((uint8_t)0x25)
#define SX1276_FSK_PREAMBLE_LEN_LSB                             ((uint8_t)0x26)
#define SX1276_FSK_SYNC_WORD                                    ((uint8_t)0x27)
#define SX1276_FSK_SYNC_WORD_DEFAULT                            ((uint8_t)0x10)
#define SX1276_FSK_SYNC_WORD_START                              ((uint8_t)0x28)
#define SX1276_FSK_PCKCONF1                                     ((uint8_t)0x30)
#define SX1276_FSK_PCKCONF1_VARIABLE_LENGTH                     ((uint8_t)0x80)
#define SX1276_FSK_PCKCONF1_ENCODE_SHIFT                        5
#define SX1276_FSK_PCKCONF1_CRC_EN                              ((uint8_t)0x10)
#define SX1276_FSK_PCKCONF2                                     ((uint8_t)0x31)
#define SX1276_FSK_PCKCONF2_PACKET_MODE                         ((uint8_t)0x40)
#define SX1276_FSK_PCKCONF2_DEFAULT                             ((uint8_t)0x00)
#define SX1276_FSK_MAXPAYLOAD                                   ((uint8_t)0x32)
#define SX1276_FSK_MAXPAYLOAD_DEFAULT                           ((uint8_t)0x80)
#define SX1276_FSK_FIFO_THRESHOLD                               ((uint8_t)0x35)
#define SX1276_FSK_FIFO_THRESHOLD_DEFAULT                       ((uint8_t)0xbf)
#define SX1276_FSK_IRQ1_FLAGS                                   ((uint8_t)0x3e)
#define SX1276_FSK_IRQ1_MODE_READY                              ((uint8_t)0x80)
#define SX1276_FSK_IRQ1_RX_READY                                ((uint8_t)0x40)
#define SX1276_FSK_IRQ1_TX_READY                                ((uint8_t)0x20)
#define SX1276_FSK_IRQ1_PLL_LOCK                                ((uint8_t)0x10)
#define SX1276_FSK_IRQ1_RSSI_THRESHOLD                          ((uint8_t)0x08)
#define SX1276_FSK_IRQ1_TIMEOUT                                 ((uint8_t)0x04)
#define SX1276_FSK_IRQ1_PREAMBLE_DETECT                         ((uint8_t)0x02)
#define SX1276_FSK_IRQ1_SYNC_MATCH                              ((uint8_t)0x01)
#define SX1276_FSK_IRQ2_FLAGS                                   ((uint8_t)0x3f)
#define SX1276_FSK_IRQ2_FIFO_FULL                               ((uint8_t)0x80)
#define SX1276_FSK_IRQ2_FIFO_EMPTY                              ((uint8_t)0x40)
#define SX1276_FSK_IRQ2_FIFO_LEVEL                              ((uint8_t)0x20)
#define SX1276_FSK_IRQ2_FIFO_OVERRUN                            ((uint8_t)0x10)
#define SX1276_FSK_IRQ2_PACKET_SEND                             ((uint8_t)0x08)
#define SX1276_FSK_IRQ2_PAYLOAD_READY                           ((uint8_t)0x04)
#define SX1276_FSK_IRQ2_CRC_OK                                  ((uint8_t)0x02)
#define SX1276_FSK_IRQ2_LOW_BAT                                 ((uint8_t)0x01)

typedef struct
{
    uint8_t regFifo;                                // 0x00     FIFO read/write access
    uint8_t regOpMode;                              // 0x01     Operation mode
    uint8_t regBitrateMsb;                          // 0x02     Bit rate settings. Most significant bits
    uint8_t regBitrateLsb;                          // 0x03     Bit rate settings. Least significant bits
    uint8_t regFDevMsb;                             // 0x04     Frequency deviation settings. Most significant bits
    uint8_t regFDevLsb;                             // 0x05     Frequency deviation settings. Least significant bits
    uint8_t regFreqMsb;                             // 0x06     RF Carrier frequency settings. Most significant bits
    uint8_t regFreqMid;                             // 0x07     RF Carrier frequency settings. Intermediate bits
    uint8_t reqFreqLsb;                             // 0x08     RF Carrier frequency settings. Least significant bits
    uint8_t regPaConfig;                            // 0x09     PA selection and output power control
    uint8_t regPaRamp;                              // 0x0a     Control of PA ramp time, low phase noise PLL
    uint8_t regOcp;                                 // 0x0b     Over current protection control
    uint8_t regLna;                                 // 0x0c     LNA settings
    uint8_t regRxConfig;                            // 0x0d     AFC, AGC controls
    uint8_t regRSSIConfig;                          // 0x0e     RSSI
    uint8_t regRSSICollision;                       // 0x0f     RSSI collision detector
    uint8_t regRSSIThreshold;                       // 0x10     RSSI threshold control
    uint8_t regRSSIValue;                           // 0x11     RSSI value in dBm
    uint8_t regRxBw;                                // 0x12     Channel filter BW control
    uint8_t regAfcBw;                               // 0x13     AFC Channel filter BW
    uint8_t regOokPeak;                             // 0x14     OOK demodulator
    uint8_t regOokFix;                              // 0x15     Threshold of the OOK demodulator
    uint8_t regOokAvg;                              // 0x16     Average of the OOK demodulator
    uint8_t reserved17;                             // 0x17
    uint8_t reserved18;                             // 0x18
    uint8_t reserved19;                             // 0x19
    uint8_t regAfcFei;                              // 0x1a     AFC and FEI control
    uint8_t regAfcMsb;                              // 0x1b     Frequency correction value
    uint8_t regAfcLsb;                              // 0x1c     of the AFC
    uint8_t regFeiMsb;                              // 0x1d     Value of the calculated
    uint8_t regFeiLsb;                              // 0x1e     frequency error
    uint8_t regPreambleDetect;                      // 0x1f     Settings of the preamble detector
    uint8_t regRxTimeout1;                          // 0x20     Timeout Rx request and RSSI
    uint8_t regRxTimeout2;                          // 0x21     Timeout RSSI and Payload Ready
    uint8_t regRxTimeout3;                          // 0x22     Timeout RSSI and SyncAddress
    uint8_t regRxDelay;                             // 0x23     Delay between Rx cycles
    uint8_t regOsc;                                 // 0x24     RC oscillators setting. CLK-Out frequency
    uint8_t regPreambleMsb;                         // 0x25     Preamble length, MSB
    uint8_t regPreambleLsb;                         // 0x26     Preamble length, LSB
    uint8_t regSyncConfig;                          // 0x27     Sync word recognition control
    uint8_t regSyncValue1;                          // 0x28     Sync word bytes 1
    uint8_t regSyncValue2;                          // 0x29     Sync word bytes 2
    uint8_t regSyncValue3;                          // 0x2a     Sync word bytes 3
    uint8_t regSyncValue4;                          // 0x2b     Sync word bytes 4
    uint8_t regSyncValue5;                          // 0x2c     Sync word bytes 5
    uint8_t regSyncValue6;                          // 0x2d     Sync word bytes 6
    uint8_t regSyncValue7;                          // 0x2e     Sync word bytes 7
    uint8_t regSyncValue8;                          // 0x2f     Sync word bytes 8
    uint8_t regPacketConfig1;                       // 0x30     Packet mode settings
    uint8_t regPacketConfig2;                       // 0x31     Packet mode settings
    uint8_t regPayloadLength;                       // 0x32     Payload length settings
    uint8_t regNodeAddress;                         // 0x33     Node address
    uint8_t regBroadcastAddress;                    // 0x34     Broadcast address
    uint8_t regFifoThreshold;                       // 0x35     Fifo threshold, Tx start condition
    uint8_t regSeqConfig1;                          // 0x36     Top level sequencer settings
    uint8_t regSeqConfig2;                          // 0x37     Top level sequencer settings
    uint8_t regTimerResolution;                     // 0x38     Timer 1 and 2 resolution
    uint8_t regTimer1Coef;                          // 0x39     Timer1 settings
    uint8_t regTimer2Coef;                          // 0x3a     Timer2 settings
    uint8_t regImageCalibration;                    // 0x3b     Image calibration engine control
    uint8_t regTemp;                                // 0x3c     Temperature sensor value
    uint8_t regLowBat;                              // 0x3d     Low battery indicator settings
    uint8_t regIrqFlags1;                           // 0x3e     Status register: PLL Lock state, Timeout, RSSI
    uint8_t regIrqFlags2;                           // 0x3f     Status register: FIFO handling flags, Low battery
    uint8_t regDioMapping1;                         // 0x40     Mapping of pins DIO0 to DIO3
    uint8_t regDioMapping2;                         // 0x41     Mapping of pins DIO4 and DIO5, ClkOut frequency
    uint8_t regVersion;                             // 0x42     Semtech ID relating the silicon revision
    uint8_t reserved43;                             // 0x43
    uint8_t regPllHop;                              // 0x44     Control of the fast frequency hopping mode
    uint8_t reserved45;                             // 0x45
    uint8_t reserved46;                             // 0x46
    uint8_t reserved47;                             // 0x47
    uint8_t reserved48;                             // 0x48
    uint8_t reserved49;                             // 0x49
    uint8_t reserved4a;                             // 0x4a
    uint8_t regTcxo;                                // 0x4b     TCXO and XTAL input setting
    uint8_t reserved4c;                             // 0x4c
    uint8_t recPaDac;                               // 0x4d     Higher power settings of the PA
    uint8_t reserved4e;                             // 0x4e
    uint8_t reserved4f;                             // 0x4f
    uint8_t reserved50;                             // 0x50
    uint8_t reserved51;                             // 0x51
    uint8_t reserved52;                             // 0x52
    uint8_t reserved53;                             // 0x53
    uint8_t reserved54;                             // 0x54
    uint8_t reserved55;                             // 0x55
    uint8_t reserved56;                             // 0x56
    uint8_t reserved57;                             // 0x57
    uint8_t reserved58;                             // 0x58
    uint8_t reserved59;                             // 0x59
    uint8_t reserved5a;                             // 0x5a
    uint8_t regFormerTemp;                          // 0x5b     Stored temperature during the former IQ Calibration
    uint8_t reserved5c;                             // 0x5c
    uint8_t regBitRateFrac;                         // 0x5d     Fractional part in the Bit Rate division ratio
    uint8_t reserved5e;                             // 0x5e
    uint8_t reserved5f;                             // 0x5f
    uint8_t reserved60;                             // 0x60
    uint8_t regAgcRef;                              // 0x61
    uint8_t regAgcThreshold1;                       // 0x62
    uint8_t regAgcThreshold2;                       // 0x63
    uint8_t regAgcThreshold3;                       // 0x64
    uint8_t reserved65;                             // 0x65
    uint8_t reserved66;                             // 0x66
    uint8_t reserved67;                             // 0x67
    uint8_t reserved68;                             // 0x68
    uint8_t reserved69;                             // 0x69
    uint8_t reserved6a;                             // 0x6a
    uint8_t reserved6b;                             // 0x6b
    uint8_t reserved6c;                             // 0x6c
    uint8_t reserved6d;                             // 0x6d
    uint8_t reserved6e;                             // 0x6e
    uint8_t reserved6f;                             // 0x6f
    uint8_t regPll;                                 // 0x70     Control of the PLL bandwidth
} sx1276_fsk_registers;

typedef enum
{
    SX1276_FSK_MODE_SLEEP = 0,
    SX1276_FSK_MODE_STANDBY = 1,
    SX1276_FSK_MODE_FSTX = 2,
    SX1276_FSK_MODE_TRANSMIT = 3,
    SX1276_FSK_MODE_FSRX = 4,
    SX1276_FSK_MODE_RECEIVE = 5
} sx1276_fsk_mode_t;

typedef enum
{
    SX1276_BW_2600Hz = 0,
    SX1276_BW_3100Hz = 1,
    SX1276_BW_3900Hz = 2,
    SX1276_BW_5200Hz = 3,
    SX1276_BW_6300Hz = 4,
    SX1276_BW_7800Hz = 5,
    SX1276_BW_10400Hz = 6,
    SX1276_BW_12500Hz = 7,
    SX1276_BW_15600Hz = 8,
    SX1276_BW_20800Hz = 9,
    SX1276_BW_25000Hz = 10,
    SX1276_BW_31300Hz = 11,
    SX1276_BW_41700Hz = 12,
    SX1276_BW_50000Hz = 13,
    SX1276_BW_62500Hz = 14,
    SX1276_BW_83300Hz = 15,
    SX1276_BW_100000Hz = 16,
    SX1276_BW_125000Hz = 17,
    SX1276_BW_166700Hz = 18,
    SX1276_BW_200000Hz = 19,
    SX1276_BW_250000Hz = 20
} sx1276_fsk_bandwidth_t;

typedef enum
{
    SX1276_FSK_OUTPUT_NOFILTER = 0,
    SX1276_FSK_OUTPUT_GFSK_BT1_0 = 1,
    SX1276_FSK_OUTPUT_GFSK_BT0_5 = 2,
    SX1276_FSK_OUTPUT_GFSK_BT0_3 = 3
} sx1276_fsk_output_shape_t;

typedef enum
{
    SX1276_FSK_PACKET_ENCODE_NONE = 0,
    SX1276_FSK_PACKET_ENCODE_MANCHESTER = 1,
    SX1276_FSK_PACKET_ENCODE_WHITENING = 2
} sx1276_fsk_encode_t;


typedef struct
{
    sx1276_SPIControl_t Interface;
    struct
    {
        struct {
            uint32_t frequency;
            uint32_t deviation;
            uint32_t bitrate;
            enum {
                SX1276_FSK_FSK,
                SX1276_FSK_OOK
            } modulation;
            sx1276_fsk_bandwidth_t bandwidth;
            sx1276_fsk_bandwidth_t afc_bandwidth;
            enum
            {
                SX1276_FSK_CONTINUOUS_MODE = 0,
                SX1276_FSK_PACKET_MODE = 1
            } mode;
        } general;
        struct {
            enum {
                SX1276_FSK_OUT_RFO,
                SX1276_FSK_OUT_PA_BOOST
            } output_pin;
            uint8_t maxPower_trim;
            uint8_t outputPower_trim;
            sx1276_fsk_output_shape_t output_shape;
        } transmitter;
        struct
        {
            uint16_t preambleLength;
            uint8_t syncWordSize;
            uint8_t syncWord[8];
            sx1276_fsk_encode_t encode;
            enum
            {
                SX1276_FSK_PACKET_CRC_DISABLE = 0,
                SX1276_FSK_PACKET_CRC_ENABLE = 1
            } crc;
        } packet;
    } settings;
    sx1276_state_t curState;
    sx1276_state_t prevState;
    uint8_t busy;
    uint8_t readyRead;
    uint8_t buffer[256];
    uint8_t packetLength;
    int rssi;
    uint8_t rssiReady;
} sx1276_fsk_t;

void sx1276_fsk_init (sx1276_fsk_t *trc);
int sx1276_fsk_sendPacket (sx1276_fsk_t *trc, uint8_t* buffer, size_t length);
void sx1276_fsk_poll(sx1276_fsk_t *trc);
int sx1276_fsk_startReceive (sx1276_fsk_t *trc);

#endif //DYNAMIC_STAND_BASE_SX1276_FSK_H
