/**
 * BME280, Combined humidity and pressure sensor
 */
#ifndef BME280_H
#define BME280_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define BME280_CHIP_ID (0x60)

#define BME280_NUM_REGS (46)
#define BME280_RESET_CODE (0xB6)
#define BME280_MIN_PRESS_PA (30000)

typedef void (*bme280_write_reg_t)(void* ctx, uint16_t reg);
typedef uint8_t (*bme280_read_reg_t)(void* ctx, uint16_t reg);

typedef enum {
    BME280_REG_OFFSET_HUM_LSB = 0,
    BME280_REG_OFFSET_HUM_MSB,
    BME280_REG_OFFSET_TEMP_XLSB,
    BME280_REG_OFFSET_TEMP_LSB,
    BME280_REG_OFFSET_TEMP_MSB,
    BME280_REG_OFFSET_PRESS_XLSB,
    BME280_REG_OFFSET_PRESS_LSB,
    BME280_REG_OFFSET_PRESS_MSB,
    BME280_REG_OFFSET_CONFIG,
    BME280_REG_OFFSET_CTRL_MEAS,
    BME280_REG_OFFSET_STATUS,
    BME280_REG_OFFSET_CTRL_HUM,
    BME280_REG_OFFSET_RESET,
    BME280_REG_OFFSET_ID,
} bme280_reg_offset_t;

typedef enum {
    BME280_PRESSURE_OVERSAMPLING_1X = 0,
    BME280_PRESSURE_OVERSAMPLING_2X,
    BME280_PRESSURE_OVERSAMPLING_4X,
    BME280_PRESSURE_OVERSAMPLING_8X,
    BME280_PRESSURE_OVERSAMPLING_16X,
} bme280_pressure_oversampling_t;

typedef enum {
    BME280_TEMPERATURE_OVERSAMPLING_1X = 0,
    BME280_TEMPERATURE_OVERSAMPLING_2X,
    BME280_TEMPERATURE_OVERSAMPLING_4X,
    BME280_TEMPERATURE_OVERSAMPLING_8X,
    BME280_TEMPERATURE_OVERSAMPLING_16X,
} bme280_temperature_oversampling_t;

typedef enum {
    BME280_HUMIDITY_OVERSAMPLING_1X = 0,
    BME280_HUMIDITY_OVERSAMPLING_2X,
    BME280_HUMIDITY_OVERSAMPLING_4X,
    BME280_HUMIDITY_OVERSAMPLING_8X,
    BME280_HUMIDITY_OVERSAMPLING_16X,
} bme280_humidity_oversampling_t;

typedef enum {
    BME280_SLEEP = 0,
    BME280_FORCED = 1,
    BME280_NORMAL = 3,
} bme280_mode_t;

typedef enum {
    BME280_STANDBY_TIME_0P5_MS = 0,
    BME280_STANDBY_TIME_62P5_MS,
    BME280_STANDBY_TIME_125_MS,
    BME280_STANDBY_TIME_250_MS,
    BME280_STANDBY_TIME_500_MS,
    BME280_STANDBY_TIME_1000_MS,
    BME280_STANDBY_TIME_10_MS,
    BME280_STANDBY_TIME_20_MS,
} bme280_standby_time_t;

typedef enum {
    BME280_FILTER_OFF = 0,
    BME280_FILTER_2X,
    BME280_FILTER_4X,
    BME280_FILTER_8X,
    BME280_FILTER_16X,
} bme280_filter_t;

typedef struct {
    uint16_t hum_lsb: 8;  // Contains the MSB part uh[15:8] of the raw humidity measurement output data
    uint16_t addr: 7;     // Register 7-bit address
    uint16_t rw: 1;       // Read/Write direction
} bme280_hum_lsb_t;

typedef struct {
    uint16_t hum_msb: 8;  // Contains the LSB part uh[7:0] of the raw humidity measurement output data
    uint16_t addr: 7;     // Register 7-bit address
    uint16_t rw: 1;       // Read/Write direction
} bme280_hum_msb_t;

typedef struct {
    uint16_t reserved_b3_to_b0: 4;  // Reserved. Set to 0
    uint16_t temp_xlsb: 4;          // Contains the XLSB part ut[3:0] of the raw temperature measurement output data
    uint16_t addr: 7;               // Register 7-bit address
    uint16_t rw: 1;                 // Read/Write direction
} bme280_temp_xlsb_t;

typedef struct {
    uint16_t temp_lsb: 8;  // Contains the LSB part ut[11:4] of the raw temperature measurement output data
    uint16_t addr: 7;      // Register 7-bit address
    uint16_t rw: 1;        // Read/Write direction
} bme280_temp_lsb_t;

typedef struct {
    uint16_t temp_msb: 8;  // Contains the MSB part ut[19:12] of the raw temperature measurement output data
    uint16_t addr: 7;      // Register 7-bit address
    uint16_t rw: 1;        // Read/Write direction
} bme280_temp_msb_t;

typedef struct {
    uint16_t reserved_b3_to_b0: 4;  // Reserved. Set to 0
    uint16_t press_xlsb: 4;         // Contains the XLSB part up[3:0] of the raw pressure measurement output data
    uint16_t addr: 7;               // Register 7-bit address
    uint16_t rw: 1;                 // Read/Write direction
} bme280_press_xlsb_t;

typedef struct {
    uint16_t press_lsb: 8;  // Contains the LSB part up[11:4] of the raw pressure measurement output data
    uint16_t addr: 7;       // Register 7-bit address
    uint16_t rw: 1;         // Read/Write direction
} bme280_press_lsb_t;

typedef struct {
    uint16_t press_msb: 8;  // Contains the MSB part up[19:12] of the raw pressure measurement output data
    uint16_t addr: 7;       // Register 7-bit address
    uint16_t rw: 1;         // Read/Write direction
} bme280_press_msb_t;

typedef struct {
    uint16_t spi3w_en: 1;     // Enables 3-wire SPI interface when set to `1`
    uint16_t reserved_b1: 1;  // Reserved. Set to 0
    uint16_t filter: 3;       // Controls the time constant of the IIR filter
    uint16_t t_sb: 3;         // Controls inactive duration Tstandby in normal mode
    uint16_t addr: 7;         // Register 7-bit address
    uint16_t rw: 1;           // Read/Write direction
} bme280_config_t;

typedef struct {
    uint16_t mode: 2;    // Controls the power mode of the device
    uint16_t osrs_p: 3;  // Controls oversampling of pressure data
    uint16_t osrs_t: 3;  // Controls oversampling of temperature data
    uint16_t addr: 7;    // Register 7-bit address
    uint16_t rw: 1;      // Read/Write direction
} bme280_ctrl_meas_t;

typedef struct {
    uint16_t osrs_h: 3;             // Controls oversampling of humidity data
    uint16_t reserved_b7_to_b3: 5;  // Reserved. Set to 0
    uint16_t addr: 7;               // Register 7-bit address
    uint16_t rw: 1;                 // Read/Write direction
} bme280_ctrl_hum_t;

typedef struct {
    // Automatically set to `1` when the NVM data are being copied to image registers and
    // back to `0` when the copying is done. The data are copied at power-on-reset
    // and before every conversion.
    uint16_t im_update: 1;

    uint16_t reserved_b2_to_b1: 2;  // Reserved. Set to 0

    // Automatically set to `1` whenever a conversion is running and back to `0` when
    // the results have been transferred to the data registers
    uint16_t measuring: 1;

    uint16_t reserved_b7_to_b4: 4;  // Reserved. Set to 0

    uint16_t addr: 7;  // Register 7-bit address
    uint16_t rw: 1;    // Read/Write direction
} bme280_status_t;

typedef struct {
    // The soft reset word reset[7:0]. If the value 0xB6 is written to the  register,
    // the device is reset using the complete power-on-reset procedure.
    // Writing other values than 0xB6 has no effect. The readout value is always 0x00
    uint16_t reset: 8;

    uint16_t addr: 7;  // Register 7-bit address
    uint16_t rw: 1;    // Read/Write direction
} bme280_reset_t;

typedef struct {
    uint16_t chip_id: 8;  // Chip ID
    uint16_t addr: 7;     // Register 7-bit address
    uint16_t rw: 1;       // Read/Write direction
} bme280_id_t;

typedef struct {
    uint16_t value: 8;  // Calibration value
    uint16_t addr: 7;   // Register 7-bit address
    uint16_t rw: 1;     // Read/Write direction
} bme280_calib_t;

typedef struct {
    bme280_hum_lsb_t hum_lsb;
    bme280_hum_msb_t hum_msb;
    bme280_temp_xlsb_t temp_xlsb;
    bme280_temp_lsb_t temp_lsb;
    bme280_temp_msb_t temp_msb;
    bme280_press_xlsb_t press_xlsb;
    bme280_press_lsb_t press_lsb;
    bme280_press_msb_t press_msb;
    bme280_config_t config;
    bme280_ctrl_meas_t ctrl_meas;
    bme280_status_t status;
    bme280_ctrl_hum_t ctrl_hum;
    bme280_reset_t reset;
    bme280_id_t id;
    bme280_calib_t dig_t1_lsb;
    bme280_calib_t dig_t1_msb;
    bme280_calib_t dig_t2_lsb;
    bme280_calib_t dig_t2_msb;
    bme280_calib_t dig_t3_lsb;
    bme280_calib_t dig_t3_msb;
    bme280_calib_t dig_p1_lsb;
    bme280_calib_t dig_p1_msb;
    bme280_calib_t dig_p2_lsb;
    bme280_calib_t dig_p2_msb;
    bme280_calib_t dig_p3_lsb;
    bme280_calib_t dig_p3_msb;
    bme280_calib_t dig_p4_lsb;
    bme280_calib_t dig_p4_msb;
    bme280_calib_t dig_p5_lsb;
    bme280_calib_t dig_p5_msb;
    bme280_calib_t dig_p6_lsb;
    bme280_calib_t dig_p6_msb;
    bme280_calib_t dig_p7_lsb;
    bme280_calib_t dig_p7_msb;
    bme280_calib_t dig_p8_lsb;
    bme280_calib_t dig_p8_msb;
    bme280_calib_t dig_p9_lsb;
    bme280_calib_t dig_p9_msb;
    bme280_calib_t dig_h1;
    bme280_calib_t dig_h2_lsb;
    bme280_calib_t dig_h2_msb;
    bme280_calib_t dig_h3;
    bme280_calib_t dig_h4_h5_lsb;
    bme280_calib_t dig_h4_msb;
    bme280_calib_t dig_h5_msb;
    bme280_calib_t dig_h6;
} bme280_regs_t;

/**
 * BME280 device driver state
 * @param regs: Registers union to have easy direct values access
 * @param ctx: External context. It is passed as parameter into handlers and callbacks
 */
typedef struct {
    bool status;

    union {
        bme280_regs_t regs;
        uint16_t regs_u16[BME280_NUM_REGS];
    } regs;

    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;

    uint8_t dig_h1;
    int16_t dig_h2;
    uint8_t dig_h3;
    int16_t dig_h4;
    int16_t dig_h5;
    int8_t dig_h6;

    bme280_read_reg_t read_register;
    bme280_write_reg_t write_register;

    void* ctx;
} bme280_t;

bool bme280_init(bme280_t* state, bme280_read_reg_t read_register, bme280_write_reg_t write_register, void* ctx);
bool bme280_get_status(bme280_t* state);
void bme280_set_standby_time(bme280_t* state, bme280_standby_time_t time);
void bme280_set_mode(bme280_t* state, bme280_mode_t mode);
void bme280_set_pressure_oversampling(bme280_t* state, bme280_pressure_oversampling_t oversampling);
void bme280_set_temperature_oversampling(bme280_t* state, bme280_temperature_oversampling_t oversampling);
void bme280_set_humidity_oversampling(bme280_t* state, bme280_humidity_oversampling_t oversampling);
void bme280_set_filter(bme280_t* state, bme280_filter_t filter);
void bme280_get_measurement(bme280_t* state, uint16_t* press, int8_t* temp, uint8_t* hum);
uint16_t bme280_get_pressure(bme280_t* state);
int8_t bme280_get_temperature(bme280_t* state);
uint8_t bme280_get_humidity(bme280_t* state);

#ifdef __cplusplus
}
#endif

#endif  // BME280_H