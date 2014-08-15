#include <DallasTemperature.h>
#include <OneWire.h>
#include "thermo_sensor.h"
#include "thermo_config.h"
#include "thermo_display.h"

// DS18x20 digital thermometer chip
static OneWire onewire(THERMOMETER_PIN);
static DallasTemperature ds18x20(&onewire);
static DeviceAddress thermometer;

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
    Serial.println("Looking for temperature sensor");
    ds18x20.begin();
    Serial.print("Found ");
    Serial.print(ds18x20.getDeviceCount(), DEC);
    Serial.println(" devices.");
        
    if (!ds18x20.getAddress(thermometer, 0)) {
        Serial.println("Unable to find temperature sensor");
        display_msg(PREFIX_ERR, ERR_NO_TEMPERATURE_SENSOR);
    }
    ds18x20.setResolution(thermometer, THERMOMETER_RESOLUTION_BITS);
}

float get_temperature_celsius() {
    ds18x20.requestTemperatures();
    return ds18x20.getTempC(thermometer);
}
