cmake_minimum_required(VERSION 3.16)

project(iot_device)

set(EXECUTABLE ${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_STANDARD 99)

include_directories(
)

add_executable(
    ${EXECUTABLE}
    main.c
)

target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra)
