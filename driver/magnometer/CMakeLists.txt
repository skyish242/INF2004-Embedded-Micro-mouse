cmake_minimum_required(VERSION 3.13)

# include(pico_sdk_import.cmake)

# project(lsm303dlh_project C CXX ASM)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX-STANDARD 17)

add_executable(magnometer
   magnometer.c
)

add_library(magnoLib magnometer.h magnometer.c)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(magnometer pico_stdlib hardware_i2c)
target_link_libraries(magnoLib pico_stdlib hardware_i2c)

# enable usb output, disable uart output
pico_enable_stdio_usb(magnometer 1)
pico_enable_stdio_uart(magnometer 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(magnometer)

# add url via pico_set_program_url
example_auto_set_url(magnometer)