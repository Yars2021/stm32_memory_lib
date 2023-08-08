//
// Created by Dizarker on 20.10.2021.
//

#ifndef _LED_H_
#define _LED_H_

#include <stdlib.h>
#include <stdint.h>

#include "stm32f1xx_hal.h"
#include "software_timer.h"

typedef struct
{
    GPIO_TypeDef *PORT;
    uint32_t pin;
    timeout_t timer;
    int value;
    int zero_time;
    int one_time;
} led_t;

void led_init (led_t *led, GPIO_TypeDef *PORT, uint32_t pin);
void led_set (led_t *led);
void led_clr (led_t *led);
void led_toggle (led_t *led);
void led_blink (led_t *led, int zero_time, int one_time, int init_state);
void led_task (led_t *led);
void led_one_pulse (led_t *led, int pulse_time, int init_state);

#endif // _LED_H_
