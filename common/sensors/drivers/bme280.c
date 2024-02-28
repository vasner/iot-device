/**
 * BME280, Combined humidity and pressure sensor
 */

#include "bme280.h"

#include <string.h>

#include "log.h"
#include "math_functions.h"
#include "utils.h"

static void _reset_registers(bme280_regs_t* state);
static void _sync_registers(bme280_t* state);
static int32_t _get_press_raw(bme280_t* state);
static int32_t _get_temp_raw(bme280_t* state);
static int16_t _get_hum_raw(bme280_t* state);
static int32_t _press_raw_to_pa(bme280_t* state, int32_t p_raw, int32_t t_fine);
static uint8_t _hum_raw_to_perc(bme280_t* state, int16_t h_raw, int32_t t_fine);
static int32_t _temp_raw_to_fine(bme280_t* state, int32_t t_raw);

/**
 * Initializes BME280 device driver
 * @param[in,out] state BME280 driver state
 * @param[in] read_register Read register callback, set to NULL if unused
 * @param[in] write_register Write register callback, set to NULL if unused
 * @param[in] ctx External context. It is passed as parameter into handlers and callbacks functions,
 *                set to NULL if unused
 * @return `true` if initialization was successful, `false` otherwise
 */
bool bme280_init(bme280_t* state, bme280_read_reg_t read_register, bme280_write_reg_t write_register, void* ctx) {
    bme280_regs_t* regs = &state->regs.regs;

    state->read_register = read_register;
    state->write_register = write_register;
    state->ctx = ctx;
    _reset_registers(regs);

    // Software reset
    delay_ms(2);  // Start-up time
    regs->reset.rw = false;
    regs->reset.reset = BME280_RESET_CODE;
    write_register(ctx, state->regs.regs_u16[BME280_REG_OFFSET_RESET]);
    delay_ms(2);  // Start-up time

    _sync_registers(state);

    state->status = regs->id.chip_id == BME280_CHIP_ID;
    if (!state->status) { LOG_ERROR("Wrong BME280 chip id `%2X`", regs->id.chip_id); }
    return state->status;
}

bool bme280_get_status(bme280_t* state) { return state->status; }

void bme280_set_standby_time(bme280_t* state, bme280_standby_time_t time) {
    bme280_regs_t* regs = &state->regs.regs;
    regs->config.rw = false;
    regs->config.t_sb = time;
    state->write_register(state->ctx, state->regs.regs_u16[BME280_REG_OFFSET_CONFIG]);
}

void bme280_set_mode(bme280_t* state, bme280_mode_t mode) {
    bme280_regs_t* regs = &state->regs.regs;
    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.mode = mode;
    state->write_register(state->ctx, state->regs.regs_u16[BME280_REG_OFFSET_CTRL_MEAS]);
}

void bme280_set_pressure_oversampling(bme280_t* state, bme280_pressure_oversampling_t oversampling) {
    bme280_regs_t* regs = &state->regs.regs;
    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.osrs_p = oversampling;
    state->write_register(state->ctx, state->regs.regs_u16[BME280_REG_OFFSET_CTRL_MEAS]);
}

void bme280_set_temperature_oversampling(bme280_t* state, bme280_temperature_oversampling_t oversampling) {
    bme280_regs_t* regs = &state->regs.regs;
    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.osrs_t = oversampling;
    state->write_register(state->ctx, state->regs.regs_u16[BME280_REG_OFFSET_CTRL_MEAS]);
}

void bme280_set_humidity_oversampling(bme280_t* state, bme280_humidity_oversampling_t oversampling) {
    bme280_regs_t* regs = &state->regs.regs;
    regs->ctrl_hum.rw = false;
    regs->ctrl_hum.osrs_h = oversampling;
    state->write_register(state->ctx, state->regs.regs_u16[BME280_REG_OFFSET_CTRL_HUM]);
    // Since changes to `ctrl_hum` register only become effective after a write operation to `ctrl_meas`
    regs->ctrl_meas.rw = false;
    state->write_register(state->ctx, state->regs.regs_u16[BME280_REG_OFFSET_CTRL_MEAS]);
}

void bme280_set_filter(bme280_t* state, bme280_filter_t filter) {
    bme280_regs_t* regs = &state->regs.regs;
    regs->config.rw = false;
    regs->config.filter = filter;
    state->write_register(state->ctx, state->regs.regs_u16[BME280_REG_OFFSET_CONFIG]);
}

/**
 * Gets pressure in mmHg (range 225..825), temperature in Celsius degrees (range is -40..85) and
 * humidity in percentage (range 0..100) from device
 * @param[in] state BME280 driver state
 * @param[out] press Measured pressure, in mmHg
 * @param[out] temp Measured temperature, in Celsius degrees
 * @param[out] hum Measured humidity, in Percentage
 */
void bme280_get_measurement(bme280_t* state, uint16_t* press, int8_t* temp, uint8_t* hum) {
    int32_t t_fine = _temp_raw_to_fine(state, _get_temp_raw(state));
    int32_t p_pa = _press_raw_to_pa(state, _get_press_raw(state), t_fine);
    *press = (uint16_t)sc_mul_round_s32(p_pa, MATH_CONST_S32(0.00750062f, 28), 28);
    *temp = (int8_t)((((t_fine * 5 + 128) >> 8) + 50) / 100);
    *hum = _hum_raw_to_perc(state, _get_hum_raw(state), t_fine);
}

/**
 * Gets pressure from device in mmHg (range 225..825)
 * @param[in] state BME280 driver state
 * @return Measured pressure, in mmHg
 */
uint16_t bme280_get_pressure(bme280_t* state) {
    int32_t p_pa = _press_raw_to_pa(state, _get_press_raw(state), _temp_raw_to_fine(state, _get_temp_raw(state)));
    return (uint16_t)sc_mul_round_s32(p_pa, MATH_CONST_S32(0.00750062f, 28), 28);
}

/**
 * Gets temperature from device in Celsius degrees (range -40..85)
 * @param[in] state BME280 driver state
 * @return Measured temperature, in Celsius degrees
 */
int8_t bme280_get_temperature(bme280_t* state) {
    return (int8_t)((((_temp_raw_to_fine(state, _get_temp_raw(state)) * 5 + 128) >> 8) + 50) / 100);
}

/**
 * Gets humidity from device in percentage (range 0..100)
 * @param[in] state BME280 driver state
 * @return Measured temperature, in Celsius degrees
 */
uint8_t bme280_get_humidity(bme280_t* state) {
    return _hum_raw_to_perc(state, _get_hum_raw(state), _temp_raw_to_fine(state, _get_temp_raw(state)));
}

static int32_t _get_press_raw(bme280_t* state) {
    bme280_regs_t* regs = &state->regs.regs;
    uint16_t* regs_u16 = state->regs.regs_u16;
    uint8_t press_xlsb = state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_PRESS_XLSB] & 0xFF00) >> 4;
    uint8_t press_lsb = state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_PRESS_LSB] & 0xFF00);
    uint8_t press_msb = state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_PRESS_MSB] & 0xFF00);
    regs->press_xlsb.press_xlsb = press_xlsb;
    regs->press_lsb.press_lsb = press_lsb;
    regs->press_msb.press_msb = press_msb;
    return (int32_t)((press_msb << 12) | (press_lsb << 4) | press_xlsb);
}

static int32_t _get_temp_raw(bme280_t* state) {
    bme280_regs_t* regs = &state->regs.regs;
    uint16_t* regs_u16 = state->regs.regs_u16;
    uint8_t temp_xlsb = (state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_TEMP_XLSB] & 0xFF00)) >> 4;
    uint8_t temp_lsb = state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_TEMP_LSB] & 0xFF00);
    uint8_t temp_msb = state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_TEMP_MSB] & 0xFF00);
    regs->temp_xlsb.temp_xlsb = temp_xlsb;
    regs->temp_lsb.temp_lsb = temp_lsb;
    regs->temp_msb.temp_msb = temp_msb;
    return (int32_t)((temp_msb << 12) | (temp_lsb << 4) | temp_xlsb);
}

static int16_t _get_hum_raw(bme280_t* state) {
    bme280_regs_t* regs = &state->regs.regs;
    uint16_t* regs_u16 = state->regs.regs_u16;
    uint8_t hum_lsb = state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_HUM_LSB] & 0xFF00);
    uint8_t hum_msb = state->read_register(state->ctx, regs_u16[BME280_REG_OFFSET_HUM_MSB] & 0xFF00);
    regs->hum_lsb.hum_lsb = hum_lsb;
    regs->hum_msb.hum_msb = hum_msb;
    return (int16_t)((hum_msb << 8) | (hum_lsb));
}

static int32_t _press_raw_to_pa(bme280_t* state, int32_t p_raw, int32_t t_fine) {
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

    v1 = (int64_t)t_fine - 128000;
    v2 = v1 * v1 * dig_p6;
    v2 = v2 + ((v1 * dig_p5) << 17);
    v2 = v2 + (dig_p4 << 35);
    v1 = ((v1 * v1 * dig_p3) >> 8) + ((v1 * dig_p2) << 12);
    v1 = ((((((int64_t)1) << 47) + v1)) * dig_p1) >> 33;
    if (v1 == 0) { return BME280_MIN_PRESS_PA; }
    p = 1048576 - p_raw;
    p = (((p << 31) - v2) * 3125) / v1;
    v1 = (dig_p9 * (p >> 13) * (p >> 13)) >> 25;
    v2 = (dig_p8 * p) >> 19;

    return (int32_t)((((p + v1 + v2) >> 8) + (dig_p7 << 4)) >> 8);
}

static uint8_t _hum_raw_to_perc(bme280_t* state, int16_t h_raw, int32_t t_fine) {
    int32_t v1, v2, v3, v4, v5;

    int32_t dig_h1 = state->dig_h1;
    int32_t dig_h2 = state->dig_h2;
    int32_t dig_h3 = state->dig_h3;
    int32_t dig_h4 = state->dig_h4;
    int32_t dig_h5 = state->dig_h5;
    int32_t dig_h6 = state->dig_h6;

    v1 = t_fine - 76800;
    v2 = h_raw << 14;
    v3 = dig_h4 << 20;
    v4 = dig_h5 * v1;
    v5 = (((v2 - v3) - v4) + 16384) >> 15;
    v2 = (v1 * dig_h6) >> 10;
    v3 = (v1 * dig_h3) >> 12;
    v4 = ((v2 * (v3 + 32768)) >> 10) + 2097152;
    v2 = ((v4 * dig_h2) + 8192) >> 14;
    v3 = v5 * v2;
    v4 = ((v3 >> 15) * (v3 >> 15)) >> 7;
    v5 = v3 - ((v4 * dig_h1) >> 4);
    v5 = (v5 < 0) ? 0 : v5;
    v5 = (v5 > 419430400) ? 419430400 : v5;

    return (uint8_t)(v5 >> 22);
}

static int32_t _temp_raw_to_fine(bme280_t* state, int32_t t_raw) {
    int32_t v1, v2;
    int32_t dig_t1 = state->dig_t1;
    int32_t dig_t2 = state->dig_t2;
    int32_t dig_t3 = state->dig_t3;

    v1 = (((t_raw >> 3) - (dig_t1 << 1)) * dig_t2) >> 11;
    v2 = (t_raw >> 4) - dig_t1;
    v2 = (((v2 * v2) >> 12) * dig_t3) >> 14;

    return v1 + v2;
}

static void _reset_registers(bme280_regs_t* regs) {
    memset(regs, 0, sizeof(bme280_regs_t));

    regs->hum_lsb.rw = true;
    regs->hum_lsb.addr = 0x7E;

    regs->hum_msb.rw = true;
    regs->hum_msb.addr = 0x7D;

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
    regs->config.t_sb = BME280_STANDBY_TIME_0P5_MS;
    regs->config.filter = BME280_FILTER_OFF;
    regs->config.spi3w_en = false;

    regs->ctrl_meas.rw = false;
    regs->ctrl_meas.addr = 0x74;
    regs->ctrl_meas.osrs_t = BME280_TEMPERATURE_OVERSAMPLING_1X;
    regs->ctrl_meas.osrs_p = BME280_PRESSURE_OVERSAMPLING_1X;
    regs->ctrl_meas.mode = BME280_SLEEP;

    regs->status.rw = true;
    regs->status.addr = 0x73;

    regs->ctrl_hum.rw = false;
    regs->ctrl_hum.addr = 0x72;
    regs->ctrl_hum.osrs_h = BME280_HUMIDITY_OVERSAMPLING_1X;

    regs->reset.rw = false;
    regs->reset.addr = 0x60;

    regs->id.rw = true;
    regs->id.addr = 0x50;

    regs->dig_t1_lsb.rw = true;
    regs->dig_t1_lsb.addr = 0x08;
    regs->dig_t1_msb.rw = true;
    regs->dig_t1_msb.addr = 0x09;
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

    regs->dig_h1.rw = true;
    regs->dig_h1.addr = 0x21;

    regs->dig_h2_lsb.rw = true;
    regs->dig_h2_lsb.addr = 0x61;

    regs->dig_h2_msb.rw = true;
    regs->dig_h2_msb.addr = 0x62;

    regs->dig_h3.rw = true;
    regs->dig_h3.addr = 0x63;

    regs->dig_h4_msb.rw = true;
    regs->dig_h4_msb.addr = 0x64;

    regs->dig_h4_h5_lsb.rw = true;
    regs->dig_h4_h5_lsb.addr = 0x65;

    regs->dig_h5_msb.rw = true;
    regs->dig_h5_msb.addr = 0x66;

    regs->dig_h6.rw = true;
    regs->dig_h6.addr = 0x67;
}

static void _sync_registers(bme280_t* state) {
    bme280_regs_t* regs = &state->regs.regs;
    uint16_t* regs_u16 = state->regs.regs_u16;

    for (int n = 0; n < BME280_NUM_REGS; n++) {
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

    state->dig_h1 = regs->dig_h1.value;
    state->dig_h2 = (regs->dig_h2_msb.value << 8) | regs->dig_h2_lsb.value;
    state->dig_h3 = regs->dig_h3.value;
    // TODO: Should we perform sign extend for `dig_h4` and `dig_h5`?
    state->dig_h4 = (regs->dig_h4_msb.value << 4) | (regs->dig_h4_h5_lsb.value & 0xF);
    state->dig_h5 = (regs->dig_h5_msb.value << 4) | ((regs->dig_h4_h5_lsb.value >> 4) & 0xF);
    state->dig_h6 = regs->dig_h6.value;
}
