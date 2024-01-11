/**
 * Console module 
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#define CONSOLE_MAX_LEN_MESSAGE_BYTES (255)

typedef enum {
    CONSOLE_COMMAND_HELP = 0,
    CONSOLE_COMMAND_VERSION,
    CONSOLE_COMMAND_LOG,
    CONSOLE_COMMAND_LEDS,
    CONSOLE_COMMAND_SENSORS,
    CONSOLE_COMMAND_CONTROLS,
    NUM_CONSOLE_COMMANDS // Should always be the last
} command_type_t;

typedef uint8_t (*console_get_input_t)(char* data, uint8_t max_len);
typedef void (*console_put_output_t)(const char* data);
typedef void (*command_run_t)(const char** args, uint8_t num_args, console_put_output_t put_output);

typedef struct {
    command_type_t type;
    const char* name;
    const char* description;
    command_run_t process;
} command_t;

typedef struct {
    command_t* commands[NUM_CONSOLE_COMMANDS];
    console_get_input_t get_input;
    console_put_output_t put_output;
    uint8_t num_commands; // Number of registered commands
} console_t;

extern command_t command_version;
extern command_t command_leds;

void console_init(console_t* state, console_get_input_t get_input, console_put_output_t put_output);
void console_run(console_t* state);

#endif // CONSOLE_H