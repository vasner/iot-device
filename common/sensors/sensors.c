/**
 * Sensors module
 */

#include "sensors.h"

/**
 * Initializes sensors module
 * @param[in,out] state Sensors module state
 * @param[in] bmp280_read_reg BMP280 read register callback, set to NULL if unused
 * @param[in] bmp280_write_reg BMP280 write register callback, set to NULL if unused
 * @param[in] bmp280_ctx BMP280 external context. It is passed as parameter into handlers and callbacks functions,
 *                set to NULL if unused
 * @return `true` if initialization was successful, `false` otherwise
 */
bool sensors_init(
    sensors_t* state, bmp280_read_reg_t bmp280_read_reg, bmp280_write_reg_t bmp280_write_reg, void* bmp280_ctx
) {
    bool is_success = bmp280_init(&state->bmp280, bmp280_read_reg, bmp280_write_reg, bmp280_ctx);
    if (!is_success) { return is_success; }
    bmp280_set_standby_time(&state->bmp280, BMP280_STANDBY_TIME_125_MS);
    bmp280_set_mode(&state->bmp280, BMP280_NORMAL);
    bmp280_set_pressure_oversampling(&state->bmp280, BMP280_PRESSURE_OVERSAMPLING_16X);
    bmp280_set_temperature_oversampling(&state->bmp280, BMP280_TEMPERATURE_OVERSAMPLING_16X);
    bmp280_set_filter(&state->bmp280, BMP280_FILTER_16X);
    return is_success;
}

void sensors_measure(sensors_t* state, sensors_data_t* data) {
    data->humidity = 0;
    bmp280_get_measurement(&state->bmp280, &data->pressure, &data->temperature);
}
