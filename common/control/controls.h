/**
 * Controls module
 */
#ifndef CONTROLS_H
#define CONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "leds.h"
#include "relays.h"

void controls_to_json(leds_t* leds, char* json);

#ifdef __cplusplus
}
#endif

#endif  // CONTROLS_H
