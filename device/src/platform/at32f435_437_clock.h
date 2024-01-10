/**
 * Clock configuration
 */
#ifndef __AT32F435_437_CLOCK_H
#define __AT32F435_437_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f435_437.h"

#define CRM_TMR_FREQ_HZ (288000000)

void system_clock_config(void);

#ifdef __cplusplus
}
#endif

#endif
