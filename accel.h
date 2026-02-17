#pragma once

#include <stdint.h>
#include <stdbool.h>

// Initialize the accelerometer's SPI interface and settings
void accel_init(void (*accel_cf)(int16_t x, int16_t y, int16_t z));

// Check the accelerometer's WHO_AM_I register
uint8_t accel_whoami(void);

// Check if new accelerometer data is available.
//
// Returns true if new data is available, false if not.
bool accel_data_ready(void);

// Read out X, Y, and Z axis 16-bit signed values from
// the accelerometer.  Should be called only after 
// accel_data_read() returns 'true'. 
//
// Returns X,Y,Z acceleration measurements  from the accelerometer in units
// of "milli-Gravities," where 1 Gravity is Earth's gravitational
// acceleration (~9.8 m/s^2)
void accel_read(int16_t *x, int16_t *y, int16_t *z);