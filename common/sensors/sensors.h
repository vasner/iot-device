/**
 * Sensors module
 */
#ifndef SENSORS_H
#define SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "bmp280.h"

typedef struct {
    int8_t temperature;
    uint8_t humidity;
    uint16_t pressure;
} sensors_data_t;

typedef struct {
    bmp280_t bmp280;
} sensors_t;

bool sensors_init(
    sensors_t* state, bmp280_read_reg_t bmp280_read_reg, bmp280_write_reg_t bmp280_write_reg, void* bmp280_ctx
);
void sensors_measure(sensors_t* state, sensors_data_t* data);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_H
