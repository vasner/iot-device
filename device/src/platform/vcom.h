/**
 * Virtual COM port module
 */

#ifndef VCOM_H
#define VCOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "at32f435_437.h"

#define MAX_LEN_RX_MESSAGE (128)
#define MAX_LEN_TX_MESSAGE (128)

void vcom_init(void);
uint16_t vcom_receive(uint8_t* data);
error_status vcom_send(const uint8_t* data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif  // VCOM_H