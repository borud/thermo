#ifndef THERMO_XIVELY_H
#define THERMO_XIVELY_H

void xively_init(uint8_t num_sensors);
void xively_update_value(uint8_t sensor, float value);
void xively_push();

#endif
