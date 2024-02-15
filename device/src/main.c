/**
 * Main entry point
 */

#include <string.h>

#include "bmp280_spi.h"
#include "leds.h"
#include "log.h"
#include "platform.h"
#include "sensors.h"
#include "utils.h"
#include "vcom.h"
#include "vcom_console.h"

#define LOG_LEVEL (LOG_LEVEL_DEBUG)

leds_t leds;
sensors_t sensors;

int main(void) {
    platform_init_system_clock();
    log_set_level(LOG_LEVEL);
    delay_init();
    leds_init(&leds);
    leds_on(&leds, LED_AMBER);

    // Some delay to be sure that peripherals are passed power-on-reset stage
    delay_ms(50);

    vcom_init();
    vcom_console_init();
    bmp280_spi_init();
    sensors_init(&sensors, bmp280_spi_read_reg, bmp280_spi_write_reg, NULL);

    leds_blink(&leds, LED_AMBER);
    leds_on(&leds, LED_GREEN);

    while (1) {
        vcom_console_run();
        // TODO: Turn on sleep mode here
    }

    return 0;
}
