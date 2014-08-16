#include <DallasTemperature.h>
#include <OneWire.h>
#include "thermo_sensor.h"
#include "thermo_config.h"
#include "thermo_display.h"

static OneWire onewire(SENSOR_PIN);
static DallasTemperature ds18x20(&onewire);
static DeviceAddress sensor_addr[SENSOR_MAX_NUMBER];

/**
 * Just halt processing.  Should move this to a shared file since it
 * is useful.  The idea is that if we call sleep we might burn less
 * power?  Needs to be checked.
 */
void halt_execution(byte code) {
    display_msg(PREFIX_ERR, code);
    while(true) {
        delay(60000);
    }
}

/**
 * Print thermometer address.
 */
void print_address(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void sensor_init() {
    ds18x20.begin();
    Serial.print("Found ");
    Serial.print(ds18x20.getDeviceCount(), DEC);
    Serial.println(" DS18x20 device(s).");

    // Look up all sensors and record address in sensor_addr array
    // for later use.
    for (byte i = 0; i < ds18x20.getDeviceCount(); i++) {
        if (!ds18x20.getAddress(sensor_addr[i], i)) {
            // For some reason this failed.  Sleep endlessly.
            display_msg(PREFIX_ERR, ERR_NO_TEMPERATURE_SENSOR);
            while(true) {
                delay(60000);
            }
        }

        // Set the resolution
        ds18x20.setResolution(sensor_addr[i], SENSOR_RESULTION_BITS);

        Serial.print(" - sensor ");
        Serial.print(i);
        Serial.print(" : ");
        print_address(sensor_addr[i]);
        Serial.println();
    }
}

void sensor_request_temperatures() {
    ds18x20.requestTemperatures();
}

float sensor_get_celsius(byte index) {
    // If the index is out of bounds we pretend it was disconnected.
    if (index > ds18x20.getDeviceCount()) {
        halt_execution(ERR_SENSOR_INDEX_OUT_OF_BOUNDS);
    }
    return ds18x20.getTempC(sensor_addr[index]);
}

DeviceAddress* sensor_get_address(byte index) {
    if (index > ds18x20.getDeviceCount()) {
        halt_execution(ERR_SENSOR_INDEX_OUT_OF_BOUNDS);
    }
    return &sensor_addr[index];
}

byte sensor_count() {
    return ds18x20.getDeviceCount();
}
