//
// Created by Dizarker on 18.02.2021.
//
#include "../interface.h"

// ========================================================================================================
static INLINE void spi_set_tx_int (spi_t* const spi, int x)
{
    if (x)
    {
        spi->sfr->CR2 |= SPI_CR2_TXEIE;
    }
    else
    {
        spi->sfr->CR2 &= ~SPI_CR2_TXEIE;
    }
}



// ========================================================================================================
static INLINE void spi_rx_isr (spi_t* const spi, uint_fast16_t  status)
{
    static const uint_fast16_t  ERR_MASK = /*SPI_SR_FRE |*/ SPI_SR_OVR | SPI_SR_MODF | SPI_SR_CRCERR | SPI_SR_UDR;

    uint8_t data = spi->sfr->DR;
    if ((status & ERR_MASK) == 0)
    {
        fifo_put_byte(&spi->buffers.rx, data);
    }
}

// ========================================================================================================
static INLINE void spi_tx_isr (spi_t* const spi)
{
    uint_fast16_t  status = spi->sfr->SR;

    if (status & SPI_SR_TXE)
    {
        if (!fifo_get_byte(&spi->buffers.tx, (uint8_t*)&spi->sfr->DR))
        {
            spi_set_tx_int(spi, 0);
        }
    }
}

// ========================================================================================================
static INLINE void spi_isr (spi_t* const spi)
{
    uint_fast16_t status = spi->sfr->SR;

    if (status & SPI_SR_RXNE)
    {
        spi_rx_isr(spi, status);
    }
    else if (status & SPI_SR_TXE)
    {
        spi_tx_isr(spi);
    }
}

// ========================================================================================================
void spi_putc (spi_t* const spi, char c)
{
    fifo_put_byte(&spi->buffers.tx, c);
    spi_set_tx_int(spi, 1);
}

// ========================================================================================================
void spi_put_block (spi_t* const spi, const void *src, size_t len)
{
    if (len)
    {
        fifo_put_block(&spi->buffers.tx, src, len);
        spi_set_tx_int(spi, 1);
    }
}

// ========================================================================================================
int spi_getc (spi_t* const spi)
{
    uint8_t x;
    if (fifo_get_byte(&spi->buffers.rx, &x))
    {
        return (x);
    }
    return (-1);
}

// ========================================================================================================
void spi_flush (spi_t* const spi)
{
    fifo_flush(&spi->buffers.tx);
    fifo_flush(&spi->buffers.rx);
}

// ========================================================================================================
static void spi_init_rcc(spi_t* const spi)
{
    switch ((uint32_t)spi->sfr)
    {
        case SPI1_BASE:
            __HAL_RCC_SPI1_CLK_ENABLE();
            break;

#ifdef SPI2
        case SPI2_BASE:
            __HAL_RCC_SPI2_CLK_ENABLE();
            break;
#endif /* SPI2 */

#ifdef SPI3
            case SPI3_BASE:
            __HAL_RCC_SPI3_CLK_ENABLE();
            break;
#endif /* SPI3 */

        default: break;
    }
}

// ========================================================================================================
static void spi_init_nvic (spi_t* const spi)
{
    switch ((uint32_t)spi->sfr)
    {
        case SPI1_BASE: NVIC_EnableIRQ(SPI1_IRQn); break;

#ifdef SPI2
        case SPI2_BASE: NVIC_EnableIRQ(SPI2_IRQn); break;
#endif /* SPI2 */

#ifdef SPI3
            case SPI3_BASE: NVIC_EnableIRQ(SPI3_IRQn); break;
#endif /* SPI3 */

        default: break;
    }
}

// ========================================================================================================
void spi_init (spi_t* const spi)
{
    spi->handler->Instance = spi->sfr;
    HAL_SPI_DeInit(spi->handler);
    spi_init_rcc(spi);
    spi_flush(spi);

    HAL_SPI_Init(spi->handler);

    spi_init_nvic(spi);

    spi->sfr->CR2 |= SPI_CR2_RXNEIE;
    spi->sfr->CR1 |= SPI_CR1_SPE;
}

// ========================================================================================================
#define SPI_ASSIGN(N, TX_BUFFER_SIZE, RX_BUFFER_SIZE)                   \
                                                                        \
    void SPI##N##_IRQHandler (void)                 {spi_isr(&spi##N);} \
                                                                           \
    static volatile uint8_t spi##N##_buffer_rx[MAX(1, RX_BUFFER_SIZE)]; \
    static volatile uint8_t spi##N##_buffer_tx[MAX(1, TX_BUFFER_SIZE)]; \
                                                                        \
    static SPI_HandleTypeDef spi##N##_handler;                          \
                                                                        \
    spi_t spi##N =                                                      \
    {                                                                   \
        (SPI_TypeDef*)SPI##N##_BASE,                                    \
        &spi##N##_handler,                                              \
        {                                                               \
            {                                                           \
                &spi##N##_buffer_rx[0],                                 \
                MAX(1,RX_BUFFER_SIZE),                                  \
                0,                                                      \
                0                                                       \
            },                                                          \
            {                                                           \
                &spi##N##_buffer_tx[0],                                 \
                MAX(1,TX_BUFFER_SIZE),                                  \
                0,                                                      \
                0                                                       \
            }                                                           \
        }                                                               \
    }

// ========================================================================================================
// Генерация по надобности (указано в interface_conf)
// ========================================================================================================
#if defined (SPI1_TX_BUFFER_SIZE) || defined (SPI1_RX_BUFFER_SIZE)
SPI_ASSIGN(1, SPI1_TX_BUFFER_SIZE, SPI1_RX_BUFFER_SIZE);
#endif /* UART1_IT */

// ========================================================================================================
#if defined (SPI2_TX_BUFFER_SIZE) || defined (SPI2_RX_BUFFER_SIZE)
SPI_ASSIGN(2, SPI2_TX_BUFFER_SIZE, SPI2_RX_BUFFER_SIZE);
#endif /* UART2_IT */

// ========================================================================================================
#if defined (SPI3_TX_BUFFER_SIZE) || defined (SPI3_RX_BUFFER_SIZE)
SPI_ASSIGN(3, SPI3_TX_BUFFER_SIZE, SPI3_RX_BUFFER_SIZE);
#endif /* UART2_IT */
