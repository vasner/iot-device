/**
 * Linux named pipe (aka FIFO) based console interface and commands implementation
 */

#include "pipe_console.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "console.h"
#include "log.h"

static console_t _console;
static bool _is_duplicate_tx_to_stdout = false;

static const char* _rx_fifo_name = "iot_rx_fifo";
static int _rx_fifo = -1;

static const char* _tx_fifo_name = "iot_tx_fifo";
static int _tx_fifo = -1;

static void _console_put_output(const char* data) {
    if (_is_duplicate_tx_to_stdout) {
        printf("%s", data);
    }
    if (_rx_fifo < 0) return;
    write(_tx_fifo, data, strlen(data));
}

static uint8_t console_get_input(char* data, uint8_t max_len) {
    if (_rx_fifo < 0) return 0;
    return read(_rx_fifo, data, max_len);
}

void pipe_console_init(bool is_duplicate_tx_to_stdout) {
    _is_duplicate_tx_to_stdout = is_duplicate_tx_to_stdout;
    
    mkfifo(_rx_fifo_name, 0666);
    _rx_fifo = open(_rx_fifo_name, O_RDONLY | O_NONBLOCK);
    if (_rx_fifo < 0) {
        LOG_ERROR("Unable to open RX FIFO\n");
    }
    
    mkfifo(_tx_fifo_name, 0666);
    _tx_fifo = open(_tx_fifo_name, O_RDONLY | O_NONBLOCK);
    if (_tx_fifo < 0) {
        LOG_ERROR("Unable to open TX FIFO\n");
    }

    console_init(&_console, console_get_input, _console_put_output);
}

void pipe_console_deinit(void) {
    close(_rx_fifo);
    unlink(_rx_fifo_name);
}

void pipe_console_run(void) {
    console_run(&_console);
}
