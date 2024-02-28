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

#include "bme280.h"

typedef struct {
    int8_t temperature;
    uint8_t humidity;
    uint16_t pressure;
} sensors_data_t;

typedef struct {
    bool status;
    bme280_t bme280;
} sensors_t;

bool sensors_init(
    sensors_t* state, bme280_read_reg_t bme280_read_reg, bme280_write_reg_t bme280_write_reg, void* bme280_ctx
);
void sensors_measure(sensors_t* state, sensors_data_t* data);
bool sensors_get_status(sensors_t* state);
void sensors_to_json(sensors_data_t* data, char* json);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_H
