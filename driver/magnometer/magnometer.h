// Include necessary libraries for input/output, mathematics, and data types
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

// Library for standard operations in Raspberry Pi Pico
#include "pico/stdlib.h"
#include "pico/binary_info.h"

// Library for I2C communication in Raspberry Pi Pico
#include "hardware/i2c.h"

// Define constants related to I2C communication and register addresses.
#define I2C_BAUD 400 // 400 or 100 (kHz)
#define REFRESH_PERIOD 100 // Data refresh period in ms

#define INTERFACE_A 0x19 // I2C address for the accelerometer
#define INTERFACE_B 0x1E // I2C address for the magnetometer

// Register addresses for accelerometer setup and data reading
#define CTRL_REG_1 0x20
#define CTRL_REG_4 0x23
#define ACC_REG 0xA8

// Register addresses for magnetometer setup and data reading
// More info can be found in https://www.digikey.in/en/htmldatasheets/production/786600/0/0/1/asd2613-r
// Pages 12 - 14 more on CRA, CRB and MR (Configuration Register A, Configuration Register B and Mode register)
#define MAG_CRA 0x00
#define MAG_CRB 0x01
#define MAG_MR 0x02
#define MAG_REG 0x03

// Data structures to hold accelerometer readings
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} accel_t;

// Data structures to hold magnetometer readings
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mag_t;

// Declare functions for later use
int get_direction();
void lsm303dlh_acc_setup();
void lsm303dlh_mag_setup();
void lsm303dlh_read_acc(accel_t *acc);
void lsm303dlh_read_mag(mag_t *mag);
int32_t get_angle(mag_t *mag);
void init_i2c_default();

void magnometer_init();