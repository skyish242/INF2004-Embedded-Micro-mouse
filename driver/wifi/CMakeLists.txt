add_executable(wifi
        wifi.c
        )

target_link_libraries(wifi pico_stdlib hardware_adc hardware_pwm)

pico_enable_stdio_usb(wifi 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(wifi)

# add url via pico_set_program_url
#example_auto_set_url(wifi)
