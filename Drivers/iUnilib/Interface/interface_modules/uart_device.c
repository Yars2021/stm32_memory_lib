#include "../interface.h"


// ============================================================================
ssize_t uart1_write (char *buffer, size_t len)
{
    uart_put_block(&uart1, buffer, len);
    return len;
}

ssize_t uart1_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = uart_getc(&uart1);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}
// ============================================================================

ssize_t uart2_write (char *buffer, size_t len)
{
    uart_put_block(&uart2, buffer, len);
    return len;
}

ssize_t uart2_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = uart_getc(&uart2);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}
// ============================================================================

ssize_t uart3_write (char *buffer, size_t len)
{
    uart_put_block(&uart3, buffer, len);
    return len;
}

ssize_t uart3_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = uart_getc(&uart3);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}
// ============================================================================

ssize_t uart4_write (char *buffer, size_t len)
{
    uart_put_block(&uart4, buffer, len);
    return len;
}

ssize_t uart4_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = uart_getc(&uart4);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}
// ============================================================================

ssize_t uart5_write (char *buffer, size_t len)
{
    uart_put_block(&uart5, buffer, len);
    return len;
}

ssize_t uart5_read (char *buffer, size_t len)
{
    int res = 0;
    int ch = 0;

    while (len)
    {
        ch = uart_getc(&uart5);
        if (ch == -1) break;
        *buffer++ = (char)ch;
        res++;
        len--;
    }

    return res;
}
// ============================================================================
