/**
 * Console commands tests
 */

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "version.h"
#include "pipe_console.h"
#include "catch.hpp"

TEST_CASE("command_unknown", "[console][command]") {
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);
    const char* command =  "bla-bla\r\n";
    write(_rx_fifo, command, strlen(command));

    pipe_console_run();

    char result[CONSOLE_MAX_LEN_MESSAGE_BYTES];
    ssize_t len_message = read(_tx_fifo, result, CONSOLE_MAX_LEN_MESSAGE_BYTES);

    SECTION("validate result") {
        REQUIRE(len_message > 0);
        REQUIRE(strstr(result, "Unknown command"));
    }
 
    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}

TEST_CASE("command_version", "[console][command]") {
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);
    const char* command =  "version\r\n";
    write(_rx_fifo, command, strlen(command));

    pipe_console_run();

    char result[CONSOLE_MAX_LEN_MESSAGE_BYTES];
    ssize_t len_message = read(_tx_fifo, result, CONSOLE_MAX_LEN_MESSAGE_BYTES);

    SECTION("validate result") {
        REQUIRE(len_message > 0);
        REQUIRE(strstr(result, VERSION));
    }
 
    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}

TEST_CASE("command_help_specific", "[console][command]") {
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);
    const char* command =  "help version\r\n";
    write(_rx_fifo, command, strlen(command));

    pipe_console_run();

    char result[CONSOLE_MAX_LEN_MESSAGE_BYTES];
    ssize_t len_message = read(_tx_fifo, result, CONSOLE_MAX_LEN_MESSAGE_BYTES);

    SECTION("validate result") {
        REQUIRE(len_message > 0);
        REQUIRE(strstr(result, "Gets device version"));
    }
 
    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}
