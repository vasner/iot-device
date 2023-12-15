/**
 * Main module
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "leds.h"

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

static const char* _rx_fifo_name = "iot_rx_fifo";

int main(void) {
    static leds_t leds;
    leds_init(&leds);
    leds_on(&leds, LED_GREEN);

    sensors_t s;
    sensors_default(&s);
    sensors_measure(&s);

    // Create receiver channel FIFOs
    mkfifo(_rx_fifo_name, 0666);      

    // Read messages from input FIFO
    char tmp[256];
    for (;;) {
        int input_fifo = open(_rx_fifo_name, O_RDONLY);
        if (input_fifo < 0) return -1;
        ssize_t count = read(input_fifo, tmp, 256);
        printf("%s", tmp);
        close(input_fifo);
    }
    
    unlink(_rx_fifo_name);
         
    return 0;
}
