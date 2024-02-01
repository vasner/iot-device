/**
 * Relays module
 */
#ifndef RELAYS_H
#define RELAYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    RELAY_PUMP_19 = 0,
    RELAY_ANGHAMMARAD,
    RELAY_GLADYS,
    RELAY_SHMATA,
    NUM_RELAYS, // Should always be the last
} relay_type_t;

#ifdef __cplusplus
}
#endif

#endif // RELAYS_H
