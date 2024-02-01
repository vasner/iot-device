/**
 * LEDs module
 */

#include "leds.h"

#include <string.h>

#include "platform.h"

#define _LED_RED_PIN (GPIO_PINS_13)
#define _LED_RED_GPIO (GPIOD)
#define _LED_RED_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)

#define _LED_AMBER_PIN (GPIO_PINS_14)
#define _LED_AMBER_GPIO (GPIOD)
#define _LED_AMBER_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)

#define _LED_GREEN_PIN (GPIO_PINS_15)
#define _LED_GREEN_GPIO (GPIOD)
#define _LED_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)

uint16_t _pins[NUM_LEDS] = {_LED_RED_PIN, _LED_AMBER_PIN, _LED_GREEN_PIN};
gpio_type* _gpios[NUM_LEDS] = {_LED_RED_GPIO, _LED_AMBER_GPIO, _LED_GREEN_GPIO};
crm_periph_clock_type _crm_clks[NUM_LEDS] = {_LED_RED_GPIO_CRM_CLK, _LED_AMBER_GPIO_CRM_CLK, _LED_GPIO_CRM_CLK};

void _led_init(led_type_t led) {
    gpio_init_type gpio_init_struct;

    crm_periph_clock_enable(_crm_clks[led], TRUE);
    gpio_default_para_init(&gpio_init_struct);

    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = _pins[led];
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(_gpios[led], &gpio_init_struct);
}

void leds_init(leds_t* state) {
    for (int led = 0; led < NUM_LEDS; led++) {
        _led_init(led);
        leds_off(state, led);
    }
}

void leds_on(leds_t* state, led_type_t led) {
    state->led_state[led] = true;
    _gpios[led]->clr = _pins[led];
}

void leds_off(leds_t* state, led_type_t led) {
    state->led_state[led] = false;
    _gpios[led]->scr = _pins[led];
}

void leds_toggle(leds_t* state, led_type_t led) {
    state->led_state[led] = !state->led_state[led];  
    _gpios[led]->odt ^= _pins[led];
}

bool leds_state(leds_t* state, led_type_t led) {
    return state->led_state[led];
}
