add_executable(encoder
        encoder.c
        encoder.h
        )

add_library(encoderLib encoder.h encoder.c)

target_link_libraries(encoder pico_stdlib hardware_adc hardware_pwm)
target_link_libraries(encoderLib pico_stdlib hardware_adc hardware_pwm)

target_include_directories(encoder PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    # ${CMAKE_SOURCE_DIR}/driver/motor
)

# create map/bin/hex file etc.
pico_add_extra_outputs(encoder)

# add url via pico_set_program_url
example_auto_set_url(encoder)

pico_enable_stdio_usb(encoder 1)