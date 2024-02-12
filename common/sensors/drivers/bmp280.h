/**
 * BMP280, Digital Pressure Sensor, Bosch Sensortec
 */
#ifndef BMP280_H
#define BMP280_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define BMP280_CHIP_ID (0x58)

#define BMP280_CALIB_ADDR (0x25)
#define BMP280_NUM_CALIB_REGS (26)
#define BMP280_NUM_CONTROL_REGS (11)
#define BMP280_NUM_REGS (37)

#define BMP280_RESET_CODE (0xB6)

#define BMP280_MIN_PRESS_PA (30000)

typedef void (*bmp280_write_reg_t)(void* ctx, uint16_t reg);
typedef uint8_t (*bmp280_read_reg_t)(void* ctx, uint16_t reg);

typedef enum {
    BMP280_REG_OFFSET_TEMP_XLSB = 0,
    BMP280_REG_OFFSET_TEMP_LSB,
    BMP280_REG_OFFSET_TEMP_MSB,
    BMP280_REG_OFFSET_PRESS_XLSB,
    BMP280_REG_OFFSET_PRESS_LSB,
    BMP280_REG_OFFSET_PRESS_MSB,
    BMP280_REG_OFFSET_CONFIG,
    BMP280_REG_OFFSET_CTRL_MEAS,
    BMP280_REG_OFFSET_STATUS,
    BMP280_REG_OFFSET_RESET,
    BMP280_REG_OFFSET_ID,
} bmp280_reg_offset_t;

typedef enum {
    BMP280_PRESSURE_OVERSAMPLING_1X = 0,
    BMP280_PRESSURE_OVERSAMPLING_2X,
    BMP280_PRESSURE_OVERSAMPLING_4X,
    BMP280_PRESSURE_OVERSAMPLING_8X,
    BMP280_PRESSURE_OVERSAMPLING_16X,
} bmp280_pressure_oversampling_t;

typedef enum {
    BMP280_TEMPERATURE_OVERSAMPLING_1X = 0,
    BMP280_TEMPERATURE_OVERSAMPLING_2X,
    BMP280_TEMPERATURE_OVERSAMPLING_4X,
    BMP280_TEMPERATURE_OVERSAMPLING_8X,
    BMP280_TEMPERATURE_OVERSAMPLING_16X,
} bmp280_temperature_oversampling_t;

typedef enum {
    BMP280_SLEEP = 0,
    BMP280_FORCED = 1,
    BMP280_NORMAL = 3,
} bmp280_mode_t;

typedef enum {
    BMP280_STANDBY_TIME_0P5_MS = 0,
    BMP280_STANDBY_TIME_62P5_MS,
    BMP280_STANDBY_TIME_125_MS,
    BMP280_STANDBY_TIME_250_MS,
    BMP280_STANDBY_TIME_500_MS,
    BMP280_STANDBY_TIME_1000_MS,
    BMP280_STANDBY_TIME_2000_MS,
    BMP280_STANDBY_TIME_4000_MS,
} bmp280_standby_time_t;

typedef enum {
    BMP280_FILTER_OFF = 0,
    BMP280_FILTER_2X,
    BMP280_FILTER_4X,
    BMP280_FILTER_8X,
    BMP280_FILTER_16X,
} bmp280_filter_t;

typedef struct {
    uint16_t reserved_b4_to_b0: 4;  // Reserved. Set to 0
    uint16_t temp_xlsb: 4;          // Contains the XLSB part ut[3:0] of the raw temperature measurement output data
    uint16_t addr: 7;               // Register 7-bit address
    uint16_t rw: 1;                 // Read/Write direction
} bmp280_temp_xlsb_t;

typedef struct {
    uint16_t temp_lsb: 8;  // Contains the LSB part ut[11:4] of the raw temperature measurement output data
    uint16_t addr: 7;      // Register 7-bit address
    uint16_t rw: 1;        // Read/Write direction
} bmp280_temp_lsb_t;

typedef struct {
    uint16_t temp_msb: 8;  // Contains the MSB part ut[19:12] of the raw temperature measurement output data
    uint16_t addr: 7;      // Register 7-bit address
    uint16_t rw: 1;        // Read/Write direction
} bmp280_temp_msb_t;

typedef struct {
    uint16_t reserved_b4_to_b0: 4;  // Reserved. Set to 0
    uint16_t press_xlsb: 4;         // Contains the XLSB part up[3:0] of the raw pressure measurement output data
    uint16_t addr: 7;               // Register 7-bit address
    uint16_t rw: 1;                 // Read/Write direction
} bmp280_press_xlsb_t;

typedef struct {
    uint16_t press_lsb: 8;  // Contains the LSB part up[11:4] of the raw pressure measurement output data
    uint16_t addr: 7;       // Register 7-bit address
    uint16_t rw: 1;         // Read/Write direction
} bmp280_press_lsb_t;

typedef struct {
    uint16_t press_msb: 8;  // Contains the MSB part up[19:12] of the raw pressure measurement output data
    uint16_t addr: 7;       // Register 7-bit address
    uint16_t rw: 1;         // Read/Write direction
} bmp280_press_msb_t;

typedef struct {
    uint16_t spi3w_en: 1;     // Enables 3-wire SPI interface when set to `1`
    uint16_t reserved_b1: 1;  // Reserved. Set to 0
    uint16_t filter: 3;       // Controls the time constant of the IIR filter
    uint16_t t_sb: 3;         // Controls inactive duration Tstandby in normal mode
    uint16_t addr: 7;         // Register 7-bit address
    uint16_t rw: 1;           // Read/Write direction
} bmp280_config_t;

typedef struct {
    uint16_t mode: 2;    // Controls the power mode of the device
    uint16_t osrs_p: 3;  // Controls oversampling of pressure data
    uint16_t osrs_t: 3;  // Controls oversampling of temperature data
    uint16_t addr: 7;    // Register 7-bit address
    uint16_t rw: 1;      // Read/Write direction
} bmp280_ctrl_meas_t;

typedef struct {
    // Automatically set to `1` when the NVM data are being copied to image registers and
    // back to `0` when the copying is done. The data are copied at power-on-reset
    // and before every conversion.
    uint16_t im_update: 1;

    uint16_t reserved_b2_to_b1: 4;  // Reserved. Set to 0

    // Automatically set to `1` whenever a conversion is running and back to `0` when
    // the results have been transferred to the data registers
    uint16_t measuring: 1;

    uint16_t reserved_b7_to_b4: 4;  // Reserved. Set to 0

    uint16_t addr: 7;  // Register 7-bit address
    uint16_t rw: 1;    // Read/Write direction
} bmp280_status_t;

typedef struct {
    // The soft reset word reset[7:0]. If the value 0xB6 is written to the  register,
    // the device is reset using the complete power-on-reset procedure.
    // Writing other values than 0xB6 has no effect. The readout value is always 0x00
    uint16_t reset: 8;

    uint16_t addr: 7;  // Register 7-bit address
    uint16_t rw: 1;    // Read/Write direction
} bmp280_reset_t;

typedef struct {
    uint16_t chip_id: 8;  // Chip ID
    uint16_t addr: 7;     // Register 7-bit address
    uint16_t rw: 1;       // Read/Write direction
} bmp280_id_t;

typedef struct {
    uint16_t value: 8;  // Calibration value
    uint16_t addr: 7;   // Register 7-bit address
    uint16_t rw: 1;     // Read/Write direction
} bmp280_calib_t;

typedef struct {
    bmp280_temp_xlsb_t temp_xlsb;
    bmp280_temp_lsb_t temp_lsb;
    bmp280_temp_msb_t temp_msb;
    bmp280_press_xlsb_t press_xlsb;
    bmp280_press_lsb_t press_lsb;
    bmp280_press_msb_t press_msb;
    bmp280_config_t config;
    bmp280_ctrl_meas_t ctrl_meas;
    bmp280_status_t status;
    bmp280_reset_t reset;
    bmp280_id_t id;
    bmp280_calib_t dig_t1_lsb;
    bmp280_calib_t dig_t1_msb;
    bmp280_calib_t dig_t2_lsb;
    bmp280_calib_t dig_t2_msb;
    bmp280_calib_t dig_t3_lsb;
    bmp280_calib_t dig_t3_msb;
    bmp280_calib_t dig_p1_lsb;
    bmp280_calib_t dig_p1_msb;
    bmp280_calib_t dig_p2_lsb;
    bmp280_calib_t dig_p2_msb;
    bmp280_calib_t dig_p3_lsb;
    bmp280_calib_t dig_p3_msb;
    bmp280_calib_t dig_p4_lsb;
    bmp280_calib_t dig_p4_msb;
    bmp280_calib_t dig_p5_lsb;
    bmp280_calib_t dig_p5_msb;
    bmp280_calib_t dig_p6_lsb;
    bmp280_calib_t dig_p6_msb;
    bmp280_calib_t dig_p7_lsb;
    bmp280_calib_t dig_p7_msb;
    bmp280_calib_t dig_p8_lsb;
    bmp280_calib_t dig_p8_msb;
    bmp280_calib_t dig_p9_lsb;
    bmp280_calib_t dig_p9_msb;
    bmp280_calib_t dig_pX_lsb;
    bmp280_calib_t dig_pX_msb;
} bmp280_regs_t;

/**
 * BMP280 device driver state
 * @param regs: Registers union to have easy direct values access
 * @param ctx: External context. It is passed as parameter into handlers and callbacks
 */
typedef struct {
    union {
        bmp280_regs_t regs;
        uint16_t regs_u16[BMP280_NUM_REGS];
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

    bmp280_read_reg_t read_register;
    bmp280_write_reg_t write_register;

    void* ctx;
} bmp280_t;

void bmp280_init(bmp280_t* state, bmp280_read_reg_t read_register, bmp280_write_reg_t write_register, void* ctx);
void bmp280_set_standby_time(bmp280_t* state, bmp280_standby_time_t time);
void bmp280_set_mode(bmp280_t* state, bmp280_mode_t mode);
void bmp280_set_pressure_oversampling(bmp280_t* state, bmp280_pressure_oversampling_t oversampling);
void bmp280_set_temperature_oversampling(bmp280_t* state, bmp280_temperature_oversampling_t oversampling);
void bmp280_set_filter(bmp280_t* state, bmp280_filter_t filter);
void bmp280_get_measurement(bmp280_t* state, uint16_t* press, int8_t* temp);
uint16_t bmp280_get_pressure(bmp280_t* state);
int8_t bmp280_get_temperature(bmp280_t* state);

#ifdef __cplusplus
}
#endif

#endif  // BMP280_H