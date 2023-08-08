#include "include.h"

static void ubxProcessByte(ubx_t *ubx, uint8_t ch);
static ubx_ChkSumm_t ubxCrcCalc (uint8_t *data, uint16_t length);

static inline void ubxResetState(ubx_t *ubx)
{
  ubx->receiver.pnt = ubx->receiver.buffer;
  ubx->receiver.state = 0;
}
  
static inline void ubxStateUp(ubx_t *ubx)
{
  ubx->receiver.pnt++;
  ubx->receiver.state++;
}

static void ubxProcessByte(ubx_t *ubx, uint8_t ch)
{
  *ubx->receiver.pnt = ch;
  
  switch (ubx->receiver.state)
  {
    case 0:
      // Ждем первый старт байт
      if (ch == UBX_HEADER1) ubxStateUp(ubx);
      break;
    
    case 1:
      // Второй старт байт
      if (ch == UBX_HEADER2) ubxStateUp(ubx);
      else ubxResetState(ubx);
      break;
    
    case 2:
      // Класс сообщения. Интересует только NAV сообщения
      if (ch == UBX_MSG_CLASS_NAV)
      ubxStateUp(ubx);
      else ubxResetState (ubx);
      break;
    
    case 3:
      // Тип сообщения. Примем все
      ubxStateUp(ubx);
      break;
    
    case 4:
      // Младшая часть длины посылки
      ubx->receiver.length = ch;
      ubxStateUp(ubx);
      break;
    
    case 5:
      // Старшая часть посылки
      ubx->receiver.length |= (ch << 8);
      ubxStateUp(ubx);
      break;
    
    case 6:
      // Принимаем тело сообщения
      ubx->receiver.length--;
      if (!(ubx->receiver.length)) ubxStateUp(ubx);
      else ubx->receiver.pnt++;
      break;
    
    case 7:
      // Первый байт контрольнной суммы
      ubxStateUp(ubx);
      break;
    
    case 8:
      // Второй байт контрольной суммы. Сообщение принято
      ubxProcessPacket(ubx);
      ubxResetState(ubx);
      break;
    
    default: 
      // Тут мы вообще оказаться не должны, но если вдруг - сбросим автомат
      ubxResetState(ubx);
      break;
  }
}

static ubx_ChkSumm_t ubxCrcCalc (uint8_t *data, uint16_t length)
{
  ubx_ChkSumm_t temp = {0, 0};
  
  for (int i = 0; i < length; i++)
  {
    temp.ckA = temp.ckA + *(data + i);
    temp.ckB = temp.ckB + temp.ckA;
  }
  
  return temp;
}

void ubxProcessPacket(ubx_t *ubx)
{
  ubx_msg_t *msg = (ubx_msg_t*)ubx->receiver.buffer;
  ubx_ChkSumm_t ckSum;

  // Для начала проверим CRC
  ckSum = ubxCrcCalc ((uint8_t*)&msg->messageClass, msg->messageLength + UBX_CRCCALC_HEADERS_SIZE);
  if ((ckSum.ckA != *(uint8_t*)(&msg->messageClass + msg->messageLength + UBX_CRCCALC_HEADERS_SIZE)) || (ckSum.ckB != *(uint8_t*)(&msg->messageClass + msg->messageLength + UBX_CRCCALC_HEADERS_SIZE + 1))) return;
  // CRC сошлось. Теперь распарсим данные

  switch (msg->messageID)
  {
    case UBX_MSG_NAV_SOL:
      memcpy ((void*)&ubx->rawData.sol, msg->data, sizeof (ubx_nav_sol_t));
      // Тут возьмем только количество спутников и тип фиксации
      ubx->aggregatedData.fixMode = ubx->rawData.sol.gpsFixType;
      ubx->aggregatedData.numSv = ubx->rawData.sol.numSV;

      if (ubx->updateAt == UBX_MSG_NAV_SOL) ubx->dataUpdated = 1;
      break;
    
    case UBX_MSG_NAV_POSLLH:
      memcpy ((void*)&ubx->rawData.posllh, msg->data, sizeof (ubx_nav_posllh_t));
      // Здесь нам нужны координаты
      ubx->aggregatedData.lattitude = (double)ubx->rawData.posllh.lattitude / 10000000;
      ubx->aggregatedData.longitude = (double)ubx->rawData.posllh.longitude / 10000000;
      ubx->aggregatedData.height = (double)ubx->rawData.posllh.heightAboveSeaLevel / 1000;

      if (ubx->updateAt == UBX_MSG_NAV_POSLLH) ubx->dataUpdated = 1;
      break;
    
    case UBX_MSG_NAV_VELNED:
      memcpy ((void*)&ubx->rawData.ned, msg->data, sizeof (ubx_nav_velned_t));
      // Скорость
      ubx->aggregatedData.speed2d = (float)ubx->rawData.ned.speed2d / 100;
      ubx->aggregatedData.speed3d = (float)ubx->rawData.ned.speed3d / 100;
      ubx->aggregatedData.azimuth = (float)ubx->rawData.ned.heading / 100000;

      if (ubx->updateAt == UBX_MSG_NAV_VELNED) ubx->dataUpdated = 1;
      break;
    
    case UBX_MSG_NAV_TIMEUTC:
      memcpy ((void*)&ubx->rawData.timeutc, msg->data, sizeof (ubx_nav_timeutc_t));
      // Время
      ubx->aggregatedData.year = ubx->rawData.timeutc.year;
      ubx->aggregatedData.month = ubx->rawData.timeutc.month;
      ubx->aggregatedData.day = ubx->rawData.timeutc.day;
      ubx->aggregatedData.hour = ubx->rawData.timeutc.hour;
      ubx->aggregatedData.min = ubx->rawData.timeutc.min;
      ubx->aggregatedData.sec = ubx->rawData.timeutc.sec;

      if (ubx->updateAt == UBX_MSG_NAV_TIMEUTC) ubx->dataUpdated = 1;
      break;

    case UBX_MSG_NAV_STATUS:
      memcpy ((void*)&ubx->rawData.status, msg->data, sizeof (ubx_nav_status_t));

      if (ubx->updateAt == UBX_MSG_NAV_STATUS) ubx->dataUpdated = 1;
      break;

    case UBX_MSG_NAV_RELPOS:
      if (msg->data[0] == 0x00)
        memcpy ((void*)&ubx->rawData.relpos, msg->data, sizeof (ubx_nav_relposned_v0_t));
      else if (msg->data[0] == 0x01)
        memcpy ((void*)&ubx->rawData.relpos, msg->data, sizeof (ubx_nav_relposned_v1_t));

      if (ubx->updateAt == UBX_MSG_NAV_RELPOS) ubx->dataUpdated = 1;
      break;

    default:
      break;
  }  
}

void ubx_init(ubx_t *ubx, int interface_fd)
{
  ubx->receiver.pnt = ubx->receiver.buffer;
  ubx->receiver.state = 0;
  ubx->dataUpdated = 0;
  ubx->receiver.interface_fd = interface_fd;
  ubx->updateAt = UBX_MSG_NAV_SOL;
}

void ubx_task (ubx_t *ubx)
{
  char ch;
  int res;

  for (;;)
  {
    res = read(ubx->receiver.interface_fd, &ch, 1);
    if (res == 0) break;

    ubxProcessByte(ubx, (uint8_t)ch);
  }
}