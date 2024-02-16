/**
 * Console commands tests
 */

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>

#include "catch.hpp"
#include "leds.h"
#include "pipe_console.h"
#include "version.h"

leds_t leds;

static const char* _led_type_to_str[NUM_LEDS] = {"red", "amber", "green"};

TEST_CASE("command_unknown", "[console][command]") {
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);
    const char* command = "bla-bla\r\n";
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
    const char* command = "version\r\n";
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
    const char* command = "help version\r\n";
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

TEST_CASE("command_leds_on", "[console][command][hw]") {
    char tmp[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    leds_init(&leds);
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);

    for (int led = 0; led < NUM_LEDS; led++) {
        sprintf(tmp, "leds %s on\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);

        sprintf(tmp, "leds %s\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        ssize_t len_message = read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);
        SECTION("validate result") {
            REQUIRE(len_message > 0);
            REQUIRE(strstr(tmp, "on"));
        }
    }

    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}

TEST_CASE("command_leds_off", "[console][command][hw]") {
    char tmp[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    leds_init(&leds);
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);

    for (int led = 0; led < NUM_LEDS; led++) {
        sprintf(tmp, "leds %s off\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);

        sprintf(tmp, "leds %s\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        ssize_t len_message = read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);
        SECTION("validate result") {
            REQUIRE(len_message > 0);
            REQUIRE(strstr(tmp, "off"));
        }
    }

    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}

TEST_CASE("command_leds_toggle", "[console][command][hw]") {
    char tmp[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    leds_init(&leds);
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);

    for (int led = 0; led < NUM_LEDS; led++) {
        sprintf(tmp, "leds %s on\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();

        sprintf(tmp, "leds %s toggle\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);

        sprintf(tmp, "leds %s\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        ssize_t len_message = read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);
        SECTION("validate result") {
            REQUIRE(len_message > 0);
            REQUIRE(strstr(tmp, "off"));
        }
    }

    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}

TEST_CASE("command_leds_blink", "[console][command][hw]") {
    char tmp[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    leds_init(&leds);
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);

    for (int led = 0; led < NUM_LEDS; led++) {
        sprintf(tmp, "leds %s blink\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);

        sprintf(tmp, "leds %s\r\n", _led_type_to_str[led]);
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        ssize_t len_message = read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);
        SECTION("validate result") {
            REQUIRE(len_message > 0);
            REQUIRE(strstr(tmp, "blink"));
        }
    }

    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}

TEST_CASE("command_leds_common_status", "[console][command][hw]") {
    char tmp[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    leds_init(&leds);
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);

    for (int led = 0; led < NUM_LEDS; led++) {
        sprintf(tmp, "leds\r\n");
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        ssize_t len_message = read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);
        SECTION("validate result") {
            REQUIRE(len_message > 0);
            REQUIRE(strstr(tmp, "red"));
            REQUIRE(strstr(tmp, "amber"));
            REQUIRE(strstr(tmp, "green"));
        }
    }

    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}

TEST_CASE("command_log_level", "[console][command]") {
    char tmp[CONSOLE_MAX_LEN_MESSAGE_BYTES];

    leds_init(&leds);
    pipe_console_init(false);

    int _tx_fifo = open(PIPE_CONSOLE_TX_FIFO_NAME, O_RDONLY);
    int _rx_fifo = open(PIPE_CONSOLE_RX_FIFO_NAME, O_WRONLY);

    for (int led = 0; led < NUM_LEDS; led++) {
        sprintf(tmp, "log_level\r\n");
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        ssize_t len_message = read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);
        SECTION("validate result") {
            REQUIRE(len_message > 0);
            REQUIRE(strstr(tmp, "warn"));
        }

        sprintf(tmp, "log_level fatal\r\n");
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);

        sprintf(tmp, "log_level\r\n");
        write(_rx_fifo, tmp, strlen(tmp));
        pipe_console_run();
        len_message = read(_tx_fifo, tmp, CONSOLE_MAX_LEN_MESSAGE_BYTES);
        SECTION("validate result") {
            REQUIRE(len_message > 0);
            REQUIRE(strstr(tmp, "warn"));
        }
    }

    close(_rx_fifo);
    close(_tx_fifo);
    pipe_console_deinit();
}
