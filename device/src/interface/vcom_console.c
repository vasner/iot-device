/**
 * VCOM based console interface and commands implementation
 */

#include "vcom_console.h"

#include <string.h>

#include "console.h"
#include "log.h"
#include "vcom.h"

static console_t _console;

static void _console_put_output(const char* data) {
    vcom_send((const uint8_t*)data, (uint16_t)strlen(data));
}

static uint8_t _console_get_input(char* data, uint8_t max_len) {
    (void)max_len;
    return vcom_receive((uint8_t*)data);
}

void vcom_console_init(void) {
    console_init(&_console, _console_get_input, _console_put_output);
}

void vcom_console_run(void) {
    console_run(&_console);
}
