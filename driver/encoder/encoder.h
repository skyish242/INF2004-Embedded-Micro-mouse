#ifndef ENCODER_H
#define ENCODER_H


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define GP21 and GP22 for the left and right encoders
#define LEFT_ENCODER_PIN 21
#define RIGHT_ENCODER_PIN 22

static float left_speed;
static float right_speed;
static float distance;

void encoder_callback(uint gpio, uint32_t events);

#endif /* ENCODER_H */
