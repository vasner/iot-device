/**
 * Sensors module
 */

#include "sensors.h"

void sensors_init(
    sensors_t* state, bmp280_read_reg_t bmp280_read_reg, bmp280_write_reg_t bmp280_write_reg, void* bmp280_ctx
) {
    bmp280_init(&state->bmp280, bmp280_read_reg, bmp280_write_reg, bmp280_ctx);
    bmp280_set_standby_time(&state->bmp280, BMP280_STANDBY_TIME_125_MS);
    bmp280_set_mode(&state->bmp280, BMP280_NORMAL);
    bmp280_set_pressure_oversampling(&state->bmp280, BMP280_PRESSURE_OVERSAMPLING_16X);
    bmp280_set_temperature_oversampling(&state->bmp280, BMP280_TEMPERATURE_OVERSAMPLING_16X);
    bmp280_set_filter(&state->bmp280, BMP280_FILTER_16X);
}

void sensors_measure(sensors_t* state, sensors_data_t* data) {
    data->humidity = 0;
    bmp280_get_measurement(&state->bmp280, &data->pressure, &data->temperature);
}
