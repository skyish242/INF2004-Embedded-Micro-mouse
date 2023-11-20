cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

include(pico_extras_import_optional.cmake)

include(example_auto_set_url.cmake)

project(micro-mouse C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)
if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

#include(example_auto_set_url.cmake)
cmake_policy(SET CMP0002 NEW)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

add_subdirectory(driver)

set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel-main)

if (NOT FREERTOS_KERNEL_PATH AND NOT DEFINED ENV{FREERTOS_KERNEL_PATH})
    message("Skipping Pico W FreeRTOS examples as FREERTOS_KERNEL_PATH not defined")
else()
    include(FreeRTOS_Kernel_import.cmake)
endif()

set(PROGRAM_NAME main)
add_executable(${PROGRAM_NAME}
    main.c
    main.h
)

target_include_directories(${PROGRAM_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/driver/magnometer # Include the magnometer directory
    # ${CMAKE_SOURCE_DIR}/driver/motor # Include the motor directory
    ${CMAKE_SOURCE_DIR}/driver/web_server # Include the web_server directory
    ${CMAKE_SOURCE_DIR}/driver/irline # Include the irline directory
    ${CMAKE_SOURCE_DIR}/driver/ultrasonic # Include the ultrasonic directory
    ${CMAKE_SOURCE_DIR}/driver/mapping # Include the mapping directory
)

target_link_libraries(${PROGRAM_NAME}
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib
    pico_lwip_http
    hardware_adc
    hardware_pwm
    hardware_i2c
    FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
    motorLib
    magnoLib
    irLineLib
    floodfillLib
)

pico_enable_stdio_usb(${PROGRAM_NAME} 1)
pico_enable_stdio_uart(${PROGRAM_NAME} 0)
pico_add_extra_outputs(${PROGRAM_NAME})