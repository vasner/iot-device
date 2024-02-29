/**
 * Mock functions
 */

#include "mock_functions.h"

#include "bme280.h"

void tests_bme280_write_reg(void* ctx, uint16_t reg) {
    (void)ctx;
    (void)reg;
}

uint8_t tests_bme280_read_reg(void* ctx, uint16_t reg) {
    bme280_regs_t* regs = (bme280_regs_t*)ctx;

    // Mock real data in required registers.
    // Test data is from data-sheet
    uint8_t addr = (reg >> 8) & 0x7F;
    if (addr == regs->id.addr) {
        return BME280_CHIP_ID;
    } else if (addr == regs->hum_lsb.addr) {
        return (25307 & 0xFF);
    } else if (addr == regs->hum_msb.addr) {
        return ((25307 >> 8) & 0xFF);
    } else if (addr == regs->temp_xlsb.addr) {
        return (519888 & 0xF) << 4;
    } else if (addr == regs->temp_lsb.addr) {
        return ((519888 >> 4) & 0xFF);
    } else if (addr == regs->temp_msb.addr) {
        return ((519888 >> 12) & 0xFF);
    } else if (addr == regs->dig_t1_lsb.addr) {
        return (27504 & 0xFF);
    } else if (addr == regs->dig_t1_msb.addr) {
        return ((27504 >> 8) & 0xFF);
    } else if (addr == regs->dig_t2_lsb.addr) {
        return (26435 & 0xFF);
    } else if (addr == regs->dig_t2_msb.addr) {
        return ((26435 >> 8) & 0xFF);
    } else if (addr == regs->dig_t3_lsb.addr) {
        return ((-1000) & 0xFF);
    } else if (addr == regs->dig_t3_msb.addr) {
        return (((-1000) >> 8) & 0xFF);
    } else if (addr == regs->press_xlsb.addr) {
        return (415148 & 0xF) << 4;
    } else if (addr == regs->press_lsb.addr) {
        return ((415148 >> 4) & 0xFF);
    } else if (addr == regs->press_msb.addr) {
        return ((415148 >> 12) & 0xFF);
    } else if (addr == regs->dig_p1_lsb.addr) {
        return ((36477) & 0xFF);
    } else if (addr == regs->dig_p1_msb.addr) {
        return (((36477) >> 8) & 0xFF);
    } else if (addr == regs->dig_p2_lsb.addr) {
        return ((-10685) & 0xFF);
    } else if (addr == regs->dig_p2_msb.addr) {
        return (((-10685) >> 8) & 0xFF);
    } else if (addr == regs->dig_p3_lsb.addr) {
        return (3024 & 0xFF);
    } else if (addr == regs->dig_p3_msb.addr) {
        return ((3024 >> 8) & 0xFF);
    } else if (addr == regs->dig_p4_lsb.addr) {
        return (2855 & 0xFF);
    } else if (addr == regs->dig_p4_msb.addr) {
        return ((2855 >> 8) & 0xFF);
    } else if (addr == regs->dig_p5_lsb.addr) {
        return (140 & 0xFF);
    } else if (addr == regs->dig_p5_msb.addr) {
        return ((140 >> 8) & 0xFF);
    } else if (addr == regs->dig_p6_lsb.addr) {
        return ((-7) & 0xFF);
    } else if (addr == regs->dig_p6_msb.addr) {
        return (((-7) >> 8) & 0xFF);
    } else if (addr == regs->dig_p7_lsb.addr) {
        return (15500 & 0xFF);
    } else if (addr == regs->dig_p7_msb.addr) {
        return ((15500 >> 8) & 0xFF);
    } else if (addr == regs->dig_p8_lsb.addr) {
        return ((-14600) & 0xFF);
    } else if (addr == regs->dig_p8_msb.addr) {
        return (((-14600) >> 8) & 0xFF);
    } else if (addr == regs->dig_p9_lsb.addr) {
        return (6000 & 0xFF);
    } else if (addr == regs->dig_p9_msb.addr) {
        return ((6000 >> 8) & 0xFF);
    } else if (addr == regs->dig_h1.addr) {
        return 75;
    } else if (addr == regs->dig_h2_lsb.addr) {
        return (384 & 0xFF);
    } else if (addr == regs->dig_h2_msb.addr) {
        return ((384 >> 8) & 0xFF);
    } else if (addr == regs->dig_h3.addr) {
        return 0;
    } else if (addr == regs->dig_h4_h5_lsb.addr) {
        return (270 & 0xF) | ((50 & 0xF) << 4);
    } else if (addr == regs->dig_h4_msb.addr) {
        return ((270 >> 4) & 0xFF);
    } else if (addr == regs->dig_h5_msb.addr) {
        return ((50 >> 4) & 0xFF);
    } else if (addr == regs->dig_h6.addr) {
        return 30;
    }

    return 0;
}
