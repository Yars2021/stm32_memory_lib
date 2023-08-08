//
// Created by Dizarker on 21.10.2021.
//
#include <stdint.h>
#include "global_macro.h"

#if F1_CHECK
#include "stm32f1xx_hal.h"
#endif /* F1_CHECK */

//=============================================================================
#if F4_CHECK
#include "stm32f4xx_hal.h"
#endif /* F4_CHECK */

#include "adc_rnd_seed.h"

/*!
 \brief Функция получения случайной 32битной последовательности.
 \details В качестве источника шума выступает последний бит результата преобразования АЦП
 температурного датчика контроллера. В дальнейшем эту последовательность можно поделить по модулю,
 для получения числа внутри заданного диапазона
 \return Возвращает случайную 32битную последовательность
 */
uint32_t adc_rnd_seed (void)
{
    ADC_ChannelConfTypeDef sCh;
    ADC_HandleTypeDef hadc;

    __HAL_RCC_ADC1_CLK_ENABLE();

    hadc.Instance = ADC1;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv =  ADC_SOFTWARE_START;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.NbrOfConversion = 1;
    HAL_ADC_Init(&hadc);

    sCh.Channel = ADC_CHANNEL_TEMPSENSOR;
    sCh.Rank = ADC_REGULAR_RANK_1;
    sCh.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc, &sCh);

    ADC_Enable(&hadc);

    // АЦП настроен, теперь надо взять 32 раза значение температурного датчика
    // И из этого значения взять только младший бит и наполнить этими битами 32 битное число
    // Таким образом получим число состоящее из одного только шума младшего бита АЦП
    uint32_t res = 0;
    uint32_t tmp;

    for (int i = 0; i < 32; i++)
    {
        HAL_ADC_Start(&hadc);
        HAL_ADC_PollForConversion(&hadc,100);
        tmp = HAL_ADC_GetValue(&hadc);

        res |= tmp & 0x01;
        res <<= 1;
    }

    // Разинитим обратно АЦП
    HAL_ADC_DeInit(&hadc);
    __HAL_RCC_ADC1_CLK_DISABLE();

    return res;
}