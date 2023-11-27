add_executable(irline
        irline.h
        irline.c
        )

target_link_libraries(irline pico_stdlib hardware_adc hardware_pwm)

add_library(IRLib irline.h irline.c)
target_link_libraries(IRLib pico_stdlib hardware_adc hardware_pwm)

pico_enable_stdio_usb(irline 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(irline)

# add url via pico_set_program_url
#example_auto_set_url(irline)
