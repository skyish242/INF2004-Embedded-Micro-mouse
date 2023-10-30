#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

void motorInit();
void setupPWM(uint gpio);
void setMotorLeft(int16_t pwm_value);
void setMotorRight(int16_t pwm_value);
void moveForward();
void moveBackward();
void moveLeft();
void moveRight();

#endif /* MOTOR_H */
