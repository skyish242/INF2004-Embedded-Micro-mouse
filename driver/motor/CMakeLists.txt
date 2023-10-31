add_executable(motor
        motor.c
        motor.h
)

add_library(motorLib motor.h motor.c)

target_link_libraries(motor pico_stdlib hardware_adc hardware_pwm)
target_link_libraries(motorLib pico_stdlib hardware_adc hardware_pwm)

target_include_directories(motorLib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    # ${CMAKE_SOURCE_DIR}/driver/web_server # Include the motor directory
)

# pico_enable_stdio_usb(motor 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(motor)

# add url via pico_set_program_url
# example_auto_set_url(motor)


