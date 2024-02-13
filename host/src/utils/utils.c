/**
 * Utilities
 */

#include "utils.h"

#include <unistd.h>

void delay_init() {}

void delay_us(uint32_t nus) { usleep(nus); }

void delay_ms(uint16_t nms) { usleep(nms * 1000); }

void delay_sec(uint16_t sec) { sleep(sec); }
