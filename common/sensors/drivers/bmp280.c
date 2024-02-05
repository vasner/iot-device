/**
 * BMP280, Digital Pressure Sensor, Bosch Sensortec
 */

#include "bmp280.h"

#include <stdbool.h>
#include <string.h>

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
    memset(regs, 0, sizeof(bmp280_regs_t));

    regs->temp_xlsb.rw = true;
    regs->temp_xlsb.addr = 0x7C;

    regs->temp_lsb.rw = true;
    regs->temp_lsb.addr = 0x7B;

    regs->temp_msb.rw = true;
    regs->temp_msb.addr = 0x7A;
    regs->temp_msb.temp_msb = 0x80;

    regs->press_xlsb.rw = true;
    regs->press_xlsb.addr = 0x79;

    regs->press_lsb.rw = true;
    regs->press_lsb.addr = 0x78;

    regs->press_msb.rw = true;
    regs->press_msb.addr = 0x77;
    regs->press_msb.press_msb = 0x80;

    regs->config.rw = true;
    regs->config.addr = 0x75;
    regs->config.t_sb = BMP280_STANDBY_TIME_0P5_MS;
    regs->config.filter = BMP280_FILTER_OFF;
    regs->config.spi3w_en = false;

    regs->ctrl_meas.rw = true;
    regs->ctrl_meas.addr = 0x74;
    regs->ctrl_meas.osrs_t = BMP280_TEMPERATURE_OVERSAMPLING_1X;
    regs->ctrl_meas.osrs_p = BMP280_PRESSURE_OVERSAMPLING_1X;
    regs->ctrl_meas.mode = BMP280_SLEEP;

    regs->status.rw = true;
    regs->status.addr = 0x73;

    regs->reset.rw = true;
    regs->reset.addr = 0x60;

    regs->id.rw = true;
    regs->id.addr = 0x50;
    regs->id.chip_id = BMP280_CHIP_ID;
}

static void _init_device(bmp280_t* state) {
    // TODO: Implement
    (void)state;
}
