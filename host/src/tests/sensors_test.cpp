/**
 * Tests for BMP280, Digital Pressure Sensor, Bosch Sensortec
 */

#include "sensors.h"

#include "bmp280.h"
#include "catch.hpp"

extern bmp280_t bmp280;
extern sensors_t sensors;

TEST_CASE("bmp280_temperature", "[sensors][bmp280][hw]") {
    int8_t temp = bmp280_get_temperature(&bmp280);
    REQUIRE(temp == 25);
}

TEST_CASE("bmp280_pressure", "[sensors][bmp280][hw]") {
    uint16_t press = bmp280_get_pressure(&bmp280);
    REQUIRE(press == 754);
}

TEST_CASE("bmp280_measurement", "[sensors][bmp280][hw]") {
    REQUIRE(bmp280_get_status(&bmp280));
    int8_t temp = 0;
    uint16_t press = 0;
    bmp280_get_measurement(&bmp280, &press, &temp);
    REQUIRE(temp == 25);
    REQUIRE(press == 754);
}

TEST_CASE("sensors", "[sensors][hw]") {
    sensors_data_t data;
    REQUIRE(sensors_get_status(&sensors));
    sensors_measure(&sensors, &data);
    REQUIRE(data.temperature == 25);
    REQUIRE(data.pressure == 754);
    REQUIRE(data.humidity == 0);
}
