/*
 *  global_macro.h rev.2.0.0 2010-02-03
 *
 *  Common macros lib.
 *
 *  Contributors:
 *                Ivanov Anton (c) www.automatix.ru
 *
 */

#ifndef _GLOBAL_MACRO_H
#define _GLOBAL_MACRO_H

#ifdef __cplusplus
    extern "C" {
#endif

// ===================================================================================================================
// Макросы аттрибутов
// ===================================================================================================================
#define INLINE                                  __inline __attribute__((always_inline))
#define NOINLINE                                __attribute__((noinline))
#define NORETURN                                __attribute__((noreturn))

#define nop()                                   __asm("mov r0,r0")

#define pgm_read_byte(address)                  *(( uint8_t*)address)
#define pgm_read_word(address)                  *((uint16_t*)address)
#define pgm_read_dword(address)                 *((uint32_t*)address)
#define pgm_read_float(address)                 *((   float*)address)

#define pgm_read_ptr(address)                   ((void*)pgm_read_word(address))

#define PGM_READ(DST,SRC)                       memcpy((DST), (SRC), sizeof(*(DST)))
#define RAM_READ(DST,SRC)                       memcpy((DST), (SRC), sizeof(*(DST)))

// ===================================================================================================================
// Строковые макросы
// ===================================================================================================================
#define STR(S)                                  #S              // STR(blablabla) = "blablabla"
#define XSTR(S)                                 STR(S)          // STR(_version) = "v1.0" if _version = "v1.0"

// ===================================================================================================================
// Битовые и числовые макросы
// ===================================================================================================================
#define ARRAY_SIZE(ARRAY)                       (sizeof((ARRAY))/sizeof((ARRAY[0])))
#define TESTBIT(BYTE,BIT)                       ((BYTE)&(1UL<<(BIT)))
#define BYTE0(X)                                ((uint8_t)((X)>>0))
#define BYTE1(X)                                ((uint8_t)((X)>>8))
#define BYTE2(X)                                ((uint8_t)((X)>>16))
#define BYTE3(X)                                ((uint8_t)((X)>>24))

#define LH2WORD(LBYTE,HBYTE)                    (((uint16_t)(HBYTE)<<8) + (uint8_t)(LBYTE))

// ===================================================================================================================
// Математические макросы
// ===================================================================================================================
#define PI                                      3.14159265358979323846264
#define RAD2DEG(RAD)                            (((RAD)*180.0)/PI)
#define DEG2RAD(DEG)                            (((DEG)/180.0)*PI)

#define FLT_NAN                                 (0.0f/0.0f) // 0xffc0xxxx
#define FLT_INF                                 (1.0f/0.0f) // 0x7f800000
#define DOUBLE_NAN                              (((double)0.0)/((double)0.0))

#define MIN(a,b)                                (((a)<(b))?(a):(b))
#define MAX(a,b)                                (((a)>(b))?(a):(b))
#define ABS(x)                                  (((x)>0)?(x):(-(x)))
#define FROUND(x)                               (((x)>=0)?(int32_t)((x)+0.5):(int32_t)((x)-0.5))

#define F1_CHECK ((defined STM32F100xB) || \
                 (defined STM32F100xE) || \
                 (defined STM32F101x6) || \
                 (defined STM32F101xB) || \
                 (defined STM32F101xE) || \
                 (defined STM32F101xG) || \
                 (defined STM32F102x6) || \
                 (defined STM32F102xB) || \
                 (defined STM32F103x6) || \
                 (defined STM32F103xB) || \
                 (defined STM32F103xE) || \
                 (defined STM32F103xG) || \
                 (defined STM32F105xC) || \
                 (defined STM32F107xC))

#define F3_CHECK (defined (STM32F301x8) || \
                 (defined STM32F302x8) || \
                 (defined STM32F318xx) || \
                 (defined STM32F302xC) || \
                 (defined STM32F303xC) || \
                 (defined STM32F358xx) || \
                 (defined STM32F303x8) || \
                 (defined STM32F334x8) || \
                 (defined STM32F328xx) || \
                 (defined STM32F302xE) || \
                 (defined STM32F303xE) || \
                 (defined STM32F398xx) || \
                 (defined STM32F373xC) || \
                 (defined STM32F378xx))

#define F4_CHECK ((defined STM32F405xx) || \
                 (defined STM32F415xx) || \
                 (defined STM32F407xx) || \
                 (defined STM32F417xx) || \
                 (defined STM32F427xx) || \
                 (defined STM32F437xx) || \
                 (defined STM32F29xx) || \
                 (defined STM32F439xx) || \
                 (defined STM32F469xx) || \
                 (defined STM32F479xx) || \
                 (defined STM32F413xx) || \
                 (defined STM32F423xx))

#ifdef __cplusplus
    }
#endif

#endif /* _GLOBAL_MACRO_H_ */
