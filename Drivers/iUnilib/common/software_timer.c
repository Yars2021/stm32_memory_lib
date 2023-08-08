#include "include.h"

/* ============================================================================	*/
/* = Функция запуска таймера													*/
/* ============================================================================	*/
void software_timer_start(timeout_t *timeout, uint32_t time)
{
    timeout->settings.event_time = time;
    timeout->settings.koef_transform = 1.0f;
    timeout->flags.stop = 0;
    timeout->sense.update_end = 1;
    timeout->sense.start = 0;
    timeout->sense.end = 0;
    timeout->flags.one_time_pass = 0;
}

/* ============================================================================	*/
/* = Функция проверки - дотикал ли таймер до конца, или нет						*/
/* ============================================================================	*/
STIME_RESULT software_timer(timeout_t *timeout)
{
    uint32_t temp;

    timeout->sense.start = HAL_GetTick();

    if (timeout->flags.stop)
    {
        timeout->sense.update_end = 1;
        return (STIME_NOT_PASS);
    }

    if (timeout->sense.update_end)
    {
        temp = timeout->sense.start + (timeout->settings.event_time * timeout->settings.koef_transform);
        if ((timeout->settings.width_mask) && (temp > timeout->settings.width_mask)) temp -= timeout->settings.width_mask;
        timeout->sense.end = temp;
        timeout->sense.update_end = 0;
    }

    if (timeout->sense.start >= timeout->sense.end)
    {
        temp = timeout->sense.start + (timeout->settings.event_time * timeout->settings.koef_transform);
        if ((timeout->settings.width_mask) && (temp > timeout->settings.width_mask)) temp -= timeout->settings.width_mask;
        timeout->sense.end = temp;
        return (STIME_PASS);
    }

    return (STIME_NOT_PASS);
}

STIME_RESULT software_timer_elapsed(timeout_t * timeout)
{
    uint32_t temp;
    timeout->sense.start = HAL_GetTick();

    if(timeout->flags.stop)
    {
        return (STIME_NOT_PASS);
    }

    if(timeout->sense.update_end)
    {
        temp = timeout->sense.start + ( timeout->settings.event_time * timeout->settings.koef_transform );
        if ((timeout->settings.width_mask) && (temp > timeout->settings.width_mask)) temp -= timeout->settings.width_mask;
        timeout->sense.end 			= 	temp;
        timeout->sense.update_end	= 0;
    }

    if(timeout->sense.start >= timeout->sense.end)
    {
        if(!timeout->flags.one_time_pass)
        {
            timeout->flags.one_time_pass = 1;

            return (STIME_ONE_PASS);
        }
        return (STIME_PASS);
    }
    else
    {
        return (STIME_NOT_PASS);
    }
}

/* ============================================================================	*/
/* = Функция остановки таймера													*/
/* ============================================================================	*/
void software_timer_stop(timeout_t * timeout)
{
    timeout->flags.stop = 1;
}

/* ============================================================================	*/
/* = Функция приостановки таймера												*/
/* ============================================================================	*/
void software_timer_pause(timeout_t * timeout, uint32_t tics)
{
    uint32_t temp;
    temp = timeout->sense.start + (tics * timeout->settings.koef_transform);
    if ((timeout->settings.width_mask) && (temp > timeout->settings.width_mask)) temp -= timeout->settings.width_mask;
    timeout->sense.end = temp;
}

/* ============================================================================	*/
/* = Функция проверки - остановлен таймер или нет																*/
/* ============================================================================	*/
uint8_t software_timer_stop_test(timeout_t * timeout)
{
    if (timeout->flags.stop)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}
