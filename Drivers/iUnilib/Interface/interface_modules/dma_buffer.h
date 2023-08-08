#ifndef _DMA_BUFFER_H_
#define _DMA_BUFFER_H_

#include "global_macro.h"

typedef struct
{
#if F1_CHECK
    DMA_Channel_TypeDef* const sfr;
#elif F4_CHECK
    DMA_Stream_TypeDef* const sfr;
    uint32_t DMA_Channel;
#endif
    volatile uint8_t* const buffer;
    const size_t size;
    uint_fast16_t index;
} dma_t;

#endif /* _DMA_BUFFER_H */
