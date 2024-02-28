/**
 * Tests for BME280, Combined humidity and pressure sensor
 */

#include "sensors.h"

#include "bme280.h"
#include "catch.hpp"

extern bme280_t bme280;
extern sensors_t sensors;

TEST_CASE("bme280_temperature", "[sensors][bme280][hw]") {
    int8_t temp = bme280_get_temperature(&bme280);
    REQUIRE(temp == 25);
}

TEST_CASE("bme280_pressure", "[sensors][bme280][hw]") {
    uint16_t press = bme280_get_pressure(&bme280);
    REQUIRE(press == 754);
}

TEST_CASE("bme280_measurement", "[sensors][bme280][hw]") {
    REQUIRE(bme280_get_status(&bme280));
    int8_t temp = 0;
    uint16_t press = 0;
    uint8_t hum;
    bme280_get_measurement(&bme280, &press, &temp, &hum);
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
