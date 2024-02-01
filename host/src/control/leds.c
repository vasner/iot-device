/**
 * LEDs module
 */

#include "leds.h"

#include <string.h>

void leds_init(leds_t* state) {
    for (int led = 0; led < NUM_LEDS; led++) {
        state->led_state[led] = false;
    }
}

void leds_on(leds_t* state, led_type_t led) {
    state->led_state[led] = true;
}

void leds_off(leds_t* state, led_type_t led) {
    state->led_state[led] = false;
}

void leds_toggle(leds_t* state, led_type_t led) {
    state->led_state[led] = !state->led_state[led];
}

bool leds_state(leds_t* state, led_type_t led) {
    return state->led_state[led];
}
