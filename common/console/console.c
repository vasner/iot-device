/**
 * Console module
 */

#include "console.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

#define _MAX_NARGS (10)

static void _console_process_line(console_t* state, char* line);

static void _register_command(console_t* state, command_t* command) {
    state->commands[state->num_commands++] = command;
}

void console_init(console_t* state, console_get_input_t get_input, console_put_output_t put_output) {
    state->num_commands = 0;
    state->get_input = get_input;
    state->put_output = put_output;
    _register_command(state, &command_version);
    _register_command(state, &command_leds);
    _register_command(state, &command_log_level);
    put_output("\n>>> ");
}

void console_run(console_t* state) {
    static char raw_msg[CONSOLE_MAX_LEN_MESSAGE_BYTES];
    static char line[CONSOLE_MAX_LEN_MESSAGE_BYTES];
    static uint8_t len_line = 0;

    uint8_t count = state->get_input(raw_msg, CONSOLE_MAX_LEN_MESSAGE_BYTES);

    if (count > 0) {
        int n = 0;
        while (n < count) {
            if ((raw_msg[n] == '\n') || (raw_msg[n] == '\r')) {
                line[len_line] = '\0';
                _console_process_line(state, line);
                state->put_output(">>> ");
                len_line = 0;
                n++;
                while (((raw_msg[n] == '\r') || (raw_msg[n] == '\n') || (raw_msg[n] == ' ')) && (n < count)) { n++; }
            } else {
                if (len_line >= CONSOLE_MAX_LEN_MESSAGE_BYTES) {
                    LOG_ERROR("Line buffer overflow\n");
                    len_line = 0;
                }
                line[len_line++] = raw_msg[n++];
            }
        }
    }
}

static void _console_process_line(console_t* state, char* line) {
    static const char* args[_MAX_NARGS];
    int nargs = 0;

    char* token = strtok(line, " ");
    while (token != NULL) {
        args[nargs] = token;
        token = strtok(NULL, " ");
        if (++nargs > _MAX_NARGS) {
            sprintf(state->tmp, "Number of arguments is lager than %d\r\n", _MAX_NARGS);
            state->put_output(state->tmp);
            break;
        }
    }

    if (nargs > 0) {
        if (strcmp(args[0], "help") == 0) {
            if (nargs == 1) {
                // All commands help
                for (int n = 0; n < state->num_commands; n++) {
                    sprintf(state->tmp, "%-10s %s\r\n", state->commands[n]->name, state->commands[n]->description);
                    state->put_output(state->tmp);
                }
            } else {
                // Specific command help
                bool is_handled = false;
                for (int n = 0; n < state->num_commands; n++) {
                    if (strcmp(args[1], state->commands[n]->name) == 0) {
                        sprintf(state->tmp, "%s\r\n", state->commands[n]->description);
                        state->put_output(state->tmp);
                        is_handled = true;
                    }
                }
                if (!is_handled) {
                    sprintf(state->tmp, "Unknown command: `%s`\r\n", args[1]);
                    state->put_output(state->tmp);
                }
            }

            return;
        }

        bool is_handled = false;
        for (int n = 0; n < state->num_commands; n++) {
            if (strcmp(args[0], state->commands[n]->name) == 0) {
                state->commands[n]->process(&args[1], nargs - 1, state);
                is_handled = true;
            }
        }
        if (!is_handled) {
            sprintf(state->tmp, "Unknown command: `%s`\r\n", args[0]);
            state->put_output(state->tmp);
        }
    }
}
