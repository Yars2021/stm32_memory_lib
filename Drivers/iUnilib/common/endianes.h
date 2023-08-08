/**
  ******************************************************************************
  * @file endianes.h
  * @author Vasiliy Turchenko
  * @version V0.0.1
  * @date     11-09-2020
  * @brief
  * @note This file is part of the unilib
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT STC </center></h2>
  ******************************************************************************
  */

#ifndef ENDIANES_H
#define ENDIANES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* define to 1 to enable standalone tests */
#define ENDIANES_TEST 0

#if (ENDIANES_TEST == 1)
void endianes_test(void);
#endif


#define SWAP_BYTES_IN_U16(A) ( (uint16_t)( (uint16_t)(A[0] << 8) | (uint16_t)A[1]) )

#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#else
    #error Endiannes is not defined by the compiler!!!
#endif


/* all STM32 are little-endian */
/* Intel CPUs are little endian */

static inline uint16_t read_word_lendian  (const void* src_lendian)
{

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    /* no byte swap required */
    return (uint16_t) *(const uint16_t*)src_lendian;

#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    const uint8_t* src = src_lendian;
    /* byte swap required */
    return SWAP_BYTES_IN_U16(src);

#endif
}

static inline uint16_t read_word_bendian  (const void* src_bendian)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    const uint8_t* src = (const uint8_t*)src_bendian;
    /* byte swap required */
    return SWAP_BYTES_IN_U16(src);

#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    /* no byte swap required */
    return (uint16_t) *(const uint16_t*)src_bendian;

#endif
}

static inline void write_word_lendian (void* dst_lendian, uint16_t x)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    /* no byte swap required */
    *(uint16_t*)dst_lendian = x;

#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    uint8_t* dst = dst_bendian;
    /* byte swap required */
    dst[0] = (uint8_t)(x >> 8); // hi
    dst[1] = (uint8_t)(x);      // lo

#endif
}


static inline void write_word_bendian (void* dst_bendian, uint16_t x)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    uint8_t* dst = (uint8_t*)dst_bendian;
    /* byte swap required */
    dst[0] = (uint8_t)(x >> 8); // hi
    dst[1] = (uint8_t)(x);      // lo

#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    /* no byte swap required */
    *(uint16_t*)dst_bendian = x;

#endif
}

static inline uint32_t read_dword_lendian (const void* src_lendian)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    /* no byte swap required */
    return (const uint32_t) *(const uint32_t*)src_lendian;

#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    const uint8_t* src = src_bendian;
    /* byte swap required */
    return ((uint32_t)(src[0] << 24) + (uint32_t)(src[1] << 16) + (uint32_t)(src[2] << 8) + (uint32_t)src[3]);

#endif
}

static inline uint32_t read_dword_bendian (const void* src_bendian)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    const uint8_t* src = (const uint8_t*)src_bendian;
    /* byte swap required */
    return ((uint32_t)(src[0] << 24) + (uint32_t)(src[1] << 16) + (uint32_t)(src[2] << 8) + (uint32_t)src[3]);

#elif 	(__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    /* no byte swap required */
    return (uint32_t) *(const uint32_t*)src_bendian;

#endif
}

static inline void write_dword_lendian(void* dst_lendian, uint32_t x)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    /* no byte swap required */
    *(uint32_t*)dst_lendian = x;

#elif 	(__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    uint8_t* dst = dst_lendian;
    /* byte swap required */
    dst[0] = (uint8_t)( x >> 24 ); // hi
    dst[1] = (uint8_t)( x >> 16 );
    dst[2] = (uint8_t)( x >> 8  );
    dst[3] = (uint8_t)( x >> 0  ); // lo

#endif
}

static inline void write_dword_bendian(void* dst_bendian, uint32_t x)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    uint8_t* dst = (uint8_t*)dst_bendian;
    /* byte swap required */
    dst[0] = (uint8_t)( x >> 24 ); // hi
    dst[1] = (uint8_t)( x >> 16 );
    dst[2] = (uint8_t)( x >> 8  );
    dst[3] = (uint8_t)( x       ); // lo
#elif 	(__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    /* no byte swap required */
    *(uint32_t*)dst_bendian = x;

#endif
}

float         read_float_lendian (const void* src_lendian);
float         read_float_bendian (const void* src_bendian);

void          write_float_lendian(void* dst_lendian, float x);
void          write_float_bendian(void* dst_bendian, float x);

#ifdef __cplusplus
}
#endif

#endif /* ENDIANES_H */
