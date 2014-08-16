#ifndef THERMO_SENSOR_H
#define THERMO_SENSOR_H

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
float sensor_get_celsius(byte index);

/**
 * Get a pointer to the address of sensor at index {@code index}.
 */
DeviceAddress* sensor_get_address(byte index);

/**
 * Get number of connected sensors.
 */
byte sensor_count();

#endif
