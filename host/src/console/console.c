/**
 * Console module 
 */

#include "console.h"

static void _register_command(console_t* state, command_t* command) {
    state->commands[state->num_commands++] = command;
} 

void console_init(console_t* state, console_output_t console_output) {
    state->num_commands = 0;
    state->console_output = console_output;
    _register_command(state, &command_version);
}

void console_process(const char* line) {
    // TODO: Implement
    (void)line;
}

