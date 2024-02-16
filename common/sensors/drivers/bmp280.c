/**
 * BMP280, Digital Pressure Sensor, Bosch Sensortec
 */

#include "bmp280.h"

#include <string.h>

#include "log.h"
#include "math_functions.h"
#include "utils.h"

static void _reset_registers(bmp280_regs_t* state);
static void _sync_registers(bmp280_t* state);
static int32_t _get_press_raw(bmp280_t* state);
static int32_t _get_temp_raw(bmp280_t* state);
static int32_t _press_raw_to_pa(bmp280_t* state, int32_t p_raw, int32_t t_raw);
static int32_t _temp_raw_to_fine(bmp280_t* state, int32_t t_raw);

/**
 * Initializes BMP280 device driver
 * @param[in,out] state BMP280 driver state
 * @param[in] read_register Read register callback, set to NULL if unused
 * @param[in] write_register Write register callback, set to NULL if unused
 * @param[in] ctx External context. It is passed as parameter into handlers and callbacks functions,
 *                set to NULL if unused
 * @return `true` if initialization was successful, `false` otherwise
 */
bool bmp280_init(bmp280_t* state, bmp280_read_reg_t read_register, bmp280_write_reg_t write_register, void* ctx) {
    bmp280_regs_t* regs = &state->regs.regs;

    state->read_register = read_register;
    state->write_register = write_register;
    state->ctx = ctx;
    _reset_registers(regs);

    // Software reset
    delay_ms(2);  // Start-up time
    regs->reset.rw = false;
    regs->reset.reset = BMP280_RESET_CODE;
    write_register(ctx, state->regs.regs_u16[BMP280_REG_OFFSET_RESET]);
    delay_ms(2);  // Start-up time

    _sync_registers(state);

    bool is_success = regs->id.chip_id == BMP280_CHIP_ID;
    if (!is_success) { LOG_ERROR("Wrong BMP280 chip id `%2X`", regs->id.chip_id); }
    return is_success;
}

void bmp280_set_standby_time(bmp280_t* state, bmp280_standby_time_t time) {
    bmp280_regs_t* regs = &state->regs.regs;
    regs->config.rw = false;
    regs->config.t_sb = time;
    state->write_register(state->ctx, state->regs.regs_u16[BMP280_REG_OFFSET_CONFIG]);
}

void bmp280_set_mode(bmp280_t* state, bmp280_mode_t mode) {
    bmp280_regs_t* regs = &state->regs.regs;
    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.mode = mode;
    state->write_register(state->ctx, state->regs.regs_u16[BMP280_REG_OFFSET_CTRL_MEAS]);
}

void bmp280_set_pressure_oversampling(bmp280_t* state, bmp280_pressure_oversampling_t oversampling) {
    bmp280_regs_t* regs = &state->regs.regs;
    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.osrs_p = oversampling;
    state->write_register(state->ctx, state->regs.regs_u16[BMP280_REG_OFFSET_CTRL_MEAS]);
}

void bmp280_set_temperature_oversampling(bmp280_t* state, bmp280_temperature_oversampling_t oversampling) {
    bmp280_regs_t* regs = &state->regs.regs;
    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.osrs_t = oversampling;
    state->write_register(state->ctx, state->regs.regs_u16[BMP280_REG_OFFSET_CTRL_MEAS]);
}

void bmp280_set_filter(bmp280_t* state, bmp280_filter_t filter) {
    bmp280_regs_t* regs = &state->regs.regs;
    regs->config.rw = false;
    regs->config.filter = filter;
    state->write_register(state->ctx, state->regs.regs_u16[BMP280_REG_OFFSET_CONFIG]);
}

/**
 * Gets pressure from device in mmHg (range 225..825) and temperature from device in Celsius degrees (range is -40..85)
 * @param[in] state BMP280 driver state
 * @param[out] press Measured pressure, in mmHg
 * @param[out] temp Measured temperature, in Celsius degrees
 */
void bmp280_get_measurement(bmp280_t* state, uint16_t* press, int8_t* temp) {
    int32_t t_fine = _temp_raw_to_fine(state, _get_temp_raw(state));
    int32_t p_pa = _press_raw_to_pa(state, _get_press_raw(state), t_fine);
    *press = (uint16_t)sc_mul_round_s32(p_pa, MATH_CONST_S32(0.00750062f, 28), 28);
    *temp = (int8_t)((((t_fine * 5 + 128) >> 8) + 50) / 100);
    // TODO: Optimize formula or output format to avoid division by 100
}

/**
 * Gets pressure from device in mmHg (range 225..825)
 * @param[in] state BMP280 driver state
 * @return Measured pressure, in mmHg
 */
uint16_t bmp280_get_pressure(bmp280_t* state) {
    int32_t p_pa = _press_raw_to_pa(state, _get_press_raw(state), _temp_raw_to_fine(state, _get_temp_raw(state)));
    return (uint16_t)sc_mul_round_s32(p_pa, MATH_CONST_S32(0.00750062f, 28), 28);
}

/**
 * Gets temperature from device in Celsius degrees (range -40..85)
 * @param[in] state BMP280 driver state, in Celsius degrees
 * @return Measured temperature, in Celsius degrees
 */
int8_t bmp280_get_temperature(bmp280_t* state) {
    // TODO: Optimize formula or output format to avoid division by 100
    return (int8_t)((((_temp_raw_to_fine(state, _get_temp_raw(state)) * 5 + 128) >> 8) + 50) / 100);
}

static int32_t _get_press_raw(bmp280_t* state) {
    bmp280_regs_t* regs = &state->regs.regs;
    uint16_t* regs_u16 = state->regs.regs_u16;
    uint8_t press_xlsb = state->read_register(state->ctx, regs_u16[BMP280_REG_OFFSET_PRESS_XLSB] & 0xFF00) >> 4;
    uint8_t press_lsb = state->read_register(state->ctx, regs_u16[BMP280_REG_OFFSET_PRESS_LSB] & 0xFF00);
    uint8_t press_msb = state->read_register(state->ctx, regs_u16[BMP280_REG_OFFSET_PRESS_MSB] & 0xFF00);
    regs->press_xlsb.press_xlsb = press_xlsb;
    regs->press_lsb.press_lsb = press_lsb;
    regs->press_msb.press_msb = press_msb;
    return (int32_t)((press_msb << 12) | (press_lsb << 4) | press_xlsb);
}

static int32_t _get_temp_raw(bmp280_t* state) {
    bmp280_regs_t* regs = &state->regs.regs;
    uint16_t* regs_u16 = state->regs.regs_u16;
    uint8_t temp_xlsb = (state->read_register(state->ctx, regs_u16[BMP280_REG_OFFSET_TEMP_XLSB] & 0xFF00)) >> 4;
    uint8_t temp_lsb = state->read_register(state->ctx, regs_u16[BMP280_REG_OFFSET_TEMP_LSB] & 0xFF00);
    uint8_t temp_msb = state->read_register(state->ctx, regs_u16[BMP280_REG_OFFSET_TEMP_MSB] & 0xFF00);
    regs->temp_xlsb.temp_xlsb = temp_xlsb;
    regs->temp_lsb.temp_lsb = temp_lsb;
    regs->temp_msb.temp_msb = temp_msb;
    return (int32_t)((temp_msb << 12) | (temp_lsb << 4) | temp_xlsb);
}

static int32_t _press_raw_to_pa(bmp280_t* state, int32_t p_raw, int32_t t_fine) {
    int64_t v1, v2, p;
    int64_t dig_p1 = state->dig_p1;
    int64_t dig_p2 = state->dig_p2;
    int64_t dig_p3 = state->dig_p3;
    int64_t dig_p4 = state->dig_p4;
    int64_t dig_p5 = state->dig_p5;
    int64_t dig_p6 = state->dig_p6;
    int64_t dig_p7 = state->dig_p7;
    int64_t dig_p8 = state->dig_p8;
    int64_t dig_p9 = state->dig_p9;

    /// Following conversion is according to sensor data-sheet
    v1 = (int64_t)t_fine - 128000;
    v2 = v1 * v1 * dig_p6;
    v2 = v2 + ((v1 * dig_p5) << 17);
    v2 = v2 + (dig_p4 << 35);
    v1 = ((v1 * v1 * dig_p3) >> 8) + ((v1 * dig_p2) << 12);
    v1 = ((((((int64_t)1) << 47) + v1)) * dig_p1) >> 33;
    if (v1 == 0) { return BMP280_MIN_PRESS_PA; }
    p = 1048576 - p_raw;
    p = (((p << 31) - v2) * 3125) / v1;
    v1 = (dig_p9 * (p >> 13) * (p >> 13)) >> 25;
    v2 = (dig_p8 * p) >> 19;

    return (int32_t)((((p + v1 + v2) >> 8) + (dig_p7 << 4)) >> 8);
}

static int32_t _temp_raw_to_fine(bmp280_t* state, int32_t t_raw) {
    int32_t v1, v2;
#if 0
    int32_t dig_t1 = state->dig_t1;
#else
    // TODO: Temporary hack for uncalibrated or broken sensor, remove after debug
    int32_t dig_t1 = 28000;
#endif
    int32_t dig_t2 = state->dig_t2;
    int32_t dig_t3 = state->dig_t3;

    /// Following conversion is according to sensor data-sheet
    v1 = ((((t_raw >> 3) - (dig_t1 << 1))) * dig_t2) >> 11;
    v2 = (((((t_raw >> 4) - dig_t1) * ((t_raw >> 4) - dig_t1)) >> 12) * dig_t3) >> 14;

    return v1 + v2;
}

static void _reset_registers(bmp280_regs_t* regs) {
    memset(regs, 0, sizeof(bmp280_regs_t));

    regs->temp_xlsb.rw = true;
    regs->temp_xlsb.addr = 0x7C;

    regs->temp_lsb.rw = true;
    regs->temp_lsb.addr = 0x7B;

    regs->temp_msb.rw = true;
    regs->temp_msb.addr = 0x7A;

    regs->press_xlsb.rw = true;
    regs->press_xlsb.addr = 0x79;

    regs->press_lsb.rw = true;
    regs->press_lsb.addr = 0x78;

    regs->press_msb.rw = true;
    regs->press_msb.addr = 0x77;

    regs->config.rw = false;
    regs->config.addr = 0x75;
    regs->config.t_sb = BMP280_STANDBY_TIME_0P5_MS;
    regs->config.filter = BMP280_FILTER_OFF;
    regs->config.spi3w_en = false;

    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.addr = 0x74;
    regs->ctrl_meas.osrs_t = BMP280_TEMPERATURE_OVERSAMPLING_1X;
    regs->ctrl_meas.osrs_p = BMP280_PRESSURE_OVERSAMPLING_1X;
    regs->ctrl_meas.mode = BMP280_SLEEP;

    regs->status.rw = true;
    regs->status.addr = 0x73;

    regs->reset.rw = false;
    regs->reset.addr = 0x60;

    regs->id.rw = true;
    regs->id.addr = 0x50;

    regs->dig_t1_lsb.rw = true;
    regs->dig_t1_lsb.addr = 0x08;
    regs->dig_t1_msb.rw = true;
    regs->dig_t1_lsb.addr = 0x09;
    regs->dig_t2_lsb.rw = true;
    regs->dig_t2_lsb.addr = 0x0A;
    regs->dig_t2_msb.rw = true;
    regs->dig_t2_msb.addr = 0x0B;
    regs->dig_t3_lsb.rw = true;
    regs->dig_t3_lsb.addr = 0x0C;
    regs->dig_t3_msb.rw = true;
    regs->dig_t3_msb.addr = 0x0D;

    regs->dig_p1_lsb.rw = true;
    regs->dig_p1_lsb.addr = 0x0E;
    regs->dig_p1_msb.rw = true;
    regs->dig_p1_msb.addr = 0x0F;
    regs->dig_p2_lsb.rw = true;
    regs->dig_p2_lsb.addr = 0x10;
    regs->dig_p2_msb.rw = true;
    regs->dig_p2_msb.addr = 0x11;
    regs->dig_p3_lsb.rw = true;
    regs->dig_p3_lsb.addr = 0x12;
    regs->dig_p3_msb.rw = true;
    regs->dig_p3_msb.addr = 0x13;
    regs->dig_p4_lsb.rw = true;
    regs->dig_p4_lsb.addr = 0x14;
    regs->dig_p4_msb.rw = true;
    regs->dig_p4_msb.addr = 0x15;
    regs->dig_p5_lsb.rw = true;
    regs->dig_p5_lsb.addr = 0x16;
    regs->dig_p5_msb.rw = true;
    regs->dig_p5_msb.addr = 0x17;
    regs->dig_p6_lsb.rw = true;
    regs->dig_p6_lsb.addr = 0x18;
    regs->dig_p6_msb.rw = true;
    regs->dig_p6_msb.addr = 0x19;
    regs->dig_p7_lsb.rw = true;
    regs->dig_p7_lsb.addr = 0x1A;
    regs->dig_p7_msb.rw = true;
    regs->dig_p7_msb.addr = 0x1B;
    regs->dig_p8_lsb.rw = true;
    regs->dig_p8_lsb.addr = 0x1C;
    regs->dig_p8_msb.rw = true;
    regs->dig_p8_msb.addr = 0x1D;
    regs->dig_p9_lsb.rw = true;
    regs->dig_p9_lsb.addr = 0x1E;
    regs->dig_p9_msb.rw = true;
    regs->dig_p9_msb.addr = 0x1F;

    regs->dig_pX_lsb.rw = true;
    regs->dig_pX_lsb.addr = 0x20;
    regs->dig_pX_msb.rw = true;
    regs->dig_pX_msb.addr = 0x21;
}

static void _sync_registers(bmp280_t* state) {
    bmp280_regs_t* regs = &state->regs.regs;
    uint16_t* regs_u16 = state->regs.regs_u16;

    for (int n = 0; n < BMP280_NUM_REGS; n++) {
        // If register is read-only
        if (regs_u16[n] & 0x8000) {
            uint8_t value = state->read_register(state->ctx, regs_u16[n] & 0xFF00);
            regs_u16[n] &= 0xFF00;
            regs_u16[n] |= value;
        }
    }

    state->dig_t1 = (regs->dig_t1_msb.value << 8) | regs->dig_t1_lsb.value;
    state->dig_t2 = (regs->dig_t2_msb.value << 8) | regs->dig_t2_lsb.value;
    state->dig_t3 = (regs->dig_t3_msb.value << 8) | regs->dig_t3_lsb.value;

    state->dig_p1 = (regs->dig_p1_msb.value << 8) | regs->dig_p1_lsb.value;
    state->dig_p2 = (regs->dig_p2_msb.value << 8) | regs->dig_p2_lsb.value;
    state->dig_p3 = (regs->dig_p3_msb.value << 8) | regs->dig_p3_lsb.value;
    state->dig_p4 = (regs->dig_p4_msb.value << 8) | regs->dig_p4_lsb.value;
    state->dig_p5 = (regs->dig_p5_msb.value << 8) | regs->dig_p5_lsb.value;
    state->dig_p6 = (regs->dig_p6_msb.value << 8) | regs->dig_p6_lsb.value;
    state->dig_p7 = (regs->dig_p7_msb.value << 8) | regs->dig_p7_lsb.value;
    state->dig_p8 = (regs->dig_p8_msb.value << 8) | regs->dig_p8_lsb.value;
    state->dig_p9 = (regs->dig_p9_msb.value << 8) | regs->dig_p9_lsb.value;
}
