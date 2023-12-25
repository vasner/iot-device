/**
 * Linux named pipe (aka FIFO) based console interface and commands implementation
 */

#include "console.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define _OUTPUT_TO_STDOUT

static console_t _console;

static const char* _rx_fifo_name = "iot_rx_fifo";
static int _rx_fifo = -1;

static const char* _tx_fifo_name = "iot_tx_fifo";
static int _tx_fifo = -1;

static void _console_output(const char* data) {
#ifdef _OUTPUT_TO_STDOUT
#endif // _OUTPUT_TO_STDOUT
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
    static char _raw_message[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    if (_rx_fifo < 0) return;

    ssize_t count = read(_rx_fifo, _raw_message, CONSOLE_MAX_LEN_MESSAGE_BYTES);
    if (count > 0) { 
        printf("%s", _raw_message);
    }
}
