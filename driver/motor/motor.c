/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../motor/motor.h"
#include "math.h"

float integral = 0.0;
float prev_error = 0.0;
bool turn_left = false;
bool turn_right = false;

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
float compute_pid(float setpoint, float current_val, float *integral, float *prev_error, float Kp, float Ki, float Kd){
    // Compute error
    float error = setpoint - current_val;
    
    // Update integral term
    *integral += error;
    
    // Compute derivative term
    float derivative = error - *prev_error;
    
    // Compute control signal
    float control_signal = Kp * error + Ki * (*integral) + Kd * derivative;
    
    // Update previous error
    *prev_error = error; 
    
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
    // Period of PWM signal = ((1 / 12.5Mhz) * 12500) = 10ms
    pwm_set_wrap(slice_num, 12500);

    // Enable PWM
    pwm_set_enabled(slice_num, true);
}

// Set the speed for the left motor
void setMotorLeft() {
    
    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(LEFT_MOTOR_PWM);

    uint16_t val = compute_pid(SETPOINT, left_speed, &integral , &prev_error, KP_LEFT, KI_LEFT, KD_LEFT);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to pwm_value
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LEFT_MOTOR_PWM), val);
}

// Set the speed for the right motor
void setMotorRight() {
    
    // Find out which PWM slice is connected to the GPIO 
    uint slice_num = pwm_gpio_to_slice_num(RIGHT_MOTOR_PWM);

    uint16_t val = compute_pid(SETPOINT, right_speed, &integral, &prev_error, KP_RIGHT, KI_RIGHT, KD_RIGHT);

    // Set duty cycle of PWM signal on channel A of the specified PWM slice to pwm_value
    pwm_set_chan_level(slice_num,  pwm_gpio_to_channel(RIGHT_MOTOR_PWM), val);
}

void setSpeed(int16_t pwm_value){
    uint slice_num = pwm_gpio_to_slice_num(RIGHT_MOTOR_PWM);
    pwm_set_chan_level(slice_num,  pwm_gpio_to_channel(RIGHT_MOTOR_PWM),(uint16_t)pwm_value);
    uint slice_num2 = pwm_gpio_to_slice_num(LEFT_MOTOR_PWM);
    pwm_set_chan_level(slice_num2,  pwm_gpio_to_channel(LEFT_MOTOR_PWM),(uint16_t)pwm_value);
}

// Move backward
void moveBackward() {
   // Motor A
    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 1);
    gpio_put(IN4, 0);
}

// Move forward 
void moveForward() {

    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 1);

}

// Move left
void moveLeft() {

    // Motor A
    gpio_put(IN1, 1);
    gpio_put(IN2, 0);

    // Motor B
    gpio_put(IN3, 0);
    gpio_put(IN4, 1);
}

// Move right
void moveRight() {
    // Motor A
    gpio_put(IN1, 0);
    gpio_put(IN2, 1);

    // Motor B
    gpio_put(IN3, 1);
    gpio_put(IN4, 0);
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

int getAngleTurned(int initialBearing){
    int currentBearing = get_direction();
    if(currentBearing == initialBearing){
        return 360;
    }
    float angleTurned = fmod(currentBearing - initialBearing + 360.0, 360.0);
    return angleTurned;    
}

void turnLeft(){
    // Constants for turning
    const int targetNotchCount = LEFT_ANGLE * 20 / 360; // 90 degrees in encoder notches

    // Reset encoders
    resetNotchCount();

    //Set the motor to move left mode
    moveLeft();
    // While the average of the left and right notch counts is less than the target count
    while ((getLeftNotchCount() + getRightNotchCount()) / 2 < targetNotchCount) {
  
        // Control motors to turn left
        setSpeed(BASE_SPEED);

        sleep_ms(100);
    }

    // Stop motors after reaching the target notch count  
    stopMotors();
}

void shakeLeft(){
    // Constants for turning
    const int targetNotchCount = 5 * 20 / 360; // 90 degrees in encoder notches

    // // Reset encoders
    // resetNotchCount();
    int left_counter = 0;
    int right_counter = 0;

    //Set the motor to move left mode
    moveLeft();
    // While the average of the left and right notch counts is less than the target count
    while ((left_counter + right_counter) / 2 < targetNotchCount) {
  
        // Control motors to turn left
        setSpeed(SHAKE_SPEED);
        left_counter += 1;
        right_counter +=1;
        if(detectLine(1) < LINE_THRESHOLD)
            turn_left = true;
        if(detectLine(1) > LINE_THRESHOLD)
            turn_left = false;
        sleep_ms(100);
    }
    stopMotors();
}


void turnRight(){
    // Constants for turning
    const int targetNotchCount = RIGHT_ANGLE * 20 / 360; // 90 degrees in encoder notches

    // Reset encoders
    resetNotchCount();

    //Set the motor to move left mode
    moveRight();

    // While the average of the left and right notch counts is less than the target count
    while ((getLeftNotchCount() + getRightNotchCount()) / 2 < targetNotchCount) {
  
        // Control motors to turn left
        setSpeed(BASE_SPEED);

        sleep_ms(100);
    }

    // Stop motors after reaching the target notch count
    stopMotors();
}


void shakeRight(){
    // Constants for turning
    const int targetNotchCount = 5 * 20 / 360; // 90 degrees in encoder notches
    int left_counter = 0;
    int right_counter = 0;
    // // Reset encoders
    // resetNotchCount();

    //Set the motor to move left mode
    moveRight();
    printf("%d\n", targetNotchCount);
    // While the average of the left and right notch counts is less than the target count
    while ((left_counter + right_counter) / 2 < targetNotchCount) {
        // Control motors to turn left
        setSpeed(SHAKE_SPEED);
        left_counter += 1;
        right_counter +=1;
        if(detectLine(0) < LINE_THRESHOLD)
            turn_right = true;
        if(detectLine(0) > LINE_THRESHOLD)
            turn_right = false;
        sleep_ms(100);
    }

    stopMotors();
}

int main(){
    // Initialisation
    stdio_init_all();
    motorInit();
    // Init IR Sensors
    ir_init();
      // Init Magnometer
    magnometer_init();

    // Set up the interrupt to trigger encoder_callback when rising edge is detected
    gpio_set_irq_enabled_with_callback(LEFT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);
    gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);

    while(1) {
        while(getLeftEncoderDistance() < 20 || getRightEncoderDistance() < 20){
            // printf("Right; %f\n", getLeftEncoderDistance());
            // printf("Left; %f\n", getRightEncoderDistance());
            moveForward();
            setMotorLeft();
            setMotorRight();
    
            // if (detectLine(1) > LINE_THRESHOLD && detectLine(0) < LINE_THRESHOLD){ //Black Left
            //     shakeRight();
            //     // continue;
            // }
            // else if (detectLine(0) > LINE_THRESHOLD && detectLine(1) < LINE_THRESHOLD){ // Black Right
            //     shakeLeft();
            //     // continue;
            // }
            if(!(getLeftEncoderDistance() > 16) || !(getRightEncoderDistance() > 16)){
                if (detectLine(1) > LINE_THRESHOLD ){ // Black left and no black right
                    shakeRight();
                    shakeLeft();
                }
                else if(detectLine(0) > LINE_THRESHOLD ){ // Black Right and no black left
                    shakeLeft();
                    shakeRight();   
                }
            }
        }    
        stopMotors();
        resetNotchCount();
        resetEncoderDistance();
    
        if(turn_left){
            printf("left\n");
            // shakeLeft();
            // shakeRight();
            // if(detectLine(1) > LINE_THRESHOLD){
            //     shakeRight();
            // }else{
            //     shakeRight();
            //     turnLeft();
            // }
            turnLeft();
            turn_left =false;  
            sleep_ms(1000);
            continue;
        }else if(turn_right){
            printf("Right\n");
            // shakeRight();
            // shakeLeft();
            // if(detectLine(0) > LINE_THRESHOLD){
            //     shakeLeft();
           
            // }else{
            //     shakeLeft();
            //     turnRight();
            // }
            turnRight();
            turn_right = false;
            sleep_ms(1000);
            continue;
        }else if(detectLine(2) > LINE_THRESHOLD){
            moveBackward();
            while(getLeftEncoderDistance() < 20 || getRightEncoderDistance() < 20){
                setMotorLeft();
                setMotorRight();
            }
            resetNotchCount();
            resetEncoderDistance();
            stopMotors();
            sleep_ms(1000);
            continue;
        }

        sleep_ms(1000);
    }
    //stopMotors();
    // while(1){
    //     while(getLeftEncoderDistance() < 17 && getRightEncoderDistance() < 17){
    //         moveForward();
    //         setMotorLeft();
    //         setMotorRight();
    //     }
    //     setSpeed(0);
    //     stopMotors();   
    //     resetNotchCount();
    //     resetEncoderDistance();
    //     stopMotors();   
    //     sleep_ms(1000);
    // }
    // while(1){
    //     moveForward();
    //     setMotorLeft();
    //     setMotorRight();
    //     shakeLeft();
    //     sleep_ms(1000);
    // }
}
