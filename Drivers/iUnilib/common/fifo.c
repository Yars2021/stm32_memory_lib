#include "include.h"

#define FIFO_IDX(FIFO,IN_OUT) (FIFO->IN_OUT & (FIFO->size-1))

//=====================================================================================================================
void fifo_put_byte (fifo_t* const fifo, const uint8_t x)
{
   ENTER_CRITICAL_SECTION();
   {
       fifo->buffer[FIFO_IDX(fifo,in)] = x;
       fifo->in++;
   }
   LEAVE_CRITICAL_SECTION();
}

//=====================================================================================================================
void fifo_put_block (fifo_t* const fifo, const void* src, size_t len)
{
    const uint8_t* p = src;

    while (len--)
    {
        fifo_put_byte(fifo, *p++);
    }
}

//=====================================================================================================================
int fifo_get_byte (fifo_t* const fifo, uint8_t* const dst)
{
    uint32_t qty;

    ENTER_CRITICAL_SECTION();
    {
        qty = fifo->in - fifo->out;

        if (qty)
        {
            *dst = fifo->buffer[FIFO_IDX(fifo, out)];
            fifo->out++;
        }
    }
    LEAVE_CRITICAL_SECTION();

    return (qty != 0);
}

//=====================================================================================================================
uint32_t fifo_get_qty (fifo_t* const fifo)
{
    return (fifo->in - fifo->out);
}

//=====================================================================================================================
void fifo_flush (fifo_t* const fifo)
{
    ENTER_CRITICAL_SECTION();
    {
        fifo->in = 0;
        fifo->out = 0;
    }
    LEAVE_CRITICAL_SECTION();
}
