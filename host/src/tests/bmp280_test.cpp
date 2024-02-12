/**
 * Tests for BMP280, Digital Pressure Sensor, Bosch Sensortec
 */

#include "bmp280.h"

#include "catch.hpp"

static bmp280_t _bmp280;

static void _write_register(void* ctx, uint16_t reg) {
    (void)ctx;
    (void)reg;
}

static uint8_t _read_register(void* ctx, uint16_t reg) {
    bmp280_regs_t* regs = (bmp280_regs_t*)ctx;

    // Mock real data in required registers
    uint8_t addr = (reg >> 8) & 0x7F;
    if (addr == regs->id.addr) {
        return BMP280_CHIP_ID;
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
    }
    return 0;
}

TEST_CASE("bmp280_temperature", "[sensors][bmp280][hw]") {
    bmp280_init(&_bmp280, _read_register, _write_register, &_bmp280.regs.regs);
    int32_t temp = bmp280_get_temperature(&_bmp280);
    REQUIRE(temp == 25);
}

#if 0
TEST_CASE("bmp280_pressure", "[sensors][bmp280][hw]") {
    bmp280_init(&_bmp280, _read_register, _write_register, &_bmp280.regs.regs);
    uint16_t press = bmp280_get_pressure(&_bmp280);
    REQUIRE(press == 755);
}

TEST_CASE("bmp280_measurement", "[sensors][bmp280][hw]") {
    bmp280_init(&_bmp280, _read_register, _write_register, &_bmp280.regs.regs);
    int32_t temp = 0;
    uint16_t press = 0;
    bmp280_get_measurement(&_bmp280, &press, &temp);
    REQUIRE(temp == 25);
    REQUIRE(press == 755);
}
#endif
