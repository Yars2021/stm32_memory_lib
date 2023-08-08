//
// Created by Dizarker on 18.02.2021.
//
#ifndef _SPI_IT_H
#define _SPI_IT_H

typedef struct
{
    SPI_TypeDef *const sfr;
    SPI_HandleTypeDef *const handler;
    struct
    {
        fifo_t rx;
        fifo_t tx;
    } buffers;
} spi_t;

extern spi_t spi1;
extern spi_t spi2;
extern spi_t spi3;

void spi_init           (spi_t* const spi);
void spi_putc           (spi_t* const spi, char c);
int  spi_getc           (spi_t* const spi);
void spi_flush          (spi_t* const spi);
void spi_put_block      (spi_t* const spi, const void* src, size_t len);
void spi_get_block      (spi_t* const spi, const void* dst, size_t len);

#endif /* _SPI_IT_H_ */
