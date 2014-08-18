#ifndef THERMO_CONFIG_H
#define THERMO_CONFIG_H

// Determine whether or not we want to enable Wifi.  Turning this off
// is useful for development since connecting to Wifi is slow.
#undef ENABLE_WIFI

// Serial speed
#define SERIAL_SPEED 9600

// Configure which pins the MAX7219 is connected to.
//
#define MAX7219_LOAD       10
#define MAX7219_CLK         9
#define MAX7219_DATA_INPUT  8

// Pin the DS18x20 OneWire bus is connected to
#define SENSOR_PIN             2

// Number of bits resolution.
#define SENSOR_RESULTION_BITS 11

// Max number of DS18x20 sensors we support
#define SENSOR_MAX_NUMBER 3

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
#define ERR_NO_TEMPERATURE_SENSOR        1
#define ERR_NEGATIVE_TEMPERATURE         2
#define ERR_SENSOR_INDEX_OUT_OF_BOUNDS  20

// Prefixes for use in thermo_display.cpp : display_msg()
#define PREFIX_ERR    'E'
#define PREFIX_BOOT   'b'
#define PREFIX_SENSOR 'L'

// Boot sequence codes
#define BOOT_TEMP_SENSOR_SCAN   1
#define BOOT_CONNECT_WIFI       2
#define BOOT_FINISHED          99

#endif
