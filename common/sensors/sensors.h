/**
 * Sensors module
 */
#ifndef SENSORS_H
#define SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    float temperature;
    uint8_t humidity;
    uint16_t pressure;
} sensors_t;

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_H
