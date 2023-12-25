/**
 * Console module 
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#define CONSOLE_MAX_LEN_MESSAGE_BYTES (256)

typedef enum {
    CONSOLE_COMMAND_HELP = 0,
    CONSOLE_COMMAND_VERSION,
    CONSOLE_COMMAND_LOG,
    CONSOLE_COMMAND_LEDS,
    CONSOLE_COMMAND_SENSORS,
    CONSOLE_COMMAND_CONTROLS,
    NUM_CONSOLE_COMMANDS // Should always be the last
} command_type_t;

typedef void (*console_output_t)(const char* data);
typedef void (*command_run_t)(const char** args, uint8_t num_args, console_output_t console_output);

typedef struct {
    const char* name;
    const char* description;
    command_run_t process;
} command_t;

typedef struct {
    command_t* commands[NUM_CONSOLE_COMMANDS];
    console_output_t console_output;
    uint8_t num_commands; // Number of registered commands
} console_t;

extern command_t command_version;

void console_init(console_t* state, console_output_t console_output);
void console_process(const char* line);

#endif // CONSOLE_H