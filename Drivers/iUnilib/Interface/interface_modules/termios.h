//
// Created by user on 04.02.2021.
//

#ifndef _TERMIOS_H_
#define _TERMIOS_H_

typedef uint64_t tcflag_t;
typedef int cc_t ;

typedef enum
{
    B50 = 50,
    B75 = 75,
    B110 = 110,
    B134 = 134,
    B150 = 150,
    B200 = 200,
    B300 = 300,
    B600 = 600,
    B1200 = 1200,
    B1800 = 1800,
    B2400 = 2400,
    B4800 = 4800,
    B7200 = 7200,
    B9600 = 9600,
    B14400 = 14400,
    B19200 = 19200,
    B38400 = 38400,
    B56K = 56000,
    B57600 = 57600,
    B115200 = 115200,
    B128K = 128000,
    B256K = 256000
} speed_t;

#define VEOF                    0
#define VEOL                    1
#define VERASE                  2
#define VKILL                   3
#define VINTR                   4
#define VQUIT                   5
#define VSUSP                   6
#define VSTOP                   7
#define VSTART                  8
#define VTIME                   9
#define VMIN                    10

#define CSTOPB	0000100
#define CREAD	0000200
#define PARENB	0000400
#define PARODD	0001000
#define CLOCAL	0004000
#define CCTS_OFLOW      0x00010000      /* CTS flow control of output */
#define CRTS_IFLOW      0x00020000      /* RTS flow control of input */
#define CRTSCTS         (CCTS_OFLOW | CRTS_IFLOW)
#define CDTR_IFLOW      0x00040000      /* DTR flow control of input */
#define CDSR_OFLOW      0x00080000      /* DSR flow control of output */
#define CCAR_OFLOW      0x00100000      /* DCD flow control of output */


// До этого все данные были из стандартной библиотеки для линукса и т.д.
// Никаким регламентом размер структуры termios не ограничен
// Поэтому мы можем расширить массив c_cc настолько, насколько мы этого хотим.
// В общем туда мы и свалим все настройки других интерфейсов, отличных от uart

// ========================== SPI ========================== //

#define V_SPI_MASTER_MODE         11
#define V_SPI_DIRECTION           12
#define V_SPI_MODE                13
#define V_SPI_FIRSTBIT            14
#define V_SPI_BAUD                15
#define V_TTY_DUPLEX              16

#define V_SPI_MASTER_MODE_MASTER                ((int)0)
#define V_SPI_MASTER_MODE_SLAVE                 ((int)1)
#define V_SPI_DIRECTION_2LINES                  ((int)0)
#define V_SPI_DIRECTION_1LINES                  ((int)1)
#define V_SPI_DIRECTION_2LINES_RXONLY           ((int)2)
#define V_SPI_MODE_0                            ((int)0)
#define V_SPI_MODE_1                            ((int)1)
#define V_SPI_MODE_2                            ((int)2)
#define V_SPI_MODE_3                            ((int)3)
#define V_SPI_FIRSTBIT_MSB                      ((int)0)
#define V_SPI_FIRSTBIT_LSB                      ((int)1)
#define V_SPI_PRESCALLER_2                      ((int)0)
#define V_SPI_PRESCALLER_4                      ((int)1)
#define V_SPI_PRESCALLER_8                      ((int)2)
#define V_SPI_PRESCALLER_16                     ((int)3)
#define V_SPI_PRESCALLER_32                     ((int)4)
#define V_SPI_PRESCALLER_64                     ((int)5)
#define V_SPI_PRESCALLER_128                    ((int)6)
#define V_SPI_PRESCALLER_256                    ((int)7)
#define V_TTY_FULLDUPLEX                        ((int)0)
#define V_TTY_HALFDUPLEX                        ((int)1)

// ========================== SPI ========================== //



// ========================== I2C ========================== //

#define V_I2C_CLOCKSPEED          17
#define V_I2C_DUTYCYCLE           18
#define V_I2C_OWNADRESS_1         19
#define V_I2C_OWNADRESS_2         20
#define V_I2C_ADDRESSINGMODE      21
#define V_I2C_DUALADDRESS         22
#define V_I2C_GENERALCALL         23
#define V_I2C_NOSTRETCH           24
#define V_I2C_TIMEOUT             25


#define V_I2C_DUTYCYCLE_2                      ((int)0)
#define V_I2C_DUTYCYCLE_16_9                   ((int)1)
#define V_I2C_ADDRESSINGMODE_7BIT              ((int)0)
#define V_I2C_ADDRESSINGMODE_10BIT             ((int)1)
#define V_I2C_DUALADDRESS_DISABLE              ((int)0)
#define V_I2C_DUALADDRESS_ENABLE               ((int)1)
#define V_I2C_GENERALCALL_DISABLE              ((int)0)
#define V_I2C_GENERALCALL_ENABLE               ((int)1)
#define V_I2C_NOSTRETCH_DISABLE                ((int)0)
#define V_I2C_NOSTRETCH_ENABLE                 ((int)1)

// ========================== I2C ========================== //

// ======================= FSK Modem ======================= //

#define V_FSK_FREQUENCY           26
#define V_FSK_BITRATE             27
#define V_FSK_DEVIATION           28
#define V_FSK_PREAMBLE_LENGTH     29
#define V_FSK_SYNCWORD_SIZE       30
#define V_FSK_SYNCWORD_A          31
#define V_FSK_SYNCWORD_B          32
#define V_FSK_BANDWIDTH           33
#define V_FSK_AFCBANDWIDTH        34
#define V_FSK_OUTPUTSHAPE         35
#define V_FSK_ENCODE              36
#define V_FSK_MAXPOWER_TRIM       37
#define V_FSK_OUTPOWER_TRIM       38

// ======================= FSK Modem ======================= //

// ====================== LORA Modem ======================= //

#define V_LORA_FREQUENCY          39
#define V_LORA_BANDWIDTH          40
#define V_LORA_PREAMBLE_LENGTH    41
#define V_LORA_ERROR_CODE_RATE    42
#define V_LORA_HEADER_TYPE        43
#define V_LORA_SPREADING_FACTOR   44
#define V_LORA_CRC                45
#define V_LORA_MAXPOWER_TRIM      46
#define V_LORA_OUTPOWER_TRIM      47

// ====================== LORA Modem ======================= //


// Этот дефайн - размер массива c_cc
#define NCCS                      48


struct termios
{
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_cc[NCCS];
    speed_t c_ispeed;
    speed_t c_ospeed;
};

#endif /* _TERMIOS_H_ */
