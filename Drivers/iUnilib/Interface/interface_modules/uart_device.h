#ifndef _UART_DEVICE_H_
#define _UART_DEVICE_H_

ssize_t uart1_write (char *buffer, size_t len);
ssize_t uart1_read (char *buffer, size_t len);
ssize_t uart2_write (char *buffer, size_t len);
ssize_t uart2_read (char *buffer, size_t len);
ssize_t uart3_write (char *buffer, size_t len);
ssize_t uart3_read (char *buffer, size_t len);
ssize_t uart4_write (char *buffer, size_t len);
ssize_t uart4_read (char *buffer, size_t len);
ssize_t uart5_write (char *buffer, size_t len);
ssize_t uart5_read (char *buffer, size_t len);

#endif /* _UART_DEVICE_H_ */
