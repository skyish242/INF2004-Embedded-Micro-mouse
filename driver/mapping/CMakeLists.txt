add_executable(floodfill
        floodfill.c
        floodfill.h
)

add_library(floodfillLib floodfill.h floodfill.c)

target_link_libraries(floodfill pico_stdlib hardware_adc hardware_pwm)
target_link_libraries(floodfillLib pico_stdlib hardware_adc hardware_pwm)

target_include_directories(floodfillLib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# pico_enable_stdio_usb(floodfill 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(floodfill)