/**
 * Main entry point
 */

#include <string.h>

#include "leds.h"
#include "platform.h"
#include "utils.h"
#include "vcom.h"
#include "vcom_console.h"

leds_t leds;

int main(void) {
    platform_init_system_clock();
    delay_init();
    leds_init(&leds);

    leds_on(&leds, LED_AMBER);

    vcom_init();
    vcom_console_init();

    leds_blink(&leds, LED_AMBER);
    leds_on(&leds, LED_GREEN);

    while (1) { vcom_console_run(); }

    return 0;
}
