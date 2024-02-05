/**
 * LEDs module
 */

#include "leds.h"

#include <string.h>

#include "platform.h"

/**
 * Set timer 4 period to 500ms.
 * `-1` is explained in
 * [Reference Manual page page 332](https://www.arterychip.com/download/RM/RM_AT32F435_437_EN_V2.05.pdf#page=332)
 */
#define _LED_TIMER  (TMR4)
#define _TMR_PERIOD_CLOCK (5000 - 1)
#define _TMR_FREQ_DIV (PLATFORM_CRM_TMR_FREQ_HZ / 10000 - 1)

#define _LED_RED_PIN (GPIO_PINS_13)
#define _LED_RED_GPIO (GPIOD)
#define _LED_RED_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)
#define _LED_RED_TMR_CHAN (TMR_SELECT_CHANNEL_2)
#define _LED_RED_SOURCE_TYPE (GPIO_PINS_SOURCE13)
#define _LED_RED_MUX_SEL (GPIO_MUX_2)

#define _LED_AMBER_PIN (GPIO_PINS_14)
#define _LED_AMBER_GPIO (GPIOD)
#define _LED_AMBER_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)
#define _LED_AMBER_TMR_CHAN (TMR_SELECT_CHANNEL_3)
#define _LED_AMBER_SOURCE_TYPE (GPIO_PINS_SOURCE14)
#define _LED_AMBER_MUX_SEL (GPIO_MUX_2)

#define _LED_GREEN_PIN (GPIO_PINS_15)
#define _LED_GREEN_GPIO (GPIOD)
#define _LED_GREEN_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)
#define _LED_GREEN_TMR_CHAN (TMR_SELECT_CHANNEL_4)
#define _LED_GREEN_SOURCE_TYPE (GPIO_PINS_SOURCE15)
#define _LED_GREEN_MUX_SEL (GPIO_MUX_2)

static uint16_t _pins[NUM_LEDS] = {_LED_RED_PIN, _LED_AMBER_PIN, _LED_GREEN_PIN};
static gpio_type* _gpios[NUM_LEDS] = {_LED_RED_GPIO, _LED_AMBER_GPIO, _LED_GREEN_GPIO};
static tmr_channel_select_type _tmr_chans[NUM_LEDS] = {_LED_RED_TMR_CHAN, _LED_AMBER_TMR_CHAN, _LED_GREEN_TMR_CHAN};
static gpio_mux_sel_type _mux_sels[NUM_LEDS] = {_LED_RED_MUX_SEL, _LED_AMBER_MUX_SEL, _LED_GREEN_MUX_SEL};
static crm_periph_clock_type _crm_clks[NUM_LEDS] = {_LED_RED_GPIO_CRM_CLK, _LED_AMBER_GPIO_CRM_CLK, _LED_GREEN_GPIO_CRM_CLK,};
static gpio_pins_source_type _source_types[NUM_LEDS] = {_LED_RED_SOURCE_TYPE, _LED_AMBER_SOURCE_TYPE, _LED_GREEN_SOURCE_TYPE,};

void _led_init(led_type_t led) {
    gpio_init_type gpio_init_struct;

    crm_periph_clock_enable(_crm_clks[led], TRUE);
    gpio_default_para_init(&gpio_init_struct);

    gpio_init_struct.gpio_pins = _pins[led];
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init(_gpios[led], &gpio_init_struct);
}

void _led_blink_enable(led_type_t led) {
    gpio_init_type gpio_init_struct;
    tmr_output_config_type tmr_output_struct;

    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = _pins[led];
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init(_gpios[led], &gpio_init_struct);
    gpio_pin_mux_config(_gpios[led], _source_types[led], _mux_sels[led]);

    tmr_output_default_para_init(&tmr_output_struct);
    tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_SWITCH;
    tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
    tmr_output_struct.oc_output_state = TRUE;
    tmr_output_channel_config(_LED_TIMER, _tmr_chans[led], &tmr_output_struct);
    tmr_channel_value_set(_LED_TIMER, _tmr_chans[led], _TMR_PERIOD_CLOCK);
}

void leds_init(leds_t* state) {
    crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);
    
    tmr_base_init(_LED_TIMER, _TMR_PERIOD_CLOCK, _TMR_FREQ_DIV);
    tmr_cnt_dir_set(_LED_TIMER, TMR_COUNT_UP);
    tmr_clock_source_div_set(_LED_TIMER, TMR_CLOCK_DIV1);
    tmr_overflow_event_disable(_LED_TIMER, TRUE);
    
    for (int led = 0; led < NUM_LEDS; led++) {
        leds_off(state, led);
    }

    tmr_counter_enable(_LED_TIMER, TRUE);
}

void leds_on(leds_t* state, led_type_t led) {
    _led_init(led);
    state->led_state[led] = LED_STATE_ON;
    _gpios[led]->clr = _pins[led];
}

void leds_off(leds_t* state, led_type_t led) {
    _led_init(led);
    state->led_state[led] = LED_STATE_OFF;
    _gpios[led]->scr = _pins[led];
}

void leds_toggle(leds_t* state, led_type_t led) {
    if (state->led_state[led] == LED_STATE_BLINK) return;
    _led_init(led);
    state->led_state[led] = (state->led_state[led] == LED_STATE_ON) ? LED_STATE_OFF : LED_STATE_ON;
    _gpios[led]->odt ^= _pins[led];
}

void leds_blink(leds_t* state, led_type_t led) {
    _led_blink_enable(led);
    state->led_state[led] = LED_STATE_BLINK;
}

led_state_t leds_state(leds_t* state, led_type_t led) {
    return state->led_state[led];
}
