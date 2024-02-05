/**
 * Console module
 */

#include "console.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

#define _MAX_NARGS (10)

static char _tmp_str[CONSOLE_MAX_LEN_MESSAGE_BYTES];

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
            sprintf(_tmp_str, "Number of arguments is lager than %d\n", _MAX_NARGS);
            state->put_output(_tmp_str);
            break;
        }
    }

    if (nargs > 0) {
        if (strcmp(args[0], "help") == 0) {
            if (nargs == 1) {
                // All commands help
                for (int n = 0; n < state->num_commands; n++) {
                    sprintf(_tmp_str, "%-10s %s\n", state->commands[n]->name, state->commands[n]->description);
                    state->put_output(_tmp_str);
                }
            } else {
                // Specific command help
                bool is_handled = false;
                for (int n = 0; n < state->num_commands; n++) {
                    if (strcmp(args[1], state->commands[n]->name) == 0) {
                        sprintf(_tmp_str, "%s\n", state->commands[n]->description);
                        state->put_output(_tmp_str);
                        is_handled = true;
                    }
                }
                if (!is_handled) {
                    sprintf(_tmp_str, "Unknown command: `%s`\n", args[1]);
                    state->put_output(_tmp_str);
                }
            }

            return;
        }

        bool is_handled = false;
        for (int n = 0; n < state->num_commands; n++) {
            if (strcmp(args[0], state->commands[n]->name) == 0) {
                state->commands[n]->process(&args[1], nargs - 1, state->put_output);
                is_handled = true;
            }
        }
        if (!is_handled) {
            sprintf(_tmp_str, "Unknown command: `%s`\n", args[0]);
            state->put_output(_tmp_str);
        }
    }
}
