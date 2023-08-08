#ifndef _SOFTWARE_TIMER_H_
#define _SOFTWARE_TIMER_H_

typedef struct
{
    struct
    {
        uint32_t start;
        uint32_t end;
        uint32_t update_end;
    } sense;

    volatile uint32_t *ticks;

    struct
    {
        float koef_transform;
        float event_time;
        uint32_t width_mask;
    } settings;

    struct {
        uint8_t stop;
        uint8_t one_time_pass;
    } flags;
} timeout_t;

typedef enum
{
    STIME_NOT_PASS              = 0,
    STIME_PASS                  = 1,
    STIME_ONE_PASS              = 2
} STIME_RESULT;

void            software_timer_stop         (timeout_t *timeout);
STIME_RESULT    software_timer_elapsed      (timeout_t *timeout);
uint8_t         software_timer_stop_test    (timeout_t *timeout);
STIME_RESULT    software_timer              (timeout_t *timeout);
void            software_timer_start        (timeout_t *timeout, uint32_t time);
void            software_timer_pause        (timeout_t *timeout, uint32_t tics);

#endif /* _SOFTWARE_TIMER_H_ */
