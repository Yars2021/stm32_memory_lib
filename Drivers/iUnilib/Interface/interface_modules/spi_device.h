#ifndef _SPI_DEVICE_H
#define _SPI_DEVICE_H

ssize_t spi1_write (char *buffer, size_t len);
ssize_t spi1_read (char *buffer, size_t len);
ssize_t spi2_write (char *buffer, size_t len);
ssize_t spi2_read (char *buffer, size_t len);
ssize_t spi3_write (char *buffer, size_t len);
ssize_t spi3_read (char *buffer, size_t len);

void spi_device_init(spi_t* const spi, struct termios *opt);

#endif /* _SPI_DEVICE_H */
