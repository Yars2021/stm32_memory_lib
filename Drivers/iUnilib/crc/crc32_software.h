#ifndef CRC32_SOFTWARE_H_
#define CRC32_SOFTWARE_H_

#include <stdlib.h>
#include <inttypes.h>

void crc32sftwr_init(void);
uint32_t crc32_sftwr(uint32_t init_crc, const uint8_t *buf, size_t len);

#endif
