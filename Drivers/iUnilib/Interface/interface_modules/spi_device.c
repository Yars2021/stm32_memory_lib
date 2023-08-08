#include "../interface.h"


// ============================================================================
ssize_t spi1_write (char *buffer, size_t len)
{
    spi_put_block(&spi1, buffer, len);
    return len;
}

ssize_t spi1_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = spi_getc(&spi1);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}

// ============================================================================
ssize_t spi2_write (char *buffer, size_t len)
{
    spi_put_block(&spi2, buffer, len);
    return len;
}

ssize_t spi2_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = spi_getc(&spi2);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}

// ============================================================================
ssize_t spi3_write (char *buffer, size_t len)
{
    spi_put_block(&spi3, buffer, len);
    return len;
}

ssize_t spi3_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = spi_getc(&spi3);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}

// ============================================================================

void spi_device_init(spi_t* const spi, struct termios *opt)
{
    if (HAL_SPI_DeInit(spi->handler) == HAL_ERROR) return;

    spi->handler->Instance = spi->sfr;

    // Master / Slave mode
    if (opt->c_cc[V_SPI_MASTER_MODE] == V_SPI_MASTER_MODE_MASTER)
        spi->handler->Init.Mode = SPI_MODE_MASTER;
    else
        spi->handler->Init.Mode = SPI_MODE_SLAVE;

    // Direction
    if (opt->c_cc[V_SPI_DIRECTION] == V_SPI_DIRECTION_1LINES)
        spi->handler->Init.Direction = SPI_DIRECTION_1LINE;
    else if (opt->c_cc[V_SPI_DIRECTION] == V_SPI_DIRECTION_2LINES)
        spi->handler->Init.Direction = SPI_DIRECTION_2LINES;
    else if (opt->c_cc[V_SPI_DIRECTION] == V_SPI_DIRECTION_2LINES_RXONLY)
        spi->handler->Init.Direction = SPI_DIRECTION_2LINES_RXONLY;


    // Datasize
    spi->handler->Init.DataSize = SPI_DATASIZE_8BIT;

    // SPI Mode
    if (opt->c_cc[V_SPI_MODE] == V_SPI_MODE_0) {
        spi->handler->Init.CLKPolarity = SPI_POLARITY_LOW;
        spi->handler->Init.CLKPhase = SPI_PHASE_1EDGE;
    }
    else if (opt->c_cc[V_SPI_MODE] == V_SPI_MODE_1) {
        spi->handler->Init.CLKPolarity = SPI_POLARITY_LOW;
        spi->handler->Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    else if (opt->c_cc[V_SPI_MODE] == V_SPI_MODE_2) {
        spi->handler->Init.CLKPolarity = SPI_POLARITY_HIGH;
        spi->handler->Init.CLKPhase = SPI_PHASE_1EDGE;
    }
    else if (opt->c_cc[V_SPI_MODE] == V_SPI_MODE_3) {
        spi->handler->Init.CLKPolarity = SPI_POLARITY_HIGH;
        spi->handler->Init.CLKPhase = SPI_PHASE_2EDGE;
    }

    // SPI BaudRate prescaller
    switch(opt->c_cc[V_SPI_BAUD])
    {
        case V_SPI_PRESCALLER_2: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; break;
        case V_SPI_PRESCALLER_4: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4; break;
        case V_SPI_PRESCALLER_8: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; break;
        case V_SPI_PRESCALLER_16: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; break;
        case V_SPI_PRESCALLER_32: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; break;
        case V_SPI_PRESCALLER_64: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64; break;
        case V_SPI_PRESCALLER_128: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; break;
        case V_SPI_PRESCALLER_256: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; break;
        default: spi->handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; break;
    }

    // NSS Mode
    spi->handler->Init.NSS = SPI_NSS_SOFT;

    // TI Mode
    spi->handler->Init.TIMode = SPI_TIMODE_DISABLE;

    // CRC Calculation
    spi->handler->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    // CRC Polynomial
    spi->handler->Init.CRCPolynomial = 10;

    spi_init(spi);
}
