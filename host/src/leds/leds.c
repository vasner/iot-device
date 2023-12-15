/**
 * LEDs module
 */

#include "leds.h"

#include <string.h>

void leds_init(leds_t* state) {
    memset(state->led_state, 0, sizeof(char) * NUM_LEDS);
}

void leds_on(leds_t* state, led_type_t led) {
    state->led_state[led] = 1;
}

void leds_off(leds_t* state, led_type_t led) {
    state->led_state[led] = 0;
}
