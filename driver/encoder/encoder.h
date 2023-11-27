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


// Constants to calculate distance in centimeters
extern const uint32_t ENCODER_NOTCHES;
extern const float WHEEL_CIRCUMFERENCE;

void encoder_callback(uint gpio, uint32_t events);
uint32_t getLeftNotchCount();
uint32_t getRightNotchCount();

void resetNotchCount();

float getLeftEncoderDistance();
float getRightEncoderDistance();
void resetEncoderDistance();

#endif /* ENCODER_H */