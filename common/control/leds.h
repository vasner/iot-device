/**
 * LEDs module
 */
#ifndef LEDS_H
#define LEDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LED_RED = 0,
    LED_AMBER,
    LED_GREEN,
    NUM_LEDS, // Should always be the last
} led_type_t;

typedef enum {
    LED_STATE_ON,
    LED_STATE_OFF,
    LED_STATE_BLINK,
    NUM_LED_STATES, // Should always be the last
} led_state_t;

typedef struct {
    led_state_t led_state[NUM_LEDS];
} leds_t;

void leds_init(leds_t* state);
void leds_on(leds_t* state, led_type_t led);
void leds_off(leds_t* state, led_type_t led);
void leds_blink(leds_t* state, led_type_t led);
void leds_toggle(leds_t* state, led_type_t led);
led_state_t leds_state(leds_t* state, led_type_t led);

#ifdef __cplusplus
}
#endif

#endif // LEDS_H
