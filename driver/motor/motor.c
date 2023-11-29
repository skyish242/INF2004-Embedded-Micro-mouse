/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../motor/motor.h"
#include "math.h"

float integral = 0.0;
float prev_error = 0.0;

float adc_rolling_avg_left;
float adc_rolling_sum_left = 0;

float adc_rolling_avg_right;
float adc_rolling_sum_right = 0;

// Rolling window to average the current adc reading
static float adc_data_left[10] = {};
static float adc_data_right[10] = {};
static int rolling_index_left = 0;
static int rolling_count_left = 0;
static int rolling_index_right = 0;
static int rolling_count_right = 0;

volatile bool left_wall;
volatile bool right_wall;

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

    left_wall = false;
    right_wall = false;

    // Init IR Sensors
    // ir_init();
    // magnometer_init();
    // printf("Magnometer initialised\n");
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
    const int targetNotchCount = LEFT_ANGLE * ENCODER_NOTCHES / 360; // 90 degrees in encoder notches

    // Reset encoders
    resetNotchCount();

    //Set the motor to move left mode
    moveLeft();
    bool continue_turn = false;

    // While the average of the left and right notch counts is less than the target count
    while ((getLeftNotchCount() + getRightNotchCount()) / 2 < targetNotchCount || continue_turn) {
  
        // Control motors to turn left
        setSpeed(BASE_SPEED);
        //printf("Meow: %d\n", detectLine(1));
        if(getLeftNotchCount() > 3 ){
            if(detectLine(1) < LINE_THRESHOLD){
                // Middle of turning and detcted line on right
                continue_turn = true;
            }else{
                continue_turn = false;
                break;
            }
       
        }

        if(getLeftNotchCount() > 13){
            continue_turn = false;
            break;
        }

        sleep_ms(100);
    }

    // Stop motors after reaching the target notch count  
    stopMotors();
    sleep_ms(100);

    if(detectLine(1) < LINE_THRESHOLD){
        while(detectLine(1) < LINE_THRESHOLD){
            shakeRight();
            sleep_ms(100);
        }
    }

}

void shakeLeft(){
    // Constants for turning
    const float targetNotchCount = (float)4 * 20 / 360; // 90 degrees in encoder notches
    
    // // Reset encoders
    // resetNotchCount();
    uint32_t left_counter = 0;
    uint32_t right_counter = 0;

    //Set the motor to move left mode
    moveLeft();
    // While the average of the left and right notch counts is less than the target count
    while ((left_counter + right_counter) / 2 < targetNotchCount) {
  
        // Control motors to turn left
        setSpeed(SHAKE_SPEED);
        left_counter += 1;
        right_counter +=1;
      
        sleep_ms(100);
    }
    stopMotors();
}


void turnRight(){
    // Constants for turning
    const int targetNotchCount = RIGHT_ANGLE * ENCODER_NOTCHES / 360; // 90 degrees in encoder notches

    // Reset encoders
    resetNotchCount();

    //Set the motor to move left mode
    moveRight();

    bool continue_turn = false;
    
    // While the average of the left and right notch counts is less than the target count
    while ((getLeftNotchCount() + getRightNotchCount()) / 2 < targetNotchCount || continue_turn) {
        // Control motors to turn left
        setSpeed(BASE_SPEED);

        if(getLeftNotchCount() > 3 ){
            if(detectLine(0) < LINE_THRESHOLD){
                // Middle of turning and detcted line on right
                continue_turn = true;
            }else{
                continue_turn = false;
                break;
            }
       
        }

        if(getLeftNotchCount() > 13){
            continue_turn = false;
            break;
        }
        sleep_ms(100);
    }
        
    // Stop motors after reaching the target notch count
    stopMotors();
    sleep_ms(100);

    if(detectLine(0) < LINE_THRESHOLD){ //Overshot to the right 
        while(detectLine(0) < LINE_THRESHOLD){
            shakeLeft();
            sleep_ms(100);
        }
    }

}


void shakeRight(){
    // Constants for turning(
    const float targetNotchCount = (float)4 * 20 / 360; // 90 degrees in encoder notches
    uint32_t left_counter = 0;
    uint32_t right_counter = 0;
    // // Reset encoders
    // resetNotchCount();

    //Set the motor to move left mode
    moveRight();
    // While the average of the left and right notch counts is less than the target count
    while ((left_counter + right_counter) / 2 < targetNotchCount) {

        // Control motors to turn left
        setSpeed(SHAKE_SPEED);
        left_counter += 1;  
        right_counter +=1;
    
        sleep_ms(100);
    }

    stopMotors();
}

void calib_right_ir(int reference_bearing){
    int current_bearing = get_direction();
    int deviation = ( (current_bearing - reference_bearing) + 180) % 360 - 180;

    if(deviation > 0){ // Deviated to the right 
        while(detectLine(0) < LINE_THRESHOLD){
            shakeLeft();
            sleep_ms(100);
        }
    }else if(deviation < 0){ // Devaited to the left 
        while(detectLine(0) < LINE_THRESHOLD){
            shakeRight();
            sleep_ms(100);
        }
    }

}
void calib_left_ir(int reference_bearing){
    int current_bearing = get_direction();
    int deviation = ( (current_bearing - reference_bearing) + 180) % 360 - 180;

    if(deviation > 0){ // Deviated to the right 
        while(detectLine(1) < LINE_THRESHOLD){
            shakeLeft();
            sleep_ms(100);
        }
    }else if(deviation < 0){ // Devaited to the left 
        while(detectLine(1) < LINE_THRESHOLD){
            shakeRight();
            sleep_ms(100);
        }
    }
}

void moveOneCell(){
    bool turn_left = false;
    bool turn_right = false;

    left_wall = false;
    right_wall = false;

    bool i_have_turned = false; 
    int max_distance;

    int reference_bearing = get_direction();
    adc_rolling_sum_right = 0.0;
    adc_rolling_avg_right = 0.0;
    memset(adc_data_right, 0, sizeof(adc_data_right));
    rolling_index_right = 0;
    rolling_count_right = 0;

    adc_rolling_sum_left = 0.0;
    adc_rolling_avg_left = 0.0;   
    // Rolling window to average the current adc reading
    memset(adc_data_left, 0, sizeof(adc_data_left));
    rolling_index_left = 0;
    rolling_count_left = 0; 

    resetNotchCount();
    resetEncoderDistance();

    busy_wait_ms(100);

    // if(i_have_turned){
    //     max_distance = 17;
    // }else{
    //     max_distance = 17;
    // }

    // while(getLeftEncoderDistance() < max_distance || getRightEncoderDistance() < max_distance){

    //     if(!left_wall && detectLine(0) < LINE_THRESHOLD){ //there is nothing on left, so check right 
    //         stopMotors();
    //         sleep_ms(80);
    //         calib_right_ir(reference_bearing);
    //     } 
    //     if(!right_wall && detectLine(1) < LINE_THRESHOLD){ // there is nothing on right, so check left
    //         stopMotors();
    //         sleep_ms(80);
    //         calib_left_ir(reference_bearing);   
    //     }

    //     moveForward();
    //     setMotorLeft();
    //     setMotorRight();


    //     if( (!(getLeftEncoderDistance() > 15) || !(getRightEncoderDistance() > 15)) && getLeftEncoderDistance() > 5){
        
    //         int adc_left = detectLine(1);
    //         int adc_right = detectLine(0);
            
    //         adc_rolling_sum_left -= adc_data_left[rolling_index_left];
    //         adc_data_left[rolling_index_left] = adc_left;
    //         adc_rolling_sum_left += adc_data_left[rolling_index_left];
    //         rolling_index_left = (rolling_index_left + 1) % 10;

    //         if (rolling_count_left < 10) {
    //             rolling_count_left++;
    //         }

    //         adc_rolling_avg_left = adc_rolling_sum_left / rolling_count_left;

            
    //         adc_rolling_sum_right -= adc_data_right[rolling_index_right];
    //         adc_data_right[rolling_index_right] = adc_right;
    //         adc_rolling_sum_right += adc_data_right[rolling_index_right];
    //         rolling_index_right = (rolling_index_right + 1) % 10;

    //         if (rolling_count_right < 10) {
    //             rolling_count_right++;
    //         }

    //         adc_rolling_avg_right = adc_rolling_sum_right / rolling_count_right;

    //     }

    //     // Black on left and white on right
    //     if (adc_rolling_avg_left > LINE_THRESHOLD && adc_rolling_avg_right < LINE_THRESHOLD){
    //         //if previously detected no left wall
    //         if(!left_wall){
    //             left_wall = true;
    //         }
    //         right_wall = false;
    //     }

    //     if (adc_rolling_avg_right > LINE_THRESHOLD && adc_rolling_avg_left < LINE_THRESHOLD) {
    //         if(!right_wall){
    //             right_wall = true;
    //         }
    //         left_wall= false;
    //     }

    //     if (adc_rolling_avg_right > LINE_THRESHOLD && adc_rolling_avg_left > LINE_THRESHOLD) {
    //         left_wall = true;
    //         right_wall = true;
    //     }
    // }    

    // stopMotors();

    // //Calibration
    // if(!left_wall){
    //     calib_right_ir(reference_bearing);
  
    //     if(detectLine(1) > LINE_THRESHOLD){
    //         left_wall = true;          
    //     }

    // }
    // if(!right_wall){
    //     calib_left_ir(reference_bearing);

    //     if(detectLine(0) > LINE_THRESHOLD){
    //         right_wall = true;
    //     }
    // }

}

// int main(){
//     // Initialisation
//     stdio_init_all();
//     motorInit();
//     // Init IR Sensors
//     ir_init();
//       // Init Magnometer
//     magnometer_init();

//     // Set up the interrupt to trigger encoder_callback when rising edge is detected
//     gpio_set_irq_enabled_with_callback(LEFT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);
//     gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_callback);

//     // int reference_bearing = get_direction();
//     // printf("Reference: %d\n", reference_bearing);
//           bool i_have_turned = false; 
//           int max_distance;
//     while(1) {
//         turn_left = false;
//         turn_right = false;
//         int reference_bearing = get_direction();
//         adc_rolling_sum_right = 0.0;
//         adc_rolling_avg_right = 0.0;
//         memset(adc_data_right, 0, sizeof(adc_data_right));
//         rolling_index_right = 0;
//         rolling_count_right = 0;

//         adc_rolling_sum_left = 0.0;
//         adc_rolling_avg_left = 0.0;   
//         // Rolling window to average the current adc reading
//         memset(adc_data_left, 0, sizeof(adc_data_left));
//         rolling_index_left = 0;
//         rolling_count_left = 0; 

//         resetNotchCount();
//         resetEncoderDistance();

//         if(i_have_turned){
//             max_distance = 17;
//         }else{
//             max_distance = 17;
//         }

//         while(getLeftEncoderDistance() < max_distance || getRightEncoderDistance() < max_distance){

//             if(turn_left && detectLine(0) < LINE_THRESHOLD){ //there is nothing on left, so check right 
//                 stopMotors();
//                 sleep_ms(80);
//                 calib_right_ir(reference_bearing);
//             } 
//             if(turn_right && detectLine(1) < LINE_THRESHOLD){ // there is nothing on right, so check left
//                 stopMotors();
//                 sleep_ms(80);
//                 calib_left_ir(reference_bearing);   
//             }

//             moveForward();
//             setMotorLeft();
//             setMotorRight();

    
//             if( (!(getLeftEncoderDistance() > 15) || !(getRightEncoderDistance() > 15)) && getLeftEncoderDistance() > 5){
          
//                 int adc_left = detectLine(1);
//                 int adc_right = detectLine(0);
                
//                 adc_rolling_sum_left -= adc_data_left[rolling_index_left];
//                 adc_data_left[rolling_index_left] = adc_left;
//                 adc_rolling_sum_left += adc_data_left[rolling_index_left];
//                 rolling_index_left = (rolling_index_left + 1) % 10;

//                 if (rolling_count_left < 10) {
//                     rolling_count_left++;
//                 }

//                 adc_rolling_avg_left = adc_rolling_sum_left / rolling_count_left;

                
//                 adc_rolling_sum_right -= adc_data_right[rolling_index_right];
//                 adc_data_right[rolling_index_right] = adc_right;
//                 adc_rolling_sum_right += adc_data_right[rolling_index_right];
//                 rolling_index_right = (rolling_index_right + 1) % 10;

//                 if (rolling_count_right < 10) {
//                     rolling_count_right++;
//                 }

//                 adc_rolling_avg_right = adc_rolling_sum_right / rolling_count_right;

//             }

//             if (adc_rolling_avg_left > LINE_THRESHOLD && adc_rolling_avg_right < LINE_THRESHOLD){
//                 if(turn_left){
//                     turn_left = false;
//                 }
//                 turn_right = true;
//             }

//             if (adc_rolling_avg_right > LINE_THRESHOLD && adc_rolling_avg_left < LINE_THRESHOLD) {
//                 if(turn_right){
//                     turn_right = false;
//                 }
//                 turn_left = true;
//             }

//             if (adc_rolling_avg_right > LINE_THRESHOLD && adc_rolling_avg_left > LINE_THRESHOLD) {
//                 turn_right = false;
//                 turn_left = true;
//             }
//         }    

//         stopMotors();
//         printf("left: %d\n", turn_left);
//         printf("right: %d\n", turn_right);


//         // printf("Encoder %f\n", getRightEncoderDistance());
//         // if(detectLine(2) > LINE_THRESHOLD){
//         //     moveBackward();
                
//         // }

//         if(turn_left){
//             calib_right_ir(reference_bearing);
//             // turnLeft();
//             if(detectLine(1) < LINE_THRESHOLD){
                 
//                 turnLeft();
//             }
//             turn_left =false;  
//             i_have_turned = true;
//             // sleep_ms(1000);
//             continue;
//         }else if(turn_right){
//             calib_left_ir(reference_bearing);
    
//             // turnLeft();
//             if(detectLine(0) < LINE_THRESHOLD){
//                 turnRight();
//             }
//             turn_right = false;
//             i_have_turned = true;
//             sleep_ms(1000);
//             continue;
//         }



//         // }else if(detectLine(2) > LINE_THRESHOLD){
//         //     moveBackward();
//         //     while(getLeftEncoderDistance() < 20 || getRightEncoderDistance() < 20){
//         //         setMotorLeft();
//         //         setMotorRight();
//         //     }
//         //     resetNotchCount();
//         //     resetEncoderDistance();
//         //     stopMotors();
//         //     sleep_ms(1000);
//         //     continue;
//         // }

//         resetNotchCount();
//         resetEncoderDistance();
//         i_have_turned =false;
//         sleep_ms(1000);
//     }

// }
