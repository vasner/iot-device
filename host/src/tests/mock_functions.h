/**
 * Mock functions
 */
#ifndef MOCK_FUNCTIONS_H
#define MOCK_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void tests_bmp280_write_reg(void* ctx, uint16_t reg);
uint8_t tests_bmp280_read_reg(void* ctx, uint16_t reg);

#ifdef __cplusplus
}
#endif

#endif  // MOCK_FUNCTIONS_H