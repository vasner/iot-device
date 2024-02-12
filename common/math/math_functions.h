/**
 * Math functions library
 */
#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define __INLINE inline __attribute__((always_inline))

#define MATH_CONST_S32(C, RADIX) ((int32_t)((C) * (1 << (RADIX))))

__INLINE int32_t sc_mul_s32(int32_t x, int32_t y, uint8_t radix) { return (int32_t)(((int64_t)x * y) >> radix); }

#ifdef __cplusplus
}
#endif

#endif  // MATH_FUNCTIONS_H
