/**
 * USB device configuration
 */
#ifndef USB_CONFIG_H
#define USB_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "platform.h"

#define USE_OTG_DEVICE_MODE
#define USB_VBUS_IGNORE

// FIFO settings
#define USBD_RX_SIZE (128)
#define USBD_EP0_TX_SIZE (24)
#define USBD_EP1_TX_SIZE (20)
#define USBD_EP2_TX_SIZE (20)
#define USBD_EP3_TX_SIZE (20)
#define USBD_EP4_TX_SIZE (20)
#define USBD_EP5_TX_SIZE (20)
#define USBD_EP6_TX_SIZE (20)
#define USBD_EP7_TX_SIZE (20)

// We have to define OTG2 FIFO too, since including issue in a driver
#define USBD2_RX_SIZE (128)
#define USBD2_EP0_TX_SIZE (24)
#define USBD2_EP1_TX_SIZE (20)
#define USBD2_EP2_TX_SIZE (20)
#define USBD2_EP3_TX_SIZE (20)
#define USBD2_EP4_TX_SIZE (20)
#define USBD2_EP5_TX_SIZE (20)
#define USBD2_EP6_TX_SIZE (20)
#define USBD2_EP7_TX_SIZE (20)

void usb_delay_ms(uint32_t ms);
void usb_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif  // USB_CONFIG_H
