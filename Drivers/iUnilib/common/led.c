//
// Created by Dizarker on 20.10.2021.
//

#include "led.h"

/*!
 * \brief Функция инициализации светодиода
 * \param led_t *led - указатель на экземпляр светодиода
 * \param GPIO_TypeDef *GPIO - указатель на структуру GPIO (хоть HAL, хоть SPL)
 * \param uint32_t pin - пин, который соответствует светодиоду
 * \example led_init(&myLed, GPIOB, GPIO_PIN_3);
 */
void led_init (led_t *led, GPIO_TypeDef *PORT, uint32_t pin)
{
    led->PORT = PORT;
    led->pin = pin;
    led->value = PORT->ODR & pin;
    software_timer_stop(&led->timer);
}

/*!
 * \brief Функция установки светодиода в 1
 * \param led_t *led - указатель на экземпляр светодиода
 */
void led_set (led_t *led)
{
    led->PORT->ODR |= led->pin;
    led->value = 1;
    software_timer_stop(&led->timer);
}

/*!
 * \brief Функция установки светодиода в 0
 * \param led_t *led - указатель на экземпляр светодиода
 */
void led_clr (led_t *led)
{
    led->PORT->ODR &=~ led->pin;
    led->value = 0;
    software_timer_stop(&led->timer);
}

/*!
 * \brief Функция инвертирования состояния светодиода
 * \param led_t *led - указатель на экземпляр светодиода
 */
void led_toggle (led_t *led)
{
    led->PORT->ODR ^= led->pin;
    led->value ^= 1;
    software_timer_stop(&led->timer);
}

/*!
 * \brief Функция установки режима моргания светодиода
 * \param led_t *led - указатель на экземпляр светодиода
 * \param int zero_time - время в мс, в течении которого состояние пина будет 0
 * \param int one_time - время в мс, в течении которого состояние пина будет 1
 * \param init_state - начальное состояние пина (0 - изначально на выходе пина 0, любое число - на выходе будет 1)
 * \example led_blink(&myLed, 100, 150, 1); // моргаем с частотой 4 Гц, 150 мс светим, 100 мс ждем. Начальное состояние - светим
 */
void led_blink (led_t *led, int zero_time, int one_time, int init_state)
{
    led->one_time = one_time;
    led->zero_time = zero_time;

    if (init_state == 0)
    {
        if (led->value == 1)
        {
            led_clr(led);
        }
        software_timer_start(&led->timer, zero_time);
    }
    else
    {
        if (led->value == 0)
        {
            led_set(led);
        }
        software_timer_start(&led->timer, one_time);
    }
}


/*!
 * \brief Функция однократного импульса на светодиод
 * \param led_t *led - указатель на экземпляр светодиода
 * \param int pulse_time - время в мс, длительность импульса
 * \param state - состояние пина, на время импульса (0 - на длительность импульса на выходе будет 0, после - 1; 1 - наоборот)
 * \example led_one_pulse(&myLed, 500, 0); // на пол секунды светодиод погаснет ;)
 */
void led_one_pulse (led_t *led, int pulse_time, int state)
{
    led->one_time = 0;
    led->zero_time = 0;

    if (state == 0)
    {
        led_clr(led);
    }
    else
    {
        led_set(led);
    }

    software_timer_start(&led->timer, pulse_time);
}


/*!
 * \brief Функция периодического вызова для обновления состояние светодиодов
 * \param led_t *led - указатель на экземпляр светодиода
 */
void led_task(led_t *led)
{
    if (software_timer(&led->timer))
    {
        if (led->value == 0)
        {
            led_set(led);
            if (led->one_time)
                software_timer_start(&led->timer, led->one_time);
        }
        else
        {
            led_clr(led);
            if (led->zero_time)
                software_timer_start(&led->timer, led->zero_time);
        }
    }
}
