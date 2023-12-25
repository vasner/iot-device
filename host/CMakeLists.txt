cmake_minimum_required(VERSION 3.16)

project(iot_device)

set(EXECUTABLE ${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_STANDARD 99)

include_directories(
    src
    src/common
    src/common/console
    src/common/leds
    src/interface
)

add_executable(
    ${EXECUTABLE}
    src/common/console/commands.c
    src/common/console/console.c
    src/common/leds/leds.c
    src/interface/pipe_console.c
    src/main.c
)

target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra)
