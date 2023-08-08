#ifndef _FIFO_H_
#define _FIFO_H_

typedef struct
{
    volatile uint8_t* const buffer;
    const size_t size;
    volatile uint32_t in;
    volatile uint32_t out;
} fifo_t;

void        fifo_put_byte   (fifo_t* const fifo, const uint8_t x);
void        fifo_put_block  (fifo_t* const fifo, const void* src, size_t len);
int         fifo_get_byte   (fifo_t* const fifo, uint8_t* const dist);
uint32_t    fifo_get_qty    (fifo_t* const fifo);
void        fifo_flush      (fifo_t* const fifo);

#endif /* _FIFO_H_ */
