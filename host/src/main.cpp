/**
 * Main module
 */

#define CATCH_CONFIG_RUNNER

#include "bmp280.h"
#include "catch.hpp"
#include "leds.h"
#include "log.h"
#include "mock_functions.h"
#include "sensors.h"

#define LOG_LEVEL (LOG_LEVEL_WARN)

leds_t leds;
sensors_t sensors;
bmp280_t bmp280;

int main(int argc, char* argv[]) {
    log_set_level(LOG_LEVEL);
    leds_init(&leds);
    bmp280_init(&bmp280, tests_bmp280_read_reg, tests_bmp280_write_reg, &bmp280.regs.regs);
    sensors_init(&sensors, tests_bmp280_read_reg, tests_bmp280_write_reg, &bmp280.regs.regs);
    int result = Catch::Session().run(argc, argv);
    return result;
}
