cmake_minimum_required(VERSION 3.20)
project(iot-device)

set(CMAKE_TOOLCHAIN_FILE arm-none-eabi-gcc.cmake)
set(LINKER_FILE sdk/libraries/cmsis/cm4/device_support/startup/gcc/linker/AT32F437xM_FLASH.ld)
set(EXECUTABLE ${PROJECT_NAME}.elf)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable( 
        ${EXECUTABLE}
        src/main.c
        src/common/console/console.c
        src/common/console/commands.c
        src/common/logc/log.c
        src/common/sensors/drivers/bmp280.c
        src/control/leds.c
        src/interface/vcom_console.c
        src/platform/platform.c
        src/platform/utils.c
        src/platform/vcom.c
        sdk/libraries/cmsis/cm4/device_support/startup/gcc/startup_at32f435_437.s
        sdk/libraries/cmsis/cm4/device_support/system_at32f435_437.c
        sdk/libraries/drivers/src/at32f435_437_crm.c
        sdk/libraries/drivers/src/at32f435_437_gpio.c
        sdk/libraries/drivers/src/at32f435_437_misc.c
        sdk/libraries/drivers/src/at32f435_437_tmr.c
        sdk/libraries/drivers/src/at32f435_437_usb.c
        sdk/middlewares/usbd_class/cdc/cdc_class.c
        sdk/middlewares/usbd_class/cdc/cdc_desc.c
        sdk/middlewares/usb_drivers/src/usb_core.c
        sdk/middlewares/usb_drivers/src/usbd_core.c
        sdk/middlewares/usb_drivers/src/usbd_int.c
        sdk/middlewares/usb_drivers/src/usbd_sdr.c
)

target_include_directories(
        ${EXECUTABLE} PRIVATE
        src/interface
        src/common
        src/common/console
        src/common/control
        src/common/logc
        src/common/math
        src/common/sensors
        src/common/sensors/drivers
        src/common/utils
        src/platform
        sdk/libraries/cmsis/cm4/device_support
        sdk/libraries/cmsis/cm4/core_support
        sdk/libraries/drivers/inc
        sdk/middlewares/usb_drivers/inc
        sdk/middlewares/usbd_class/cdc
)

target_compile_definitions(
        ${EXECUTABLE} PRIVATE
        -DAT32F437ZMT7
)

target_compile_options(
        ${EXECUTABLE} PRIVATE 
        -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard 
        -fdata-sections -ffunction-sections 
        -Wall -Wextra 
        -O0 
        -g3
)

target_link_options(
        ${EXECUTABLE} PRIVATE 
        -T${CMAKE_SOURCE_DIR}/${LINKER_FILE}
        -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
        -specs=nano.specs -specs=nosys.specs
        -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections -Xlinker -print-memory-usage -Xlinker
)

# Print executable size as part of the post build process
add_custom_command(
        TARGET ${EXECUTABLE} POST_BUILD 
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE}
)

# Create hex, bin and S-Record files after the build
add_custom_command(
        TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)
