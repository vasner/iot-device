/**
 * AT-START-F437 board configuration
 */
#ifndef __AT32F435_437_BOARD_H
#define __AT32F435_437_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "at32f435_437.h"

#if !defined(AT_START_F435_V1) && !defined(AT_START_F437_V1)
#error "please select first the board at-start device used in your application (in at32f435_437_board.h file)"
#endif

typedef enum { LED2 = 0, LED3 = 1, LED4 = 2 } led_type;

#define LED_NUM 3

#if defined(AT_START_F435_V1) || defined(AT_START_F437_V1)
#define LED2_PIN (GPIO_PINS_13)
#define LED2_GPIO (GPIOD)
#define LED2_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)

#define LED3_PIN (GPIO_PINS_14)
#define LED3_GPIO (GPIOD)
#define LED3_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)

#define LED4_PIN (GPIO_PINS_15)
#define LED4_GPIO (GPIOD)
#define LED4_GPIO_CRM_CLK (CRM_GPIOD_PERIPH_CLOCK)
#endif

typedef enum { USER_BUTTON = 0, NO_BUTTON = 1 } button_type;

#define USER_BUTTON_PIN GPIO_PINS_0
#define USER_BUTTON_PORT GPIOA
#define USER_BUTTON_CRM_CLK CRM_GPIOA_PERIPH_CLOCK

void at32_board_init(void);

void at32_led_init(led_type led);
void at32_led_on(led_type led);
void at32_led_off(led_type led);
void at32_led_toggle(led_type led);

void at32_button_init(void);
button_type at32_button_press(void);
uint8_t at32_button_state(void);

void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_sec(uint16_t sec);

#ifdef __cplusplus
}
#endif

#endif
