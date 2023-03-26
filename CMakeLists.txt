cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(imu)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(imu src/main.c)

target_include_directories(imu PUBLIC lib/)

target_link_libraries(imu pico_stdlib hardware_i2c hardware_spi)

pico_enable_stdio_usb(imu 1)
pico_enable_stdio_uart(imu 0)

pico_add_extra_outputs(imu)