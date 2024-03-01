/**
 * Main module
 */

#define CATCH_CONFIG_RUNNER

#include "bme280.h"
#include "catch.hpp"
#include "leds.h"
#include "log.h"
#include "mock_functions.h"
#include "sensors.h"

#define LOG_LEVEL (LOG_LEVEL_WARN)

leds_t leds;
sensors_t sensors;
bme280_t bme280;

int main(int argc, char* argv[]) {
    log_set_level(LOG_LEVEL);
    leds_init(&leds);
    bme280_init(&bme280, tests_bme280_read_reg, tests_bme280_write_reg, &bme280.regs.regs);
    sensors_init(&sensors, tests_bme280_read_reg, tests_bme280_write_reg, &bme280.regs.regs);
    int result = Catch::Session().run(argc, argv);
    return result;
}
