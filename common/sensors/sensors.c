/**
 * Sensors module
 */

#include "sensors.h"

#include <stdio.h>

/**
 * Initializes sensors module
 * @param[in,out] state Sensors module state
 * @param[in] bme280_read_reg BME280 read register callback, set to NULL if unused
 * @param[in] bme280_write_reg BME280 write register callback, set to NULL if unused
 * @param[in] bme280_ctx BME280 external context. It is passed as parameter into handlers and callbacks functions,
 *                       set to NULL if unused
 * @return `true` if initialization was successful, `false` otherwise
 */
bool sensors_init(
    sensors_t* state, bme280_read_reg_t bme280_read_reg, bme280_write_reg_t bme280_write_reg, void* bme280_ctx
) {
    state->status = bme280_init(&state->bme280, bme280_read_reg, bme280_write_reg, bme280_ctx);
    if (!state->status) { return state->status; }
    bme280_set_standby_time(&state->bme280, BME280_STANDBY_TIME_125_MS);
    bme280_set_pressure_oversampling(&state->bme280, BME280_PRESSURE_OVERSAMPLING_16X);
    bme280_set_temperature_oversampling(&state->bme280, BME280_TEMPERATURE_OVERSAMPLING_16X);
    bme280_set_humidity_oversampling(&state->bme280, BME280_HUMIDITY_OVERSAMPLING_16X);
    bme280_set_filter(&state->bme280, BME280_FILTER_16X);
    bme280_set_mode(&state->bme280, BME280_NORMAL);
    return state->status;
}

void sensors_measure(sensors_t* state, sensors_data_t* data) {
    bme280_get_measurement(&state->bme280, &data->pressure, &data->temperature, &data->humidity);
}

bool sensors_get_status(sensors_t* state) { return state->status; }

void sensors_to_json(sensors_data_t* data, char* json) {
    sprintf(
        json,
        "{\"temperature\":{\"value\":%d,\"unit\":\"%c%cC\"},"
        "\"pressure\":{\"value\":%d,\"unit\":\"mmHg\"},"
        "\"humidity\":{\"value\":%d,\"unit\":\"%%\"}}",
        data->temperature, 0xC2, 0xB0, data->pressure, data->humidity
    );
}
