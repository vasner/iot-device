/**
 * Console module 
 */

#include "console.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define _MAX_NARGS (10)

static void _register_command(console_t* state, command_t* command) {
    state->commands[state->num_commands++] = command;
}

void console_init(console_t* state, console_output_t console_output) {
    state->num_commands = 0;
    state->console_output = console_output;
    _register_command(state, &command_version);
}

void console_process(console_t* state, char* line) {
    static const char* args[_MAX_NARGS];
    int nargs = 0;

    char* token = strtok(line, " ");
    while (token != NULL) {
        args[nargs] = token;
        token = strtok(NULL, " ");
        if (++nargs > _MAX_NARGS) {
            printf("Number of arguments is lager than %d\n", _MAX_NARGS);
            break;
        }
    }

    if (nargs > 0) {
        if (strcmp(args[0], "help") == 0) {
            if (nargs == 1) {
                // All commands help
                for (int n = 0; n < state->num_commands; n++) {
                    printf("%-10s %s\n", state->commands[n]->name, state->commands[n]->description);
                }
            } else {
                // Specific command help
                bool is_handled = false;
                for (int n = 0; n < state->num_commands; n++) {
                    if (strcmp(args[1], state->commands[n]->name) == 0) {
                        printf("%s\n", state->commands[n]->description);
                        is_handled = true;
                    }
                }
                if (!is_handled) {
                    printf("Unknown command: `%s`\n", args[1]);
                }
            }

            return;
        }
        
        bool is_handled = false;
        for (int n = 0; n < state->num_commands; n++) {
            if (strcmp(args[0], state->commands[n]->name) == 0) {
                state->commands[n]->process(&args[1], nargs - 1, state->console_output);
                is_handled = true;
            }
        }
        if (!is_handled) {
            printf("Unknown command: `%s`\n", args[0]);
        }
    }
}

