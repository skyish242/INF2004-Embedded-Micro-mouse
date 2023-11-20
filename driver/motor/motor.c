/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "motor.h"
#include "encoder.h"


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
    setupPWM(LEFT_MOTOR_PWM);
    setupPWM(RIGHT_MOTOR_PWM);
}

// Compute PID 
float compute_pid(float setpoint, float current_val, float integral, float prev_error, float Kp, float Ki, float Kd){
    // Compute error
    float error = setpoint - current_val;
    
    // Update integral term
    integral += error;
    
    // Compute derivative term
    float derivative = error - prev_error;
    
    // Compute control signal
    float control_signal = Kp * error + Ki * (integral) + Kd * derivative;
    
    // Update previous error
    prev_error = error; 
    
    return control_signal;
}

// Set up PWM
void setupPWM (uint gpio) {

    // Allocate the GPIO pin to PWM
    gpio_set_function (gpio, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Set clock divisor to 100, reducing main clock from 125Mhz to 12.5Mhz frequency
    pwm_set_clkdiv(slice_num, 100);

    // Set PWM wrap value to determine period of PWM signal
    // Set desired PWM frequency of 20khz
    // Wrap value = System Clock Frequency / Desired PWM frequency
    // Period of PWM signal = ((1 / 12.5Mhz) * (125Mz/20khz)) = 500 microseconds
    pwm_set_wrap(slice_num, 12500);

    // Enable PWM
    pwm_set_enabled(slice_num, true);
}

// Set the speed for the left motor
void setMotorLeft() {
    
    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(LEFT_MOTOR_PWM);

    uint16_t val = compute_pid(SETPOINT, left_speed(), 0.0 , 0.0, KP_LEFT, KI_LEFT, KD_LEFT) + DUTY_CYCLE;

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to pwm_value
    pwm_set_chan_level(slice_num, PWM_CHAN_A, val);
}

// Set the speed for the right motor
void setMotorRight() {
    
    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(RIGHT_MOTOR_PWM);

    uint16_t val = compute_pid(SETPOINT, right_speed(), 0.0 , 0.0, KP_RIGHT, KI_RIGHT, KD_RIGHT) + DUTY_CYCLE;

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to pwm_value
    pwm_set_chan_level(slice_num, PWM_CHAN_B, val);
}

// Move backward
void moveBackward() {

    // Motor A
    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 1);
    gpio_put(IN4, 0);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft();
    setMotorRight();
}

// Move forward 
void moveForward() {

    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 1);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft();
    setMotorRight();
}

// Move left
void moveLeft() {

    // Motor A
    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 1);


    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft();
    setMotorRight();
}

// Move right
void moveRight() {
    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    // Motor B
    gpio_put(IN3, 1);
    gpio_put(IN4, 0);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to 50%
    setMotorLeft();
    setMotorRight();
}

// Stop both motors
void stopMotors(){

    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 0);
}


int main() {

    // Initialisation
    stdio_init_all();
    motorInit();

    // Set pin as high for left encoder
    gpio_init(LEFT_VCC);
    gpio_set_dir(LEFT_VCC, GPIO_OUT);
    gpio_put(LEFT_VCC, 1);

    // Set pin as high for right encoder
    gpio_init(RIGHT_VCC);
    gpio_set_dir(RIGHT_VCC, GPIO_OUT);
    gpio_put(RIGHT_VCC, 1);

    // Set up the interrupt to trigger encoder_callback when rising edge is detected
    gpio_set_irq_enabled_with_callback(LEFT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);
    gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);

    while (1) {
        moveForward();
    }
}

