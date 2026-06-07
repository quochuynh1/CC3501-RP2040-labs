#pragma once // only include header once per build/compilation
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Defining accelerometer registers
#define LIS3DH_WHO_AM_I_REG 0x0F // register address
#define LIS3DH_WHO_AM_I_VAL 0x33 // expected value
#define LIS3DH_CTRL_REG1 0x20 // section  8.8 of datasheet (20h) configure axises 
#define LIS3DH_CTRL_REG4 0x23 // section 8.11 of datasheet (23h) range of accelerometer
#define LIS3DH_OUT_X_L 0x28 // section 8.16 of datasheet (28h) x-axis acceleration data

struct AccelData { // define a struct for the raw data of the accelerometer
    int16_t x, y, z; 
};

class LIS3DH { 
    public: // accessible by user code
        void init(); 
        bool accel_write_register(uint8_t reg, uint8_t data); // a function that takes two parameters: the register address you want to write to, and the data you want to put in it; returns true if successful, false if something went wrong
        bool accel_read_register(uint8_t reg, uint8_t *data, uint8_t length); // a function that takes three parameters: the register to read; "data" is a pointer to a buffer into which the data will be placed; "length" s the number of bytes to read
        AccelData read_accel_data(); // a function to read the acceleration data from the device. uses multi-byte reading and left-justified shifting, combining into a 16-bit integer, and removing the 6 padding zeros (M)
        float accel_data_to_g(int16_t raw_data); // a function to convert from the sensor's raw integers to floating point numbers in units of g = 9.81m/s^2
}; 