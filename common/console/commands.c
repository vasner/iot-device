/**
 * Console commands implementation
 */

#include "console.h"
#include "version.h"
#include "leds.h"

#include <string.h>

extern leds_t leds;

static void _command_version_run(const char** args, uint8_t num_args, console_put_output_t put_output);
static void _command_leds(const char** args, uint8_t num_args, console_put_output_t put_output);

command_t command_version = {
    .type = CONSOLE_COMMAND_VERSION,
    .name = "version",
    .description = "Gets device version",
    .process = _command_version_run,
};

command_t command_leds = {
    .type = CONSOLE_COMMAND_LEDS,
    .name = "leds",
    .description = "Turn on/off or return LED state. `leds <red/amber/green> ?<on/off/blink/toggle>`",
    .process = _command_leds,
};

static void _command_version_run(const char** args, uint8_t num_args, console_put_output_t put_output) {
    (void)args;
    (void)num_args;
    put_output(VERSION);
    put_output("\n");
}

static void _command_leds(const char** args, uint8_t num_args, console_put_output_t put_output) {
    if ((num_args < 1) || (num_args > 2)) {
        put_output("Wrong number of arguments\r\n");
        return;
    }

    led_type_t led = LED_RED;
    if (strcmp(args[0], "red") == 0) led = LED_RED;
    else if (strcmp(args[0], "amber") == 0) led = LED_AMBER;
    else if (strcmp(args[0], "green") == 0) led = LED_GREEN;
    else {
        put_output("Wrong LED color\r\n");
        return;
    }

    if (num_args == 1) {
        if (leds_state(&leds, led) == LED_STATE_ON) put_output("on\r\n");
        else if (leds_state(&leds, led) == LED_STATE_OFF) put_output("off\r\n");
        else put_output("blink\r\n");
    } else {
        if (strcmp(args[1], "on") == 0) leds_on(&leds, led);
        else if (strcmp(args[1], "off") == 0) leds_off(&leds, led);
        else if (strcmp(args[1], "blink") == 0) leds_blink(&leds, led);
        else if (strcmp(args[1], "toggle") == 0) leds_toggle(&leds, led);
        else {
            put_output("Wrong LED command\r\n");
        }
    }
}
