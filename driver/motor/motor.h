#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "encoder.h"
#include "magnometer.h"
#include "irline.h"

// Define GPIO pins for motor input
#define IN1 13  // Input 1 for Motor A
#define IN2 12  // Input 2 for Motor A
#define IN3 11  // Input 1 for Motor B
#define IN4 10  // Input 2 for Motor B

// #define IN1 6  // Input 1 for Motor A
// #define IN2 7  // Input 2 for Motor A
// #define IN3 3  // Input 1 for Motor B
// #define IN4 4  // Input 2 for Motor B

// Define GPIO pins for PWM
#define LEFT_MOTOR_PWM 0
#define RIGHT_MOTOR_PWM 1

// Define duty cycle to be 50%
#define DUTY_CYCLE 12500/2

// Define PID Controller Parameters 
// #define SETPOINT 40
// #define KP_RIGHT 2.0
// #define KI_RIGHT 2.0
// #define KD_RIGHT 0.0
// #define KP_LEFT 2.0
// #define KI_LEFT 2.0
// #define KD_LEFT 0.0

#define SETPOINT 30
#define KP_RIGHT 2.2
#define KI_RIGHT 2.0
#define KD_RIGHT 0.0
#define KP_LEFT 1.8
#define KI_LEFT 2.0
#define KD_LEFT 0.0

// Define turning angle
// Small: 
// Big: 
#define LEFT_ANGLE 90
#define RIGHT_ANGLE 90
#define BASE_SPEED 25000
#define SHAKE_SPEED 12000

void 
otorInit();
void setupPWM(uint gpio);
void setMotorLeft();
void setMotorRight();

void moveForward();
void moveBackward();
void moveLeft();
void moveRight();

void stopMotors();
void setSpeed(int16_t pwm_value);

void turnLeft();
void shakeLeft(bool to_check);
void turnRight();
void shakeRight(bool to_check);

#endif /* MOTOR_H */