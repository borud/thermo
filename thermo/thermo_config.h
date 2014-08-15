#ifndef THERMO_CONFIG_H
#define THERMO_CONFIG_H

// Serial speed
#define SERIAL_SPEED 9600

// Configure which pins the MAX7219 is connected to.
//
#define MAX7219_LOAD       10
#define MAX7219_CLK         9
#define MAX7219_DATA_INPUT  8

// Thermometer
#define THERMOMETER_PIN              2
#define THERMOMETER_RESOLUTION_BITS 11

// Number of chained MAX7219 chips
#define MAX7219_NUM_DEVICES 1

// The display number
#define LED_DISPLAY 0

// Defaults
#define DEFAULT_INTENSITY 4

// How many samples do we perform smoothing over?  Higher number of
// samples results in slower response to change.
#define SMOOTHING_SIZE 6


// =============== not configurable ====================

// Error codes
#define ERR_NO_TEMPERATURE_SENSOR  1
#define ERR_NEGATIVE_TEMPERATURE   2

// Prefixes for use in thermo_display.cpp : display_msg()
#define PREFIX_ERR  'E'
#define PREFIX_BOOT 'b'

// Boot sequence codes
#define BOOT_TEMP_SENSOR_SCAN   1
#define BOOT_CONNECT_WIFI       2
#define BOOT_FINISHED          99

#endif
