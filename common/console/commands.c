/**
 * Console commands implementation
 */

#include <stdio.h>
#include <string.h>

#include "console.h"
#include "leds.h"
#include "log.h"
#include "version.h"

extern leds_t leds;

static void _command_version_run(const char** args, uint8_t num_args, void* ctx);
static void _command_leds(const char** args, uint8_t num_args, void* ctx);
static void _command_log_level(const char** args, uint8_t num_args, void* ctx);

command_t command_version = {
    .type = CONSOLE_COMMAND_VERSION,
    .name = "version",
    .description = "Gets device version",
    .process = _command_version_run,
};

command_t command_leds = {
    .type = CONSOLE_COMMAND_LEDS,
    .name = "leds",
    .description = "Turns on/off or return LED(s) state. `leds ?<red/amber/green> ?<on/off/blink/toggle>` ",
    .process = _command_leds,
};

command_t command_log_level = {
    .type = CONSOLE_COMMAND_LOG_LEVEL,
    .name = "log_level",
    .description = "Sets/gets log level, default is `warn`. `log_level ?<trace/debug/info/warn/error/fatal>`",
    .process = _command_log_level,
};

static void _error_wrong_number_of_arguments(console_put_output_t put_output) {
    put_output("Wrong number of arguments\r\n");
}

static void _command_version_run(const char** args, uint8_t num_args, void* ctx) {
    (void)args;
    (void)num_args;
    console_t* console = (console_t*)ctx;
    console_put_output_t put_output = console->put_output;
    put_output(VERSION);
    put_output("\n");
}

static void _command_leds(const char** args, uint8_t num_args, void* ctx) {
    console_t* console = (console_t*)ctx;
    console_put_output_t put_output = console->put_output;

    if (num_args > 2) {
        _error_wrong_number_of_arguments(put_output);
        return;
    }

    if (num_args == 0) {
        static char* led_state_to_str[NUM_LED_STATES] = {"on", "off", "blink"};
        sprintf(
            console->tmp, "red %s, amber %s, green %s\r\n", led_state_to_str[leds_state(&leds, LED_RED)],
            led_state_to_str[leds_state(&leds, LED_AMBER)], led_state_to_str[leds_state(&leds, LED_GREEN)]
        );
        put_output(console->tmp);
        return;
    }

    led_type_t led = LED_RED;
    if (strcmp(args[0], "red") == 0) {
        led = LED_RED;
    } else if (strcmp(args[0], "amber") == 0) {
        led = LED_AMBER;
    } else if (strcmp(args[0], "green") == 0) {
        led = LED_GREEN;
    } else {
        put_output("Wrong LED color\r\n");
        return;
    }

    if (num_args == 1) {
        if (leds_state(&leds, led) == LED_STATE_ON) {
            put_output("on\r\n");
        } else if (leds_state(&leds, led) == LED_STATE_OFF) {
            put_output("off\r\n");
        } else {
            put_output("blink\r\n");
        }
    } else {
        if (strcmp(args[1], "on") == 0) {
            leds_on(&leds, led);
        } else if (strcmp(args[1], "off") == 0) {
            leds_off(&leds, led);
        } else if (strcmp(args[1], "blink") == 0) {
            leds_blink(&leds, led);
        } else if (strcmp(args[1], "toggle") == 0) {
            leds_toggle(&leds, led);
        } else {
            put_output("Wrong LED state\r\n");
        }
    }
}

static void _command_log_level(const char** args, uint8_t num_args, void* ctx) {
    console_t* console = (console_t*)ctx;
    console_put_output_t put_output = console->put_output;

    if (num_args == 0) {
        static char* _log_level_to_str[NUM_LOG_LEVELS] = {"trace", "debug", "info", "warn", "error", "fatal"};
        sprintf(console->tmp, "%s\r\n", _log_level_to_str[log_get_level()]);
        put_output(console->tmp);
    } else if (num_args == 1) {
        if (strcmp(args[1], "trace") == 0) {
            log_set_level(LOG_LEVEL_TRACE);
        } else if (strcmp(args[1], "debug") == 0) {
            log_set_level(LOG_LEVEL_DEBUG);
        } else if (strcmp(args[1], "info") == 0) {
            log_set_level(LOG_LEVEL_INFO);
        } else if (strcmp(args[1], "warn") == 0) {
            log_set_level(LOG_LEVEL_WARN);
        } else if (strcmp(args[1], "error") == 0) {
            log_set_level(LOG_LEVEL_ERROR);
        } else if (strcmp(args[1], "fatal") == 0) {
            log_set_level(LOG_LEVEL_FATAL);
        } else {
            put_output("Wrong log level\r\n");
        }
    } else {
        _error_wrong_number_of_arguments(put_output);
    }
}
