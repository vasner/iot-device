/**
 * Console commands implementation
 */

#include "console.h"
#include "version.h"

static void _command_version_run(const char** args, uint8_t num_args, console_put_output_t put_output);

command_t command_version = {
    .name = "version",
    .description = "Gets device version",
    .process = _command_version_run,
};

static void _command_version_run(const char** args, uint8_t num_args, console_put_output_t put_output) {
    (void)args;
    (void)num_args;
    put_output(VERSION);
    put_output("\n");
}
