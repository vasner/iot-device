/**
 * Main module
 */
#include <stdint.h>
#include <stdio.h>

float temperature = 0;
uint8_t humidity = 99;
uint16_t pressure = 700;
uint8_t relay = 0xF;
uint8_t led[3] = {0, 0, 0};

int main(void) {
    int8_t b = -1;           // 0xFF
    int16_t a = (int16_t)b;  // 0xFFFF
    uint8_t a6 = (uint8_t)pressure;
    printf("%4x\n", a);
    return 0;
}
