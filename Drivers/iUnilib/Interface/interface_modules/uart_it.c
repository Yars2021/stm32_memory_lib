#include "../interface.h"

// ========================================================================================================
static INLINE void uart_set_tx_int (uart_t* const uart, int x)
{
    if (x)
    {
        if (uart->sfr->CR3 & USART_CR3_HDSEL)
        {
            uart->sfr->CR1 &=~ USART_CR1_RE;
        }
        uart->sfr->CR1 |= USART_CR1_TXEIE;
    }
    else
    {
        uart->sfr->CR1 &=~ USART_CR1_TXEIE;
        if (uart->sfr->CR3 & USART_CR3_HDSEL)
        {
            uart->sfr->CR1 |= USART_CR1_TCIE;
        }
    }
}

// ========================================================================================================
static INLINE void uart_rx_isr (uart_t* const uart, uint_fast16_t status)
{
#if F3_CHECK
    static const uint_fast16_t ERR_MASK = USART_ISR_ORE | USART_ISR_NE | USART_ISR_FE | USART_ISR_PE;
    uint8_t data = uart->sfr->RDR;
#else
    static const uint_fast16_t ERR_MASK = USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE;
    uint8_t data = uart->sfr->DR;
#endif
    if ((status & ERR_MASK) == 0)
    {
        fifo_put_byte(&uart->buffers.rx, data);
    }
}

// ========================================================================================================
static INLINE void uart_tx_isr (uart_t* const uart)
{
#if F3_CHECK
    uint_fast16_t  status = uart->sfr->ISR;
    if (status & USART_ISR_TXE);
    {
        if (!fifo_get_byte(&uart->buffers.tx, (uint8_t*)&uart->sfr->TDR))
        {
            uart_set_tx_int(uart, 0);
        }
    }
#else
    uint_fast16_t  status = uart->sfr->SR;
    if (status & USART_SR_TXE)
    {
        if (!fifo_get_byte(&uart->buffers.tx, (uint8_t*)&uart->sfr->DR))
        {
            uart_set_tx_int(uart, 0);
        }
    }
#endif
}

// ========================================================================================================
static INLINE void uart_isr (uart_t* const uart)
{
#if F3_CHECK
    uint_fast16_t status = uart->sfr->ISR;

    if (status & USART_ISR_RXNE)
    {
        uart_rx_isr (uart, status);
    }
    else if (status & USART_ISR_TXE)
    {
        uart_tx_isr(uart);
    }

    if ((uart->sfr->CR3 & USART_CR3_HDSEL) && (status & USART_ISR_TC))
    {
        uart->sfr->CR1 &=~ USART_CR1_TCIE;
        uart->sfr->CR1 |= USART_CR1_RE;
    }

    if( (status & USART_ISR_FE) || (status & USART_ISR_NE) )
    {
        uart->sfr->ICR |= USART_ICR_FECF;
        uart->sfr->ICR |= USART_ICR_NCF;
    }

#else
    uint_fast16_t status = uart->sfr->SR;

    if (status & USART_SR_RXNE)
    {
        uart_rx_isr (uart, status);
    }
    else if (status & USART_SR_TC)
    {
        uart->sfr->SR &=~ USART_SR_TC;
        uart->sfr->CR1 |= USART_CR1_RE;
    }
    else if (status & USART_SR_TXE)
    {
        uart_tx_isr(uart);
    }
#endif
}

// ========================================================================================================
void uart_putc (uart_t* const uart, char c)
{
    fifo_put_byte(&uart->buffers.tx, c);
    uart_set_tx_int(uart, 1);
}

// ========================================================================================================
void uart_put_block (uart_t* const uart, const void* src, size_t len)
{
    if (len)
    {
        fifo_put_block(&uart->buffers.tx, src, len);
        uart_set_tx_int(uart, 1);
    }
}

// ========================================================================================================
int uart_getc (uart_t* const uart)
{
    uint8_t x;
    if (fifo_get_byte(&uart->buffers.rx, &x))
    {
        return (x);
    }
    return (-1);
}

// ========================================================================================================
void uart_flush (uart_t* const uart)
{
    fifo_flush(&uart->buffers.tx);
    fifo_flush(&uart->buffers.rx);
}

// ========================================================================================================
static void uart_init_rcc(uart_t* const uart)
{
    switch ((uint32_t)uart->sfr) {
        case USART1_BASE:
            __HAL_RCC_USART1_CLK_ENABLE();
            break;
        case USART2_BASE:
            __HAL_RCC_USART2_CLK_ENABLE();
            break;
        case USART3_BASE:
            __HAL_RCC_USART3_CLK_ENABLE();
            break;
#ifdef UART4
        case UART4_BASE:
            __HAL_RCC_UART4_CLK_ENABLE();
            break;
#endif /* UART4 */
#ifdef UART5
        case UART5_BASE:
            __HAL_RCC_UART5_CLK_ENABLE();
            break;
#endif /* UART5 */
        default: break;
    }

#if F1_CHECK
    __HAL_RCC_AFIO_CLK_ENABLE();
#endif /* F1_CHECK */

}

// ========================================================================================================
static void uart_init_nvic (uart_t* const uart)
{
    switch ((uint32_t)uart->sfr)
    {
        case USART1_BASE: NVIC_EnableIRQ(USART1_IRQn); break;
        case USART2_BASE: NVIC_EnableIRQ(USART2_IRQn); break;
        case USART3_BASE: NVIC_EnableIRQ(USART3_IRQn); break;
#ifdef UART4
        case UART4_BASE: NVIC_EnableIRQ(UART4_IRQn); break;
#endif /* UART4 */
#ifdef UART5
        case UART5_BASE: NVIC_EnableIRQ(UART5_IRQn); break;
#endif /* UART5 */
        default: break;
    }
}

// ========================================================================================================
void uart_init (uart_t* const uart, struct termios *opt)
{
    uart->handler->Instance = uart->sfr;
    HAL_UART_DeInit(uart->handler);
    uart_init_rcc(uart);
    uart_flush(uart);

    uart->handler->Instance = uart->sfr;
    uart->handler->Init.BaudRate = opt->c_ispeed;
    uart->handler->Init.WordLength = UART_WORDLENGTH_8B;

    // Четность
    if (opt->c_cflag & PARENB)
    {
        if (opt->c_cflag & PARODD) uart->handler->Init.Parity = UART_PARITY_ODD;
        else uart->handler->Init.Parity = UART_PARITY_EVEN;
    }
    else
    {
        uart->handler->Init.Parity = UART_PARITY_NONE;
    }

    // Количество стоп битов
    if (opt->c_cflag & CSTOPB)
    {
        uart->handler->Init.StopBits = UART_STOPBITS_2;
    }
    else
    {
        uart->handler->Init.StopBits = UART_STOPBITS_1;
    }

    // Режим работы аппаратного контроля потока
    if ((opt->c_cflag & CRTS_IFLOW) && (opt->c_cflag & CCTS_OFLOW))
    {
        uart->handler->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
    }
    else if (opt->c_cflag & CRTS_IFLOW)
    {
        uart->handler->Init.HwFlowCtl = UART_HWCONTROL_RTS;
    }
    else if (opt->c_cflag & CCTS_OFLOW)
    {
        uart->handler->Init.HwFlowCtl = UART_HWCONTROL_CTS;
    }
    else
    {
        uart->handler->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    }

    uart->handler->Init.OverSampling = UART_OVERSAMPLING_16;
    uart->handler->Init.Mode = UART_MODE_TX_RX;

    if (opt->c_cc[V_TTY_DUPLEX] == V_TTY_HALFDUPLEX)
    {
        HAL_HalfDuplex_Init(uart->handler);
    }
    else
    {

        HAL_UART_Init(uart->handler);
    }

    uart->sfr->CR1 |=  USART_CR1_RXNEIE;

    uart_init_nvic(uart);
}

// ========================================================================================================
// Магия макросов - создание экземпляров уартов. Сначала для USART
// ========================================================================================================
#define USART_ASSIGN(N, TX_BUFFER_SIZE, RX_BUFFER_SIZE)                                         \
                                                                                                \
    void USART##N##_IRQHandler(void)                    {uart_isr(&uart##N);}                   \
                                                                                                \
    static volatile uint8_t uart##N##_buffer_rx[MAX(1, RX_BUFFER_SIZE)];                        \
    static volatile uint8_t uart##N##_buffer_tx[MAX(1, TX_BUFFER_SIZE)];                        \
                                                                                                \
    static UART_HandleTypeDef uart##N##_handler;                                                \
                                                                                                \
    uart_t uart##N =                                                                            \
    {                                                                                           \
        (USART_TypeDef*)USART##N##_BASE,                                                        \
        &uart##N##_handler,                                                                     \
        {                                                                                       \
            {                                                                                   \
                &uart##N##_buffer_rx[0],                                                        \
                MAX(1,RX_BUFFER_SIZE),                                                          \
                0,                                                                              \
                0                                                                               \
            },                                                                                  \
            {                                                                                   \
                &uart##N##_buffer_tx[0],                                                        \
                MAX(1,TX_BUFFER_SIZE),                                                          \
                0,                                                                              \
                0                                                                               \
            }                                                                                   \
        }                                                                                       \
    }
// ========================================================================================================
// Магия макросов - создание экземпляров уартов. Сначала для UART
// ========================================================================================================
#define UART_ASSIGN(N, TX_BUFFER_SIZE, RX_BUFFER_SIZE)                                          \
                                                                                                \
    void UART##N##_IRQHandler(void)                    {uart_isr(&uart##N);}                    \
                                                                                                \
    static volatile uint8_t uart##N##_buffer_rx[MAX(1, RX_BUFFER_SIZE)];                        \
    static volatile uint8_t uart##N##_buffer_tx[MAX(1, TX_BUFFER_SIZE)];                        \
                                                                                                \
    static UART_HandleTypeDef uart##N##_handler;                                                \
                                                                                                \
    uart_t uart##N =                                                                            \
    {                                                                                           \
        (USART_TypeDef*)UART##N##_BASE,                                                         \
        &uart##N##_handler,                                                                     \
        {                                                                                       \
            {                                                                                   \
                &uart##N##_buffer_rx[0],                                                        \
                MAX(1,RX_BUFFER_SIZE),                                                          \
                0,                                                                              \
                0                                                                               \
            },                                                                                  \
            {                                                                                   \
                &uart##N##_buffer_tx[0],                                                        \
                MAX(1,TX_BUFFER_SIZE),                                                          \
                0,                                                                              \
                0                                                                               \
            }                                                                                   \
        }                                                                                       \
    }
// ========================================================================================================
// Генерация уартов по надобности (указано в interface_conf)
// ========================================================================================================
#if defined (UART1_TX_BUFFER_SIZE) || defined (UART1_RX_BUFFER_SIZE)
USART_ASSIGN(1, UART1_TX_BUFFER_SIZE, UART1_RX_BUFFER_SIZE);
#endif /* UART1_IT */

// ========================================================================================================
#if defined (UART2_TX_BUFFER_SIZE) || defined (UART2_RX_BUFFER_SIZE)
USART_ASSIGN(2, UART2_TX_BUFFER_SIZE, UART2_RX_BUFFER_SIZE);
#endif /* UART2_IT */

// ========================================================================================================
#if defined (UART3_TX_BUFFER_SIZE) || defined (UART3_RX_BUFFER_SIZE)
USART_ASSIGN(3, UART3_TX_BUFFER_SIZE, UART3_RX_BUFFER_SIZE);
#endif /* UART2_IT */

// ========================================================================================================
#if defined (UART4_TX_BUFFER_SIZE) || defined (UART4_RX_BUFFER_SIZE)
UART_ASSIGN(4, UART4_TX_BUFFER_SIZE, UART4_RX_BUFFER_SIZE);
#endif /* UART2_IT */

// ========================================================================================================
#if defined (UART5_TX_BUFFER_SIZE) || defined (UART5_RX_BUFFER_SIZE)
UART_ASSIGN(5, UART5_TX_BUFFER_SIZE, UART5_RX_BUFFER_SIZE);
#endif /* UART2_IT */


