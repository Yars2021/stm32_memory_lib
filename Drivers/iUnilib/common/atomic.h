/*
 *  atomic.h rev.1.0.0 2010-06-28
 *
 *  Cortex-m3 KEIL atomic module.
 *
 *  Contributors:
 *                Ivanov Anton (c) www.automatix.ru
 *
 */

#ifndef _ATOMIC_H_
#define _ATOMIC_H_

// =====================================================================================================================
static __inline uint32_t get_interrupt_state(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, primask" : "=r" (result) :: "memory");
    return (result);
}

// =====================================================================================================================
static __inline void set_interrupt_state(uint32_t status)
{
    __asm volatile ("MSR primask, %0" :: "r" (status) : "memory");
}

// =====================================================================================================================

#define ENTER_CRITICAL_SECTION() do {uint32_t sreg_temp = get_interrupt_state(); __disable_irq();
#define LEAVE_CRITICAL_SECTION() set_interrupt_state(sreg_temp);} while(0)

#endif /* _ATOMIC_H_ */

