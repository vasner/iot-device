/**
 * Console module tests
 */

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "version.h"
#include "pipe_console.h"
#include "catch.hpp"

TEST_CASE("console_parsing_clean", "[console][parsing]") {
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

TEST_CASE("console_parsing_dirty", "[console][parsing]") {
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);
    const char* command =  "    version    \r\n";
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

TEST_CASE("console_parsing_partial", "[console][parsing]") {
    pipe_console_init(false);

    int offset = 6;
    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY | O_NONBLOCK);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);
    const char* command =  "    version    \r\n";
    char result[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    // Clean up FIFO
    read(_tx_fifo, result, CONSOLE_MAX_LEN_MESSAGE_BYTES);

    write(_rx_fifo, command, offset);
    pipe_console_run();
    ssize_t len_message = read(_tx_fifo, result, CONSOLE_MAX_LEN_MESSAGE_BYTES);

    SECTION("validate result") {
        REQUIRE(len_message <= 0);
    }

    write(_rx_fifo, &command[offset], strlen(command) - offset);
    pipe_console_run();
    len_message = read(_tx_fifo, result, CONSOLE_MAX_LEN_MESSAGE_BYTES);

    SECTION("validate result") {
        REQUIRE(len_message > 0);
        REQUIRE(strstr(result, VERSION));
    }
 
    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}
