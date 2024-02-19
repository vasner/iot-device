/**
 * Virtual COM port module
 */

#include "vcom.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cdc_class.h"
#include "cdc_desc.h"
#include "usb_conf.h"
#include "usb_core.h"
#include "usbd_int.h"
#include "utils.h"

// VCOM send/receive timeout and timeout step, in us
#define _TIMEOUT_US (10000)
#define _TIMEOUT_STEP_US (16)

#define USB_ID (0)

#define _OTG_PIN_GPIO (GPIOA)
#define _OTG_PIN_GPIO_CLOCK (CRM_GPIOA_PERIPH_CLOCK)

#define _OTG_PIN_DP (GPIO_PINS_12)
#define _OTG_PIN_DP_SOURCE (GPIO_PINS_SOURCE12)

#define _OTG_PIN_DM (GPIO_PINS_11)
#define _OTG_PIN_DM_SOURCE (GPIO_PINS_SOURCE11)

#define _OTG_PIN_VBUS (GPIO_PINS_9)
#define _OTG_PIN_VBUS_SOURCE (GPIO_PINS_SOURCE9)

#define _OTG_PIN_ID (GPIO_PINS_10)
#define _OTG_PIN_ID_SOURCE (GPIO_PINS_SOURCE10)

#define _OTG_PIN_SOF_GPIO (GPIOA)
#define _OTG_PIN_SOF_GPIO_CLOCK (CRM_GPIOB_PERIPH_CLOCK)
#define _OTG_PIN_SOF (GPIO_PINS_8)
#define _OTG_PIN_SOF_SOURCE (GPIO_PINS_SOURCE8)

#define _OTG_PIN_MUX (GPIO_MUX_10)

static otg_core_type _otg_core_struct;
static uint8_t _tx_buffer[MAX_LEN_TX_MESSAGE];

void vcom_init(void) {
#if defined(__GNUC__) && !defined(__clang__)
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    gpio_init_type gpio_init_struct;

    // Initialize OTG pins
    crm_periph_clock_enable(_OTG_PIN_GPIO_CLOCK, TRUE);
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_pins = _OTG_PIN_DP | _OTG_PIN_DM;
    gpio_init(_OTG_PIN_GPIO, &gpio_init_struct);
    gpio_pin_mux_config(_OTG_PIN_GPIO, _OTG_PIN_DP_SOURCE, _OTG_PIN_MUX);
    gpio_pin_mux_config(_OTG_PIN_GPIO, _OTG_PIN_DM_SOURCE, _OTG_PIN_MUX);
    gpio_init_struct.gpio_pins = _OTG_PIN_VBUS;
    gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
    gpio_pin_mux_config(_OTG_PIN_GPIO, _OTG_PIN_VBUS_SOURCE, _OTG_PIN_MUX);
    gpio_init(_OTG_PIN_GPIO, &gpio_init_struct);

    // Set clock frequency CRM_TMR_FREQ_HZ / CRM_USB_DIV_6 = 288 / 6 = 48MHz
    crm_periph_clock_enable(CRM_OTGFS1_PERIPH_CLOCK, TRUE);
    crm_usb_clock_div_set(CRM_USB_DIV_6);

    nvic_irq_enable(OTGFS1_IRQn, 0, 0);
    usbd_init(&_otg_core_struct, USB_FULL_SPEED_CORE_ID, USB_ID, &cdc_class_handler, &cdc_desc_handler);
}

void OTGFS1_IRQHandler(void) { usbd_irq_handler(&_otg_core_struct); }
void usb_delay_ms(uint32_t ms) { delay_ms(ms); }
void usb_delay_us(uint32_t us) { delay_us(us); }

uint16_t vcom_receive(uint8_t* data) { return usb_vcp_get_rxdata(&_otg_core_struct.dev, data); }

error_status vcom_send(const uint8_t* data, uint16_t len) {
    if (len > MAX_LEN_RX_MESSAGE) { return ERROR; }

    error_status result = ERROR;
    uint16_t cnt_timeout = _TIMEOUT_US / _TIMEOUT_STEP_US;
    memcpy(_tx_buffer, data, len);
    do {
        if (usb_vcp_send_data(&_otg_core_struct.dev, _tx_buffer, len) == SUCCESS) {
            result = SUCCESS;
            break;
        }
        delay_us(_TIMEOUT_STEP_US);
    } while (cnt_timeout--);
    return result;
}

/// Re-targets the C library printf function to the VCOM
#if defined(__GNUC__) && !defined(__clang__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif

PUTCHAR_PROTOTYPE {
    vcom_send((uint8_t*)&ch, 1);
    return ch;
}

#if (defined(__GNUC__) && !defined(__clang__)) || (defined(__ICCARM__))
#if defined(__GNUC__) && !defined(__clang__)
int _write(int fd, char* pbuffer, int size)
#endif
{
    (void)fd;
    vcom_send((uint8_t*)pbuffer, size);
    return size;
}
#endif
