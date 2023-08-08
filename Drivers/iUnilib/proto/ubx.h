#ifndef _UBX_H_
#define _UBX_H_

#define UBX_HEADER1                           ((uint8_t)0xb5)
#define UBX_HEADER2                           ((uint8_t)0x62)

#define UBX_MSG_CLASS_NAV                     ((uint8_t)0x01)
#define UBX_MSG_CLASS_CFG                     ((uint8_t)0x06)

#define UBX_MSG_NAV_POSECEF                     ((uint8_t)0x01)
#define UBX_MSG_NAV_POSLLH                      ((uint8_t)0x02)
#define UBX_MSG_NAV_STATUS                      ((uint8_t)0x03)
#define UBX_MSG_NAV_DOP                       ((uint8_t)0x04)
#define UBX_MSG_NAV_SOL                       ((uint8_t)0x06)
#define UBX_MSG_NAV_VELECEF                     ((uint8_t)0x11)
#define UBX_MSG_NAV_VELNED                      ((uint8_t)0x12)
#define UBX_MSG_NAV_TIMEGPS                     ((uint8_t)0x20)
#define UBX_MSG_NAV_TIMEUTC                     ((uint8_t)0x21)
#define UBX_MSG_NAV_CLOCK                     ((uint8_t)0x22)
#define UBX_MSG_NAV_SVINFO                      ((uint8_t)0x30)
#define UBX_MSG_NAV_DGPS                      ((uint8_t)0x31)
#define UBX_MSG_NAV_SBAS                      ((uint8_t)0x32)
#define UBX_MSG_NAV_EKFSTATUS                   ((uint8_t)0x40)
#define UBX_MSG_NAV_AOPSTATUS                   ((uint8_t)0x60)
#define UBX_MSG_NAV_STATUS                      ((uint8_t)0x03)
#define UBX_MSG_NAV_RELPOS                      ((uint8_t)0x3c)

#define UBX_MSG_CFG_PRT                       ((uint8_t)0x00)
#define UBX_MSG_CFG_MSG                       ((uint8_t)0x01)
#define UBX_MSG_CFG_INF                       ((uint8_t)0x02)
#define UBX_MSG_CFG_RST                       ((uint8_t)0x04)
#define UBX_MSG_CFG_DAT                       ((uint8_t)0x06)
#define UBX_MSG_CFG_TP                        ((uint8_t)0x07)
#define UBX_MSG_CFG_RATE                      ((uint8_t)0x08)
#define UBX_MSG_CFG_CFG                       ((uint8_t)0x09)
#define UBX_MSG_CFG_FXN                       ((uint8_t)0x0e)
#define UBX_MSG_CFG_RXM                       ((uint8_t)0x11)
#define UBX_MSG_CFG_EKF                       ((uint8_t)0x12)
#define UBX_MSG_CFG_ANT                       ((uint8_t)0x13)
#define UBX_MSG_CFG_SBAS                      ((uint8_t)0x16)
#define UBX_MSG_CFG_NMEA                      ((uint8_t)0x17)
#define UBX_MSG_CFG_USB                       ((uint8_t)0x1b)
#define UBX_MSG_CFG_TMODE                     ((uint8_t)0x1d)
#define UBX_MSG_CFG_NVS                       ((uint8_t)0x22)
#define UBX_MSG_CFG_NAVX5                     ((uint8_t)0x23)
#define UBX_MSG_CFG_NAV5                      ((uint8_t)0x24)
#define UBX_MSG_CFG_ESFGWT                    ((uint8_t)0x29)
#define UBX_MSG_CFG_TP5                       ((uint8_t)0x31)
#define UBX_MSG_CFG_PM                        ((uint8_t)0x32)
#define UBX_MSG_CFG_RINV                      ((uint8_t)0x34)
#define UBX_MSG_CFG_ITFM                      ((uint8_t)0x39)
#define UBX_MSG_CFG_PM2                       ((uint8_t)0x3b)
#define UBX_MSG_CFG_TMODE2                    ((uint8_t)0x3d)

#define UBX_MSGLEN_CFG_PRT                    20
#define UBX_MSGLEN_CFG_MSG                    8
#define UBX_MSGLEN_CFG_RATE                   6

#define UBX_PORT_USART1                       ((uint8_t)0x01)
#define UBX_PORT_USART2                       ((uint8_t)0x02)
#define UBX_PORT_MASK_STD                     ((uint16_t)0x08d0)
#define UBX_PORT_PROTO_MASK_UBX               ((uint16_t)0x0001)
#define UBX_PORT_PROTO_MASK_NMEA              ((uint16_t)0x0002)
#define UBX_PORT_PROTO_MASK_BOTH              ((uint16_t)0x0003)

#define UBX_CRCCALC_HEADERS_SIZE              4
#define UBX_SEND_HEADERS_SIZE                 8


typedef enum
{
  NO_FIX = 0,
  DEAD_RECKONING = 1,
  FIX_2D = 2,
  FIX_3D = 3,
  GPS_AND_DR = 4,
  TIME_ONLY = 5
} ubx_gpsFix_mode_t;

#pragma pack (push, 1)
typedef struct
{
  uint8_t ubxHeader1;
  uint8_t ubxHeader2;
  uint8_t messageClass;
  uint8_t messageID;
  uint16_t messageLength;
  uint8_t data[];
} ubx_msg_t;

typedef struct
{
  uint8_t ckA;
  uint8_t ckB;
} ubx_ChkSumm_t;

typedef struct 
{
  uint8_t portID;
  uint8_t reserved1;
  uint16_t reserved2;
  uint32_t mode;
  uint32_t speed;
  uint16_t inProto;
  uint16_t outProto;
  uint16_t reserved3;
  uint16_t reserved4;
  ubx_ChkSumm_t ckSumm;
} ubx_msg_portconfig_t;

typedef struct
{
  uint8_t messageClass;
  uint8_t messageID;
  uint8_t rate[6];
  ubx_ChkSumm_t ckSumm;
} ubx_msg_msgconfig_t;

typedef struct
{
  uint16_t measureRate;
  uint16_t navRate;
  uint16_t timeRef;
  ubx_ChkSumm_t ckSumm;
} ubx_msg_rateconfig_t;

/* Тут будут структуры пакетов, которые будут забиты в главную структуру модуля, т.е. будут обновляться все данные, которые принимает модуль. */
/* А на основании некоторых будут получены обобщенные данные                                                                                  */
typedef struct 
{
  uint32_t week_mseconds;                     // Количество милисекунд с начала недели. Неделя начинается в воскресенье
  int32_t round_nanoseconds;                  // Я того хер знает, как он с такой точностью может вычислять время, но это число наносекунд, на которую было сделано округление
  int16_t GPSweek;                            // Номер недели с начала эпохи GPS (06 января 1980 года 0 часов 0 минут 0 секунд)
  ubx_gpsFix_mode_t gpsFixType;                         // Тип фиксации GPS (0 - No fix, 1 - Dead reckoning, 2 - 2D fix, 3 - 3D fix, 4 - 3D + Dead reckoning, 5 - Time only fix)
  uint8_t fixFlags;                           // Координаты в системе ECEF, 1 LSB = 1 см
  int32_t ecef_X;
  int32_t ecef_Y;
  int32_t ecef_Z;
  uint32_t position_accuracy;                 // Точность позиционирования, 1 LSB = 1 см
  int32_t vecef_X;                            // Скорость в системе ECEF, 1 LSB = 1 см/с
  int32_t vecef_Y;  
  int32_t vecef_Z;
  uint32_t speed_accuracy;                    // Точность определения скорости, 1 LSB = 1 см/с
  uint16_t pDOP;
  uint8_t reserved1;
  uint8_t numSV;                              // Число спутников, используемых в решении
  uint32_t reserved2;
} ubx_nav_sol_t;

typedef struct
{
  uint32_t week_mseconds;                     // Количество милисекунд с начала недели. Неделя начинается в воскресенье
  uint32_t time_accuracy;                     // Точность измерения времени (в наносекундах)
  int32_t nanosec;                            // Поправка в наносекундах
  uint16_t year;                              // Год
  uint8_t month;                              // Месяц
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t TimeOfWeekValid:1;
  uint8_t WeekNumberValid:1;
  uint8_t timeValid:1;
  uint8_t reserved:5;
} ubx_nav_timeutc_t;

typedef struct
{
  uint32_t week_mseconds;                     // Количество милисекунд с начала недели. Неделя начинается в воскресенье
  int32_t longitude;                          // Широта в градусах. 1 LSB = 10-7 deg
  int32_t lattitude;                          // Долгота в градусах. 1 LSB = 10-7 deg
  int32_t heightAboveEllipsoid;               // Высота в мм над эллипсоидом
  int32_t heightAboveSeaLevel;                // Высота в мм над уровнем моря
  uint32_t horizontal_accuracy;               // Точность горизонтальная (1 LSB = 1 мм)
  uint32_t vertical_accuracy;                 // Точность вертикальная (1 LSB = 1 мм)
} ubx_nav_posllh_t;

typedef struct
{
  uint32_t week_mseconds;                     // Количество милисекунд с начала недели. Неделя начинается в воскресенье
  int32_t velN;                               // Компонент скорости на север, см/с
  int32_t velE;                               // Компонент скорости на восток, см/с
  int32_t velD;                               // Компонент скорости вниз, см/с
  uint32_t speed3d;                           // Модуль полнойс скорости (3D)
  uint32_t speed2d;                           // Модуль скорости относительно земли (2D)
  int32_t heading;                            // Азимут, 1 LSB = 10-5 deg
  uint32_t speed_accuracy;                    // Точность определения скорости, 1 LSB = 1 см/с
  uint32_t heading_accuracy;                  // Точность определения направления, 1 LSB = 10-5 deg
} ubx_nav_velned_t;

typedef struct
{
  uint32_t week_msseconds;                    // Количество милисекунд с начала недели. Неделя начинается в воскресенье
  uint8_t fixType;                        // Тип фиксации GPS
  uint8_t fixFlags;                       // Дополнительные флаги режима работы GPS
  uint8_t fixStat;                        // Еще больше подробностей о режиме работы GPS
  uint8_t fixFlags2;                        // Вообще много всего о режиме работы модуля
  uint32_t timeToFirstFix_ms;                   // Время до фиксации с момента старта
  uint32_t uptime_ms;                       // Время работы модуля с подачи питания или последнего сброса
} ubx_nav_status_t;

typedef struct
{
  uint8_t version;                        // Версия протокола 0
  uint8_t reserved1;                        // Заборонено 1
  uint16_t refStationID;                      // ID станции поправок  2
  uint32_t week_msseconds;                    // Количество милисекунд с начала недели. Неделя начинается в воскресенье 4
  int32_t relPosN_cm;                       // Расстояние до базы, проекция на север, см  8
  int32_t relPosE_cm;                       // Расстояние до базы, проекция на восток, см 12
  int32_t relPosD_cm;                       // Расстояние до базы, проекция вниз, см  16
  int8_t relPosHighPrecesionN_mm;                 // Расстояние до базы, проекция на север, 0.1 милиметра 20
  int8_t relPosHighPrecesionE_mm;                 // Расстояние до базы, проекция на восток, 0.1 милиметра  21
  int8_t relPosHighPrecesionD_mm;                 // Расстояние до базы, проекция вниз, 0.1 милиметра   22
  uint8_t reserved2;                        // Заборонено 2 23
  uint32_t accN_mm;                       // Точность измерения, по оси на север, 0.1 мм  24
  uint32_t accE_mm;                       // Точность измерения, по оси на восток, 0.1 мм   28
  uint32_t accD_mm;                       // Точность измерения, по оси вниз, 0.1 мм  32
  uint32_t gnssFixOk:1;                     // Флаг наличия фиксации
  uint32_t diffSolution:1;                    // Флаг успешного применения дифференциальных поправок
  uint32_t relPosValid:1;                     // Флаг валидности дифференциальных данных (для режима Moving Base)
  uint32_t carrierSolutionState:2;                // Флаг состояния решения диапазона фаз несущей частоты (0 - нет решения, 1 - решение с плавающей неоднозначностью, 2 - решение с фиксированной неоднозначностью)
  uint32_t isMoving:1;                        // Флаг режима MovingBase
  uint32_t refPosMissing:1;                   // 1 if extrapolated reference position was used to compute moving base solution this epoch
  uint32_t refObsMissing:1;                   // 1 if extrapolated reference observation were used to compute moving base solution this epoch
  uint32_t relPosHeadingValid:1;                  // Флаг валидности вычисления азимута на базу 
} ubx_nav_relposned_v0_t;

/* Полные компоненты расчитывается по формулам:
  relPosN_cm + relPosHighPrecesionN_mm * 1e-2
  relPosE_cm + relPosHighPrecesionE_mm * 1e-2
  relPosD_cm + relPosHighPrecesionD_mm * 1e-2
*/

typedef struct
{
  uint8_t version;                        // Версия протокола 0
  uint8_t reserved1;                        // Заборонено 1
  uint16_t refStationID;                      // ID станции поправок  2
  uint32_t week_msseconds;                    // Количество милисекунд с начала недели. Неделя начинается в воскресенье 4
  int32_t relPosN_cm;                       // Расстояние до базы, проекция на север, см  8
  int32_t relPosE_cm;                       // Расстояние до базы, проекция на восток, см 12
  int32_t relPosD_cm;                       // Расстояние до базы, проекция вниз, см  16
  int32_t relPosLength_cm;                    // Расстояние до базы (я так понимаю модуль, только не понятно, зачем тип int указан в протоколе) 20
  int32_t relPosHeading_deg;                    // Азимут на базу. 1e-5 град  24
  uint8_t reserved2[4];                     // Заборонено 2   28
  int8_t relPosHighPrecesionN_mm;                 // Расстояние до базы, проекция на север, 0.1 милиметра   32
  int8_t relPosHighPrecesionE_mm;                 // Расстояние до базы, проекция на восток, 0.1 милиметра  33
  int8_t relPosHighPrecesionD_mm;                 // Расстояние до базы, проекция вниз, 0.1 милиметра   34
  int8_t relPosHighPrecesionLength_mm;              // Расстояние до базы, модуль, 0.1 милиметра  35
  uint32_t accN_mm;                       // Точность измерения, по оси на север, 0.1 мм  36
  uint32_t accE_mm;                       // Точность измерения, по оси на восток, 0.1 мм   40
  uint32_t accD_mm;                       // Точность измерения, по оси вниз, 0.1 мм  44
  uint32_t accLenght_mm;                      // Точность измерения расстояния, 0.1 мм  48
  uint32_t accHeading_deg;                    // Точность измерения азимута, 1e-5 град  52
  uint8_t reserved3[4];                     // Заборонено 3   56
  union {
      uint32_t status_full;
      struct {
          uint32_t gnssFixOk: 1;                     // Флаг наличия фиксации
          uint32_t diffSolution: 1;                    // Флаг успешного применения дифференциальных поправок
          uint32_t relPosValid: 1;                     // Флаг валидности дифференциальных данных (для режима Moving Base)
          uint32_t carrierSolutionState: 2;                // Флаг состояния решения диапазона фаз несущей частоты (0 - нет решения, 1 - решение с плавающей неоднозначностью, 2 - решение с фиксированной неоднозначностью)
          uint32_t isMoving: 1;                        // Флаг режима MovingBase
          uint32_t refPosMissing: 1;                   // 1 if extrapolated reference position was used to compute moving base solution this epoch
          uint32_t refObsMissing: 1;                   // 1 if extrapolated reference observation were used to compute moving base solution this epoch
          uint32_t relPosHeadingValid: 1;                  // Флаг валидности вычисления азимута на базу
      } status_bits;
  } status;
} ubx_nav_relposned_v1_t;

#pragma pack (pop)

typedef struct 
{
  uint8_t buffer[256];
  uint8_t *pnt;
  uint8_t state;
  uint16_t length;
  int interface_fd;
} ubx_receiver_t;

typedef struct
{
  ubx_nav_sol_t sol;
  ubx_nav_timeutc_t timeutc;
  ubx_nav_posllh_t posllh;
  ubx_nav_velned_t ned;
  ubx_nav_status_t status;
  union 
  {
    ubx_nav_relposned_v0_t relpos_v0;
    ubx_nav_relposned_v1_t relpos_v1;
  } relpos;
} ubx_rawData_t;

typedef struct
{
  double lattitude;
  double longitude;
  double height;
  uint8_t day;
  uint8_t month;
  uint16_t year;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint32_t timestamp;
  uint8_t numSv;
  float speed2d;
  float speed3d;
  float azimuth;
  ubx_gpsFix_mode_t fixMode;
} ubx_gps_data_t;

typedef struct
{
    ubx_receiver_t receiver;  
    ubx_rawData_t rawData;
    ubx_gps_data_t aggregatedData;
    uint8_t updateAt;
    uint8_t dataUpdated;
} ubx_t;

void ubx_init (ubx_t *ubx, int interface_fd);
void ubx_task (ubx_t *ubx);
void ubxProcessPacket(ubx_t *ubx);

#endif /* _UBX_H_ */
