/**
 * Main entry point
 */

#include <string.h>

#include "at32f435_437_tmr.h"
#include "at32f435_437_gpio.h"
#include "at32f435_437_misc.h"
#include "usb_conf.h"
#include "vcom.h"
#include "vcom_console.h"
#include "utils.h"
#include "leds.h"

/**
 * Set timer 1 period to 50ms.
 * Set timer 4 period to 500ms.
 * `-1` is explained in
 * [Reference Manual page page 332](https://www.arterychip.com/download/RM/RM_AT32F435_437_EN_V2.05.pdf#page=332)
 */
#define TMR1_PERIOD_CLOCK (500 - 1)
#define TMR1_FREQ_DIV (PLATFORM_CRM_TMR_FREQ_HZ / 10000 - 1)

#define TMR4_PERIOD_CLOCK (5000 - 1)
#define TMR4_FREQ_DIV (PLATFORM_CRM_TMR_FREQ_HZ / 10000 - 1)

static leds_t _leds;

int main(void) {
    gpio_init_type gpio_init_struct;
    tmr_output_config_type tmr_output_struct;

    platform_init_system_clock();
    delay_init();
    leds_init(&_leds);
    vcom_init();

    leds_on(&_leds, LED_GREEN);

    nvic_priority_group_config(NVIC_PRIORITY_GROUP_1);
    nvic_irq_enable(TMR1_CH_IRQn, 1, 0);
    nvic_irq_enable(TMR4_GLOBAL_IRQn, 1, 0);

    crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

    // Configure LED3 as timer 4 channel 3 output
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = GPIO_PINS_14;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init(GPIOD, &gpio_init_struct);
    gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE14, GPIO_MUX_2);

    tmr_base_init(TMR4, TMR4_PERIOD_CLOCK, TMR4_FREQ_DIV);
    tmr_cnt_dir_set(TMR4, TMR_COUNT_UP);
    tmr_clock_source_div_set(TMR4, TMR_CLOCK_DIV1);

    tmr_overflow_event_disable(TMR4, TRUE);
    tmr_output_default_para_init(&tmr_output_struct);
    tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_SWITCH;
    tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
    tmr_output_struct.oc_output_state = TRUE;
    tmr_output_channel_config(TMR4, TMR_SELECT_CHANNEL_3, &tmr_output_struct);
    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_3, TMR4_PERIOD_CLOCK);
    tmr_counter_enable(TMR4, TRUE);

    // Configure PB14 as timer 1 channel 2 output
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = GPIO_PINS_14;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init(GPIOB, &gpio_init_struct);
    gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE14, GPIO_MUX_1);

    tmr_base_init(TMR1, TMR1_PERIOD_CLOCK, TMR1_FREQ_DIV);
    tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
    tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);

    tmr_overflow_event_disable(TMR1, TRUE);
    tmr_output_default_para_init(&tmr_output_struct);
    tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_SWITCH;
    tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_LOW;
    tmr_output_struct.occ_output_state = TRUE;
    tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_2, &tmr_output_struct);
    tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_2, TMR1_PERIOD_CLOCK);
    tmr_output_enable(TMR1, TRUE);
    tmr_counter_enable(TMR1, TRUE);

    // Enable interrupts
    tmr_interrupt_enable(TMR4, TMR_C4_INT, TRUE);
    tmr_interrupt_enable(TMR1, TMR_C4_INT, TRUE);

    vcom_console_init();

    while (1) {
        // Software LED2 toggle
        leds_toggle(&_leds, LED_RED);
        delay_ms(100);
        vcom_console_run();
    }

    return 0;
}

void TMR1_CH_IRQHandler(void) {
    if (tmr_flag_get(TMR1, TMR_C4_FLAG) != RESET) {
        tmr_flag_clear(TMR1, TMR_C4_FLAG);
    }
}

void TMR4_GLOBAL_IRQHandler(void) {
    if (tmr_flag_get(TMR4, TMR_C4_FLAG) != RESET) {
        tmr_flag_clear(TMR4, TMR_C4_FLAG);
        leds_toggle(&_leds, LED_GREEN);
    }
}
