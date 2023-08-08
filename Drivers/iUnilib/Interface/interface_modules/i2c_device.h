#ifndef _I2C_DEVICE_H
#define _I2C_DEVICE_H

#include "software_timer.h"



ssize_t i2c1_write (char *buffer, size_t len);
ssize_t i2c1_read (char *buffer, size_t len);
ssize_t i2c2_write (char *buffer, size_t len);
ssize_t i2c2_read (char *buffer, size_t len);
void i2c_device_init(i2c_t* const i2c, struct termios *opt);


#endif /* _I2C_DEVICE_H */
