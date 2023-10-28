/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define GPIO pins for motor input
#define IN1 10  // Input 1 for Motor A
#define IN2 11  // Input 2 for Motor A
#define IN3 12  // Input 1 for Motor B
#define IN4 13  // Input 2 for Motor B

// Define GPIO pins for PWM
#define LEFT_MOTOR_PWM 0
#define RIGHT_MOTOR_PWM 1

// Define duty cycle to be 50%
#define DUTY_CYCLE 12500/2

// Initialise motor control pins as output
void motorInit() {
    gpio_init(IN1);
    gpio_init(IN2);
    gpio_init(IN3);
    gpio_init(IN4);
    gpio_set_dir(IN1, GPIO_OUT);
    gpio_set_dir(IN2, GPIO_OUT);
    gpio_set_dir(IN3, GPIO_OUT);
    gpio_set_dir(IN4, GPIO_OUT);
}

// Set up PWM
void setupPWM (uint gpio) {

    // Allocate the GPIO pin to PWM
    gpio_set_function (gpio, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Set clock divisor to 100, reducing main clock from 125Mhz to 1.25Mhz frequency
    pwm_set_clkdiv(slice_num, 100);

    // Set PWM wrap value to determine period of PWM signal
    // Period of PWM signal = ((1 / 1.25Mhz) * 12500) = 10ms
    pwm_set_wrap(slice_num, 12500);

    // Set duty cycle of PWM signal on channel A and B of the specified PWM slice to 0%
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);

    // Enable PWM
    pwm_set_enabled(slice_num, true);
}

// Set the speed for the left motor
void setMotorLeft (int16_t pwm_value) {
    
    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(LEFT_MOTOR_PWM);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to pwm_value
    pwm_set_chan_level(slice_num, PWM_CHAN_A, (uint16_t) pwm_value);
}

// Set the speed for the right motor
void setMotorRight (int16_t pwm_value) {
    
    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(RIGHT_MOTOR_PWM);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to pwm_value
    pwm_set_chan_level(slice_num, PWM_CHAN_B, (uint16_t) pwm_value);
}

// Move forward
void moveForward() {

    // Motor A
    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 1);
    gpio_put(IN4, 0);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft(DUTY_CYCLE);
    setMotorRight(DUTY_CYCLE);
}

// Move backward 
void moveBackward() {

    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 1);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft(DUTY_CYCLE);
    setMotorRight(DUTY_CYCLE);
}

// Move left
void moveLeft() {

    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    // Motor B
    gpio_put(IN3, 1);
    gpio_put(IN4, 0);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft(DUTY_CYCLE);
    setMotorRight(DUTY_CYCLE);
}

// Move right
void moveRight() {
    // Motor A
    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 1);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft(DUTY_CYCLE);
    setMotorRight(DUTY_CYCLE);
}

// Stop both motors
void stopMotors(){

    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 0);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 0%
    setMotorLeft(0);
    setMotorRight(0);
}

int main() {

    // Initialisation
    stdio_init_all();
    motorInit();

    setupPWM(LEFT_MOTOR_PWM);
    setupPWM(RIGHT_MOTOR_PWM);

    while (1) {
        moveForward();
        
    }

}
