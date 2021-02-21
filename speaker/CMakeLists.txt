cmake_minimum_required(VERSION 3.13)
include (pico_sdk_import.cmake)
project(test_speaker) 
 
pico_sdk_init()

add_executable(speaker speaker.c)
pico_enable_stdio_usb(speaker 1)
pico_enable_stdio_uart(speaker 1)
pico_add_extra_outputs(speaker)
target_link_libraries(speaker pico_stdlib hardware_pwm) 