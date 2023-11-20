#ifndef MOTOR_H
#define MOTOR_H


#include <stdio.h>
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
#define DUTY_CYCLE 65535/2

// Define PID Controller Parameters 
#define SETPOINT 50
#define KP_RIGHT 1.0
#define KI_RIGHT 0.1
#define KD_RIGHT 0.01
#define KP_LEFT 1.0
#define KI_LEFT 0.1
#define KD_LEFT 0.01

void motorInit();
void setupPWM(uint gpio);
void setMotorLeft();
void setMotorRight();

void moveForward();
void moveBackward();
void moveLeft();
void moveRight();


void motor_init();

#endif /* MOTOR_H */
