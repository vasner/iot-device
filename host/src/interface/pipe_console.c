/**
 * Linux named pipe (aka FIFO) based console interface and commands implementation
 */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "console.h"

#define _OUTPUT_TO_STDOUT

static console_t _console;

static const char* _rx_fifo_name = "iot_rx_fifo";
static int _rx_fifo = -1;

static const char* _tx_fifo_name = "iot_tx_fifo";
static int _tx_fifo = -1;

static void _console_output(const char* data) {
#ifdef _OUTPUT_TO_STDOUT
    printf("%s", data);
#endif  // _OUTPUT_TO_STDOUT
}

void pipe_console_init(void) {
    mkfifo(_rx_fifo_name, 0666);
    _rx_fifo = open(_rx_fifo_name, O_RDONLY | O_NONBLOCK);
    if (_rx_fifo < 0) {
        printf("Unable to open RX FIFO\n");
    }

    console_init(&_console, _console_output);
}

void pipe_console_deinit(void) {
    close(_rx_fifo);
    unlink(_rx_fifo_name);
}

void pipe_console_run(void) {
    static char raw_msg[CONSOLE_MAX_LEN_MESSAGE_BYTES];
    static char line[CONSOLE_MAX_LEN_MESSAGE_BYTES];
    static uint8_t len_line = 0;

    if (_rx_fifo < 0) return;

    ssize_t count = read(_rx_fifo, raw_msg, CONSOLE_MAX_LEN_MESSAGE_BYTES);
    if (count > 0) {
        int n = 0;
        while (n < count) {
            if ((raw_msg[n] == '\n') || (raw_msg[n] == '\r')) {
                line[len_line] = '\0';
                console_process(&_console, line);
                len_line = 0;
                n++;
                while (((raw_msg[n] == '\r') || (raw_msg[n] == '\n') || (raw_msg[n] == ' ')) && (n < count)) {
                    n++;
                }
            } else {
                if (len_line > CONSOLE_MAX_LEN_MESSAGE_BYTES) {
                    printf("Line buffer overflow\n");
                    len_line = 0;
                }
                line[len_line++] = raw_msg[n++];
            }
        }
    }
}
