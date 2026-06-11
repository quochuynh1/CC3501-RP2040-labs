#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "board.h"
#include "lis3dh.h"
#include "logging/logging.h" // allows log
#include <stdio.h> // allows printf

void LIS3DH::init() { 
    // Step 2. create a function to initialise your device driver.
    // Step 2a) use the i2c_init function to set up the RP2040’s I2C interface with a suitable baud rate
    i2c_init(ACCEL_I2C_INSTANCE, 400 * 1000); // from lecture notes
    
    // Step 2b) use gpio_set_function to configure the SDA and SCL pins to be set as GPIO_FUNC_I2C
    gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C); // from lecture notes 
    gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C); // from lecture notes

    // Step 2c) send I2C transaction ro read the value of the WHO_AM_I register and confirm that the returned
    // vlaue is correct. This will validate that I2C communications are working and the accelerometer is alive. 
    uint8_t who_am_i; 
    accel_read_register(LIS3DH_WHO_AM_I_REG, &who_am_i, 1); // '&' means "address of"

    if (who_am_i != LIS3DH_WHO_AM_I_VAL) { // if the who_am_i register does not match the expected value (defined in lis3dh.h)
        log(LogLevel::ERROR, "Accelerometer not found"); 
        return; 
    }
    log(LogLevel::INFORMATION, "Accelerometer found"); 

    // Step 2d) configre the accelerometer based on how you want to use it. Speecifically, you should check and configure the measurement
    // range and sample rate. This wil require you to use I2C transactions to write to specific registers. 
    // Sample rate - from datasheet CTRL_REG1 (20h = 0x20)
    accel_write_register(LIS3DH_CTRL_REG1, RATE_10HZ | X_ENABLE | Y_ENABLE | Z_ENABLE); // 10Hz, all axises enabled (default value: 1)
    
    // Measurement range - from datasheet CTRL_REG4 (23h = 0x23)
    accel_write_register(LIS3DH_CTRL_REG4, HIGH_RESOLUTION | RANGE_2G); // enable high resolution, range default (±2g apprpriate for spirit level application)
}

bool LIS3DH::accel_write_register(uint8_t reg, uint8_t data) { 
    // Hint - It is strongly recommended that you write functions to enable you to read and write accelerometer registers over I2C. 
    // Using example code from lecture notes to get started
    uint8_t buf[2]; // create a 2-byte buffer
    buf[0] = reg; // first byte is the register address - telling the slave which memory location to write to (e.g., the control register)
    buf[1] = data; // second byte is the actual data - the value you want to store there (e.g., a mode configuration) 

    int bytes_written = i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_ADDRESS, buf, 2, false); // calls the SDK function to perform the actual I2C 
    // write (I2C_INSTANCE: which bus (i2c0), set up previously in init(); ACCEL_ADDRESS: the slave address; buf: the buffer defined just above; 
    // 2: tells it there are 2 bytes to send; false: end the transactiona fter this)
    
    if (bytes_written != 2) { // from starter code
        log(LogLevel::ERROR, "Failed to write to accelerometer register."); 
        return false; 
    }
    return true; 
}

bool LIS3DH::accel_read_register(uint8_t reg, uint8_t *data, uint8_t length) { 
    // Hint - These functions will be used within your device driver in multiple places. the example code fragments in the lecture slides will get you started. 
    // Tell the device which address we want to read

    // Feedback from Laurence: wrap the auto incremnent inside of accel_read_register(), if the number of bytes we want to read is more than 1, 
    // enable auto increment
    if (length > 1) { 
        reg |= 0x80; // enable auto-incrementing
    }

    if (1 != i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_ADDRESS, &reg, 1, true)) { 
        // You need to pass the pointer to the register address because
        // i2c_write_blocking expects a pointer to a buffer of data. 
        log(LogLevel::ERROR, "lis3dh::read_registers: Failed to select register address."); 
        return false; 
    }

    // Now read the data (from lecture notes)
    int bytes_read = i2c_read_blocking(ACCEL_I2C_INSTANCE, ACCEL_ADDRESS, data, length, false); 
    if (bytes_read != length) { 
        log(LogLevel::ERROR, "lis3dh::read_registers: Failed to read data."); 
        return false; 
    }
    return true; 
}

AccelData LIS3DH::read_accel_data() { 
    // Step 3. Create a function to read the acceleration data from the device. 
    // Step 3a) Perform a multi-byte read to obtain the most recently measured accelerations
    // along X,Y, and Z axes in a single transaction
    uint8_t raw_data[6]; // from lecture notes
    uint8_t reg = LIS3DH_OUT_X_L; // set SUB(7) = 1 to enable sequential multi-byte read
    accel_read_register(reg, raw_data, 6); // perform the multi-byte read using the function made above

    // Step 3b) Convert these to a signed 16-bit integers and right shift them correctly, as per 
    // the discussion in the lecture notes. 
    AccelData data; // declare a struct to hold the raw accelerometer readings 
    data.x = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> 4;// from lecture notes
    data.y = (int16_t)(raw_data[2] | (raw_data[3] << 8)) >> 4; // feedback from Laurence: top of datasheet says accel is 16-bit data output
    data.z = (int16_t)(raw_data[4] | (raw_data[5] << 8)) >> 4; // therefore, don't need to >> M = 6
    // Double check this: from what I understand, because we set the accelerometer to Normal Mode, section 3.2.1 says that it's 10-bit data output
    // and therefore, you would need to right shift by 6. But, as Laurence pointed out, I should configure the accelerometer in High-resolution mode. 
    // Still need to right shift by M = 4 even in HR mode though. Speak with Laurence or email Luke. 
    return data;  
} 

float LIS3DH::accel_data_to_g(int16_t raw_data) { 
    // Step 4. Create a function to convert from the sensor's raw integers to floating point numbers in units of g = 9.81m/s^2 (the
    // strength of gravity on Earth's surface). 

    // For this, you will need to find the sensityivy of the accelerometer from the datasheet to obtain the conversion factor. 
    // From datasheet: when FS bit set to High-resolution, sensitivity = 1 mg/digit
    return (raw_data * 1.0f)/1000.0f; // multiply by sensitivity factor, then divide by 1000 to convert mg to g
}