add_executable(ultrasonic
        ultrasonic.c
        ultrasonic.h
        )

target_link_libraries(ultrasonic pico_stdlib)
add_library(ultrasonicLib ultrasonic.h ultrasonic.c)
target_link_libraries(ultrasonicLib pico_stdlib)

target_include_directories(ultrasonic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
 
)

pico_enable_stdio_usb(ultrasonic 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(ultrasonic)

# add url via pico_set_program_url
#example_auto_set_url(ultrasonic)