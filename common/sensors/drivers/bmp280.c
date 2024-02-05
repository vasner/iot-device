/**
 * BMP280, Digital Pressure Sensor, Bosch Sensortec
 */

#include "bmp280.h"

static void _reset_registers(bmp280_regs_t* state);
static void _init_device(bmp280_t* state);

/**
 * Initializes BMP280 device driver
 * @param[in,out] state BMP280 driver state
 * @param[in] read_register Read register callback, set to NULL if unused
 * @param[in] write_register Write register callback, set to NULL if unused
 * @param[in] ctx External context. It is passed as parameter into handlers and callbacks functions,
 *                set to NULL if unused
 */
void bmp280_init(bmp280_t* state, bmp280_read_reg_t read_register, bmp280_write_reg_t write_register, void* ctx) {
    state->read_register = read_register;
    state->write_register = write_register;
    state->ctx = ctx;
    _reset_registers(&state->regs.regs);
    _init_device(state);
}

/**
 * Gets pressure from device
 * @param[in] state BMP280 driver state
 * @return Measured pressure
 */
uint16_t bmp280_get_pressure(bmp280_t* state) {
    // TODO: Implement
    (void)state;
    return 0;
}

/**
 * Gets temperature from device
 * @param[in] state BMP280 driver state
 * @return Measured temperature
 */
uint16_t bmp280_get_temperature(bmp280_t* state) {
    // TODO: Implement
    (void)state;
    return 0;
}

static void _reset_registers(bmp280_regs_t* regs) {
    // TODO: Implement
    (void)regs;
}

static void _init_device(bmp280_t* state) {
    // TODO: Implement
    (void)state;
}
