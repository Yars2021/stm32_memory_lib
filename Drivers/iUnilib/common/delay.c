#include "include.h"

//=============================================================================
/* User code to delay the processor goes here. Below is example code that
 *   works for a 32-bit ARM7 Cortex processor clocked at 72 MHz.  For any
 *  other processor you will need to replace this with code that works
 *  for your processor.  Many compilers will have their own delay routines
 *  so make sure you check your compiler documentation before attempting to
 *  write your own.
 */

//=============================================================================
#if F1_CHECK
void delay_us (uint32_t us)
{
    while (us--)
    {
        nop();
        nop();
        nop();
        nop();
    }
}
#endif /* F1_CHECK */

//=============================================================================
#if F4_CHECK
void INLINE delay_us (uint32_t us)
{
    us *= 21;
    while (us--)
    {
        nop();
        nop();
        nop();
        nop();
    }
}
#endif /* F4_CHECK */

//=============================================================================
void delay_ms (uint32_t ms)
{
    delay_us(ms * 1000UL);
    wdt_reset();
}

//=============================================================================
void delay_sec (uint32_t sec)
{
    while (sec--)
    {
        wdt_reset();
        delay_us (1000000UL);
    }
    wdt_reset();
}
