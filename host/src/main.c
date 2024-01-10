/**
 * Main module
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "pipe_console.h"
#include "leds.h"
#include "log.h"

#define NUM_RELAYS (4)
#define LOG_LEVEL (LOG_LEVEL_DEBUG)

uint8_t relays[NUM_RELAYS] = {0};

typedef struct {
    float temperature;
    uint8_t humidity;
    uint16_t pressure;
} sensors_t;

void sensors_measure(sensors_t* p_s) {
    p_s->temperature = 25.0f;
    p_s->humidity = 33;
    p_s->pressure = 768;
}

void sensors_default(sensors_t* p_s) {
    p_s->temperature = 23.0f;
    p_s->humidity = 60;
    p_s->pressure = 700;
}

int main(void) {
    log_set_level(LOG_LEVEL);

    static leds_t leds;
    leds_init(&leds);
    leds_on(&leds, LED_GREEN);

    sensors_t s;
    sensors_default(&s);
    sensors_measure(&s);
         
    pipe_console_init();

    LOG_DEBUG("Initialization is done");

    for (;;) {
        pipe_console_run();
        usleep(10000);
    }

    pipe_console_deinit();

    return 0;
}
