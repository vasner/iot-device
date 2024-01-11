/**
 * LEDs module
 */
#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LED_RED = 0,
    LED_AMBER,
    LED_GREEN,
    NUM_LEDS, // Should always be the last
} led_type_t;

typedef struct {
    bool led_state[NUM_LEDS];
} leds_t;

void leds_init(leds_t* state);
void leds_on(leds_t* state, led_type_t led);
void leds_off(leds_t* state, led_type_t led);
bool leds_state(leds_t* state, led_type_t led);

#endif // LEDS_H
