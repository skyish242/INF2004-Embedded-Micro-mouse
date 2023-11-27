#include "magnometer.h"

void magnometer_init(){
   // Initialise default I2C and set up output
    init_i2c_default();

   // Initialize LSM303DLH accelerometer and magnetometer (accelerometer not printed in this code but is defined, if needed in future)
   lsm303dlh_acc_setup();
   lsm303dlh_mag_setup();
}

int get_direction() {
   // Check if the default I2C pins are defined, which will be GPIO 4 (SDA) and 5 (SCL), both I2C0
   
   #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
      #warning Default I2C pins were not defined
      puts("Default I2C pins were not defined");
   #else

   // Declare a variable to store magnetormeter readings
   mag_t mag;

   // Continuous loop to read magnetometer data and compute the angle
   // Here is where we print the X, Y, Z values and the bearings
   
   lsm303dlh_read_mag(&mag);
   int32_t angle = get_angle(&mag);
   // printf("X = %d, Y = %d, Z = %d, Angle = %d degrees\n", mag.x, mag.y, mag.z, angle);
   return angle;
   

   #endif
}

// Function to initialise default I2C settings
void init_i2c_default() {
    // Initialise I2C with the defined baud rate
   i2c_init(i2c_default, I2C_BAUD * 1000);
    
   // Set the SDA and SCL pins for I2C comms
   gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
   gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

   // Enable pull-ups for SDA and SCL pins
   gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
   gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

// Function to configure LSM303DLH accelerometer settings
void lsm303dlh_acc_setup() {
   // Initialise control register
   uint8_t buffer[2] = {CTRL_REG_4, 0x00};

   // Write configuration to the accelerometer over I2C
   i2c_write_blocking(
      i2c_default,
      INTERFACE_A,
      buffer,
      2,
      true
   );

   buffer[0] = CTRL_REG_1;
   buffer[1] = 0x27;

   // Write configuration to the accelerometer over I2C
   i2c_write_blocking(
      i2c_default,
      INTERFACE_A,
      buffer,
      2,
      false
   );

}

// Function to configure LSM303DLH magnetometer settings
// This function mainly writes to the 3 different registers, CRA, CRB and MR
// It is important to write to these registers first before reading later on
void lsm303dlh_mag_setup() {
   uint8_t buffer[2] = {MAG_CRA, 0x10}; // CRA default value is 0x10, as written in the HMC5883L report
   
    // Write configuration to the magnetometer over I2C
    // Argument 1 - Instance of I2C peripheral, in this case I2C0 is default
    // Argument 2 - Address of the I2C slave device, in this case Magnetometer address in the LSM303DLH
    // Argument 3 - Pointer to the data source to be written
    // Argument 4 - Length of the data to be written
    // Argument 5 - If set to true, you can continute with a I2C read operation later on
   i2c_write_blocking(
      i2c_default,
      INTERFACE_B,
      buffer,
      2,
      true
   );

   buffer[0] = MAG_CRB;
   buffer[1] = 0xE0; // Gain config and range
   
   i2c_write_blocking(
      i2c_default,
      INTERFACE_B,
      buffer,
      2,
      true
   );

   buffer[0] = MAG_MR;
   buffer[1] = 0x00; // Continuous-conversion mode
   
   i2c_write_blocking(
      i2c_default,
      INTERFACE_B,
      buffer,
      2,
      false
   );

}

// Function to read data from LSM303DLH accelerometer
void lsm303dlh_read_acc(accel_t *acc) {
   uint8_t buffer[6];
   int16_t accel[3];
   uint8_t reg = ACC_REG;

   i2c_write_blocking(
      i2c_default,
      INTERFACE_A,
      &reg,
      1,
      true
   );

   i2c_read_blocking(
      i2c_default,
      INTERFACE_A,
      buffer,
      6,
      false
   );

   // Conver byte data to int16_t
   for (int i = 0; i < 3; i++) {
      accel[i] = ((buffer[i * 2] | buffer[(i * 2) + 1]  << 8));
   }

   acc->x = accel[0];
   acc->y = accel[1];
   acc->z = accel[2];
}

// Function to read data from LSM303DLH magnetometer
void lsm303dlh_read_mag(mag_t *mag) {
   uint8_t buffer[6];
   int16_t magnet[3];
   uint8_t reg = MAG_REG;
    
    // This function is not sending configuration or command data. It is used to set
    // the register address for the subsequent read operation below. This is common
    // for the I2C devices. Before reading data, you need to specify the address of
    // the data/register you're interested in
   i2c_write_blocking(
      i2c_default,
      INTERFACE_B,
      &reg,
      1,
      true
   );

   i2c_read_blocking(
      i2c_default,
      INTERFACE_B,
      buffer,
      6,
      false
   );

   // Convert byte data to int16_t
    // After reading the data, the function processes the byte array
    // to convert and store the magnetometer readings in the mag_t struct
   for (int i = 0; i < 3; i++) {
      magnet[i] = ((buffer[i * 2] << 8 | buffer[(i * 2) + 1] ));
   }

   mag->x = magnet[0];
   mag->y = magnet[1];
   mag->z = magnet[2];
}

// Function to compute the angle based on magnetometer readings
int32_t get_angle(mag_t *mag) {
   // Calculate Heading in radians
   double angle_rad = atan2(mag->x, mag->y);

   // Convert radian to degree
    // The +180 degrees is to re-zeroing the angle to standard
    // compass convetions where 0 degree is North. 
    // DO NOTE: When placed on the car, the issue will not lie in this function
    // You could however guess and check the 180 degrees to re-zero the angles
    // But the magnetic inteference might not make it uniform, so you would need
    // Kalman Filter or Madgwick or Mahony filters to fuse these sensor readings
    // Simple solution to to place the magnetometer somewhere else on the car
   double angle_deg = (angle_rad * (180.0 / M_PI)) + 180;

    // If the angle is below 0, don't go to negative but go back to 360 degrees
   if (angle_deg < 0) 
   {
      angle_deg += 360;
   }

   return (int32_t) angle_deg;
}