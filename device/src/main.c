/**
 * Main entry point
 */

#include <string.h>

#include "bmp280_spi.h"
#include "leds.h"
#include "platform.h"
#include "sensors.h"
#include "utils.h"
#include "vcom.h"
#include "vcom_console.h"

leds_t leds;
sensors_t sensors;

int main(void) {
    platform_init_system_clock();
    delay_init();
    leds_init(&leds);
    leds_on(&leds, LED_AMBER);

    bmp280_spi_init();
    sensors_init(&sensors, bmp280_spi_read_reg, bmp280_spi_write_reg, NULL);

    vcom_init();
    vcom_console_init();

    leds_blink(&leds, LED_AMBER);
    leds_on(&leds, LED_GREEN);

    while (1) { vcom_console_run(); }

    return 0;
}
