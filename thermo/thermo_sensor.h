#ifndef THERMO_SENSOR_H
#define THERMO_SENSOR_H

/**
 * Initialize DS18x20 temperature sensor.
 */
void sensor_init();
float get_temperature_celsius();

#endif

