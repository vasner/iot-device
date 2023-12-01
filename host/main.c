/**
 * Main module
 */
#include <stdint.h>
#include <stdio.h>

#define NUM_RELAYS (4)
#define NUM_LEDS (3)

uint8_t relay[NUM_RELAYS] = {0};
uint8_t led[NUM_LEDS] = {0};

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
    sensors_t s;
    sensors_default(&s);
    printf("%f, %d, %d\n", s.temperature, s.pressure, s.humidity);
    sensors_measure(&s);
    printf("%f, %d, %d\n", s.temperature, s.pressure, s.humidity);
    return 0;
}
