#ifndef THERMO_SENSOR_H
#define THERMO_SENSOR_H

#include <DallasTemperature.h>

/**
 * Initialize DS18x20 temperature sensor.
 */
void sensor_init();

/**
 * Issue a request for temperatures.  Use this before looping through
 * to get results.
 */
void sensor_request_temperatures();

/**
 * Get temperature in celsius from the sensor given by the
 * {@code index} argument.
 *
 * @param index the index of the sensor to be read.  Starts with 0.
 * @return the temperature in celsius
 */
float sensor_get_celsius(uint8_t index);

/**
 * Get a pointer to the address of sensor at index {@code index}.
 */
DeviceAddress* sensor_get_address(uint8_t index);

/**
 * Get number of connected sensors.
 */
uint8_t sensor_count();

/**
 * Get sensor name.  String containing hexadecimal address
 * representation.
 */
char* sensor_get_name(uint8_t index);

/**
 * Get all sensor names.
 */
char** sensor_get_names();

#endif
