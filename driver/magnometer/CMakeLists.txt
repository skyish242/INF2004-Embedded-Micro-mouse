cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)


project(lsm303dlh_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX-STANDARD 17)
pico_sdk_init()

add_executable(LSM303DLH_pico
   main.c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(LSM303DLH_pico 1)
pico_enable_stdio_uart(LSM303DLH_pico 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(LSM303DLH_pico)

# create map/bin/hex file etc.
pico_add_extra_outputs(LSM303DLH_pico)

# add url via pico_set_program_url
example_auto_set_url(LSM303DLH_pico)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(LSM303DLH_pico pico_stdlib hardware_i2c)
