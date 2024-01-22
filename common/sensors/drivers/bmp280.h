/**
 * BMP280, Digital Pressure Sensor, Bosch Sensortec
 */
#ifndef BMP280_H
#define BMP280_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void (*bmp280_write_reg_t)(void* ctx, uint16_t reg);
typedef uint8_t (*bmp280_read_reg_t)(void* ctx, uint16_t reg);

typedef struct {
    uint16_t rw: 1;                 // Read/Write direction 
    uint16_t addr: 7;               // Register 8-bit address
    uint16_t reserved_b4_to_b0: 4;  // Reserved. Set to 0
    
    // Contains the XLSB part ut[3:0] of the raw temperature measurement output data. 
    // Contents depend on pressure resolution, see Table 4.
    uint16_t temp_xlsb: 4;          
} bmp280_temp_xlsb_t;

// TODO: Add all registers

typedef struct {
    bmp280_temp_xlsb_t temp_xlsb;
} bmp280_regs_t;

/**
 * BMP280 device driver state
 * @param regs: Registers union to have easy direct values access
 * @param ctx: External context. It is passed as parameter into handlers and callbacks
 */
typedef struct {
    union {
        bmp280_regs_t regs;
        uint16_t regs_u16[36];
    } regs;

    bmp280_read_reg_t read_register;
    bmp280_write_reg_t write_register;

    void* ctx;
} bmp280_t;

void bmp280_init(bmp280_t* state, bmp280_read_reg_t read_register, bmp280_write_reg_t write_register, void* ctx);
uint16_t bmp280_get_pressure(bmp280_t* state);
uint16_t bmp280_get_temperature(bmp280_t* state);

#ifdef __cplusplus
}
#endif

#endif // BMP280_H