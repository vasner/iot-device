cmake_minimum_required(VERSION 3.16)

project(iot_device)

set(EXECUTABLE ${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_STANDARD 99)

include_directories(
    src
    src/console
    src/leds
)

add_executable(
    ${EXECUTABLE}
    src/console/commands.c
    src/console/console.c
    src/leds/leds.c
    src/main.c
)

target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra)
