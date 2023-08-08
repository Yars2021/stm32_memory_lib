#ifndef _WDT_H_
#define _WDT_H_

enum
{
    WDT_KEY_START                   = 0xcccc,
    WDT_KEY_RELOAD                  = 0xaaaa,
    WDT_KEY_ACCESS_ENABLE           = 0x5555
};

enum
{
    WDT_PRESCALER_4                 = 0,
    WDT_PRESCALER_8                 = 1,
    WDT_PRESCALER_16                = 2,
    WDT_PRESCALER_32                = 3,
    WDT_PRESCALER_64                = 4,
    WDT_PRESCALER_128               = 5,
    WDT_PRESCALER_256               = 6
};

// =====================================================================================================================
static INLINE void wdt_reset (void)
{
#ifdef NDEBUG
    IWDG->KR = WDT_KEY_RELOAD;
#endif /* NDEBUG */
}

// =====================================================================================================================
static INLINE void wdt_enable (const float wdto_sec)    // range [0..3.2767] sec, resolution 4096
{
#ifdef NDEBUG
    static const uint32_t WDT_DIV = 32;
    static const uint32_t WDT_CLK = 40000;              // is the worst case, nominal vluse is 32768

    uint32_t x = wdto_sec * WDT_CLK / WDT_DIV;          // reload value range [0x0000..0x0fff]

    IWDG->KR = WDT_KEY_ACCESS_ENABLE;                   // unlock PR & RLR
    IWDG->PR = WDT_PRESCALER_32;
    IWDG->RLR = x;                                      // init
    IWDG->KR = WDT_KEY_RELOAD;                          // apply and auto lock
    IWDG->KR = WDT_KEY_START;                           // start (the LSI oscillator will be enabled by hardware)
#endif /* NDEBUG */
}
#endif /* _WDT_H */

// =====================================================================================================================
static INLINE void NORETURN mcu_reset (void)            // Must be not macro!
{
    NVIC_SystemReset();

    for (;;);                                           // Wait until reset
}