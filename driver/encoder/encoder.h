#ifndef ENCODER_H
#define ENCODER_H


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define GP2 and GP3 for the left and right encoders
#define LEFT_ENCODER_PIN 21
#define LEFT_VCC 22
#define RIGHT_ENCODER_PIN 8
#define RIGHT_VCC 9

void encoder_callback(uint gpio, uint32_t events);
float left_speed();
float right_speed();

#endif /* ENCODER_H */
