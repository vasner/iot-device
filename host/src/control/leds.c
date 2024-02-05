/**
 * LEDs module
 */

#include "leds.h"

#include <string.h>

void leds_init(leds_t* state) {
    for (int led = 0; led < NUM_LEDS; led++) {
        state->led_state[led] = LED_STATE_OFF;
    }
}

void leds_on(leds_t* state, led_type_t led) {
    state->led_state[led] = LED_STATE_ON;
}

void leds_off(leds_t* state, led_type_t led) {
    state->led_state[led] = LED_STATE_OFF;
}

void leds_blink(leds_t* state, led_type_t led) {
    leds_toggle(state, led);
    state->led_state[led] = LED_STATE_BLINK;
}

void leds_toggle(leds_t* state, led_type_t led) {
    if (state->led_state[led] == LED_STATE_BLINK) return;
    state->led_state[led] = (state->led_state[led] == LED_STATE_ON) ? LED_STATE_OFF : LED_STATE_ON;
}

led_state_t leds_state(leds_t* state, led_type_t led) {
    return state->led_state[led];
}
