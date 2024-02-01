cmake_minimum_required(VERSION 3.16)

project(iot_device)

set(EXECUTABLE ${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories(
    src
    src/common
    src/common/catch2
    src/common/console
    src/common/control
    src/common/sensors
    src/common/sensors/drivers
    src/common/logc
    src/interface
    src/tests
)

add_executable(
    ${EXECUTABLE}
    src/common/console/commands.c
    src/common/console/console.c
    src/common/logc/log.c
    src/common/sensors/drivers/bmp280.c
    src/control/leds.c
    src/interface/pipe_console.c
    src/tests/console_test.cpp
    src/tests/commands_test.cpp
    src/main.cpp
)

target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra -Werror)
