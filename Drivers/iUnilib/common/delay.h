#ifndef _DELAY_H_
#define _DELAY_H_

// =====================================================================================================================
static INLINE void delay_8cycles (uint32_t x)
{
    while (x--)
    {
        nop();
        nop();
        nop();
        nop();
    }
}

// =====================================================================================================================
static INLINE void delay_cycles (uint32_t x)
{
    uint32_t rem = x&7;

    if      (rem == 1) { nop(); }
    else if (rem == 2) { nop(); nop(); }
    else if (rem == 3) { nop(); nop(); nop(); }
    else if (rem == 4) { nop(); nop(); nop(); nop(); }
    else if (rem == 5) { nop(); nop(); nop(); nop(); nop(); }
    else if (rem == 6) { nop(); nop(); nop(); nop(); nop(); nop(); }
    else if (rem == 7) { nop(); nop(); nop(); nop(); nop(); nop(); nop(); }

    if ((x /= 8)) delay_8cycles(x);
}

// =====================================================================================================================
static __inline void delay_ns   (double ns)     {delay_cycles(ns/((double)1e9/(double)F_CPU) + 0.5);}
                void delay_us   (uint32_t us);
                void delay_ms   (uint32_t ms);
                void delay_sec  (uint32_t sec);

#endif /* _DELAY_H_ */
