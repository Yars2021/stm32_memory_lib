#ifndef _UART_DMA_H_
#define _UART_DMA_H_

typedef struct
{
    USART_TypeDef *const sfr;
    UART_HandleTypeDef *const handler;
    struct
    {
        fifo_t rx;
        fifo_t tx;
    } buffers;
} uart_t;

extern uart_t uart1;
extern uart_t uart2;
extern uart_t uart3;
extern uart_t uart4;
extern uart_t uart5;

void    uart_init           (uart_t* const uart, struct termios *opt);
void    uart_putc           (uart_t* const uart, char c);
int     uart_getc           (uart_t* const uart);
void    uart_flush          (uart_t* const uart);
void    uart_put_block      (uart_t* const uart, const void* src, size_t len);

#endif /* _UART_DMA_H_ */
