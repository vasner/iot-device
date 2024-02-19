/**
 * Sensors module
 */

#include "sensors.h"

#include <stdio.h>

/**
 * Initializes sensors module
 * @param[in,out] state Sensors module state
 * @param[in] bmp280_read_reg BMP280 read register callback, set to NULL if unused
 * @param[in] bmp280_write_reg BMP280 write register callback, set to NULL if unused
 * @param[in] bmp280_ctx BMP280 external context. It is passed as parameter into handlers and callbacks functions,
 *                       set to NULL if unused
 * @return `true` if initialization was successful, `false` otherwise
 */
bool sensors_init(
    sensors_t* state, bmp280_read_reg_t bmp280_read_reg, bmp280_write_reg_t bmp280_write_reg, void* bmp280_ctx
) {
    state->status = bmp280_init(&state->bmp280, bmp280_read_reg, bmp280_write_reg, bmp280_ctx);
    if (!state->status) { return state->status; }
    bmp280_set_standby_time(&state->bmp280, BMP280_STANDBY_TIME_125_MS);
    bmp280_set_pressure_oversampling(&state->bmp280, BMP280_PRESSURE_OVERSAMPLING_16X);
    bmp280_set_temperature_oversampling(&state->bmp280, BMP280_TEMPERATURE_OVERSAMPLING_16X);
    bmp280_set_filter(&state->bmp280, BMP280_FILTER_16X);
    bmp280_set_mode(&state->bmp280, BMP280_NORMAL);
    return state->status;
}

void sensors_measure(sensors_t* state, sensors_data_t* data) {
    data->humidity = 0;
    bmp280_get_measurement(&state->bmp280, &data->pressure, &data->temperature);
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
