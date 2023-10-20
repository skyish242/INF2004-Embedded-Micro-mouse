add_executable(magnometer
        magnometer.c
        )

target_link_libraries(magnometer pico_stdlib hardware_adc hardware_pwm)

pico_enable_stdio_usb(magnometer 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(magnometer)

# add url via pico_set_program_url
#example_auto_set_url(magnometer)
