/**
 * Controls module
 */

#include "controls.h"

#include <stdio.h>

void controls_to_json(leds_t* leds, char* json) {
    static char* led_state_to_str[NUM_LED_STATES] = {"on", "off", "blink"};
    static char* led_color_to_str[NUM_LEDS] = {"red", "amber", "green"};
    // TODO: Can we make loop over all leds?
    sprintf(
        json, "{\"leds\":{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\"}}", led_color_to_str[LED_RED],
        led_state_to_str[leds_state(leds, LED_RED)], led_color_to_str[LED_AMBER],
        led_state_to_str[leds_state(leds, LED_AMBER)], led_color_to_str[LED_GREEN],
        led_state_to_str[leds_state(leds, LED_GREEN)]
    );
}
