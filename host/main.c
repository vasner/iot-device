/**
 * Main module
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum Leds {
    LED_BLUE = 0,
    LED_AMBER,
    LED_GREEN,
    NUM_LEDS, // Should always be the last
} led_t;

#define NUM_RELAYS (4)

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

uint8_t leds[NUM_LEDS] = {0};

void led_on(led_t led) {
    leds[led] = 1;
}

void led_off(led_t led) {
    leds[led] = 0;
}

int main(void) {
    led_on(LED_GREEN);

    sensors_t s;
    sensors_default(&s);
    printf("%f, %d, %d\n", s.temperature, s.pressure, s.humidity);
    sensors_measure(&s);
    printf("%f, %d, %d\n", s.temperature, s.pressure, s.humidity);
    return 0;
}
