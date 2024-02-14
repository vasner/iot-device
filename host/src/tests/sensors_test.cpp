/**
 * Tests for BMP280, Digital Pressure Sensor, Bosch Sensortec
 */

#include "sensors.h"

#include "bmp280.h"
#include "catch.hpp"

static bmp280_t _bmp280;

static void _bmp280_write_reg(void* ctx, uint16_t reg) {
    (void)ctx;
    (void)reg;
}

static uint8_t _bmp280_read_reg(void* ctx, uint16_t reg) {
    bmp280_regs_t* regs = (bmp280_regs_t*)ctx;

    // Mock real data in required registers.
    // Test data is from data-sheet
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
        return ((3024) & 0xFF);
    } else if (addr == regs->dig_p3_msb.addr) {
        return (((3024) >> 8) & 0xFF);
    } else if (addr == regs->dig_p4_lsb.addr) {
        return ((2855) & 0xFF);
    } else if (addr == regs->dig_p4_msb.addr) {
        return (((2855) >> 8) & 0xFF);
    } else if (addr == regs->dig_p5_lsb.addr) {
        return ((140) & 0xFF);
    } else if (addr == regs->dig_p5_msb.addr) {
        return (((140) >> 8) & 0xFF);
    } else if (addr == regs->dig_p6_lsb.addr) {
        return ((-7) & 0xFF);
    } else if (addr == regs->dig_p6_msb.addr) {
        return (((-7) >> 8) & 0xFF);
    } else if (addr == regs->dig_p7_lsb.addr) {
        return ((15500) & 0xFF);
    } else if (addr == regs->dig_p7_msb.addr) {
        return (((15500) >> 8) & 0xFF);
    } else if (addr == regs->dig_p8_lsb.addr) {
        return ((-14600) & 0xFF);
    } else if (addr == regs->dig_p8_msb.addr) {
        return (((-14600) >> 8) & 0xFF);
    } else if (addr == regs->dig_p9_lsb.addr) {
        return ((6000) & 0xFF);
    } else if (addr == regs->dig_p9_msb.addr) {
        return (((6000) >> 8) & 0xFF);
    }
    return 0;
}

TEST_CASE("bmp280_temperature", "[sensors][bmp280][hw]") {
    bmp280_init(&_bmp280, _bmp280_read_reg, _bmp280_write_reg, &_bmp280.regs.regs);
    int8_t temp = bmp280_get_temperature(&_bmp280);
    REQUIRE(temp == 25);
}

TEST_CASE("bmp280_pressure", "[sensors][bmp280][hw]") {
    bmp280_init(&_bmp280, _bmp280_read_reg, _bmp280_write_reg, &_bmp280.regs.regs);
    uint16_t press = bmp280_get_pressure(&_bmp280);
    REQUIRE(press == 754);
}

TEST_CASE("bmp280_measurement", "[sensors][bmp280][hw]") {
    bmp280_init(&_bmp280, _bmp280_read_reg, _bmp280_write_reg, &_bmp280.regs.regs);
    int8_t temp = 0;
    uint16_t press = 0;
    bmp280_get_measurement(&_bmp280, &press, &temp);
    REQUIRE(temp == 25);
    REQUIRE(press == 754);
}

TEST_CASE("sensors", "[sensors][hw]") {
    sensors_t sensors;
    sensors_data_t data;
    sensors_init(&sensors, _bmp280_read_reg, _bmp280_write_reg, &_bmp280.regs.regs);
    sensors_measure(&sensors, &data);
    REQUIRE(data.temperature == 25);
    REQUIRE(data.pressure == 754);
    REQUIRE(data.humidity == 0);
}
