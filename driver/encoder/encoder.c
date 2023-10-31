/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define GP2 and GP3 for the left and right encoders
#define LEFT_ENCODER_PIN 2
#define RIGHT_ENCODER_PIN 3

// Constants to calculate distance in centimeters
const uint32_t ENCODER_NOTCHES = 20;
const float ENCODER_CIRCUMFERENCE = 21.05; // Approximation

void encoder_callback(uint gpio, uint32_t events) {
    // Encoder variables for distance and speed
    static uint32_t left_notch_count = 0; // To count each time notch has been detected 
    static uint32_t left_last_edge_time = 0; // To measure the pulse width
    static uint32_t right_notch_count = 0; 
    static uint32_t right_last_edge_time = 0; 

    // Get the current time in microseconds
    uint32_t current_time = time_us_32();
    uint32_t pulse_width; 

    // Check the GPIO pin that triggered the interrupt
    if (gpio == LEFT_ENCODER_PIN) {
        if (events & GPIO_IRQ_EDGE_RISE) {
            // Calculate the pulse width by subtracting last edge time from current time
            pulse_width = current_time - left_last_edge_time;
            
            // Update the last edge time
            left_last_edge_time = current_time;

            // Increment the notch count by 1 and calculate distance travelled
            left_notch_count++;
            float distance = (float)left_notch_count / ENCODER_NOTCHES * ENCODER_CIRCUMFERENCE;

            // Print the results
            printf("Left motor: Pulse width = %d us, Notch count = %d, Distance = %.2f cm\n", pulse_width, left_notch_count, distance);
        }
    } else if (gpio == RIGHT_ENCODER_PIN) {
        if (events & GPIO_IRQ_EDGE_RISE) {
            pulse_width = current_time - right_last_edge_time;
            
            right_last_edge_time = current_time;

            right_notch_count++;
            float distance = (float)right_notch_count / ENCODER_NOTCHES * ENCODER_CIRCUMFERENCE;

            printf("Right motor: Pulse width = %d us, Notch count = %d, Distance = %.2f cm\n", pulse_width, right_notch_count, distance);
        }
    }
}

int main() {
    stdio_init_all();

    // Set up the interrupt to trigger encoder_callback when rising edge is detected
    gpio_set_irq_enabled_with_callback(LEFT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);
    gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);
    
    // to change to level-triggered:
    // gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_LEVEL_HIGH, true, &encoder_callback);

    // Wait forever
    while (1);
}

