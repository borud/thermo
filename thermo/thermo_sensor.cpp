#include <DallasTemperature.h>
#include <OneWire.h>
#include "thermo_config.h"
#include "thermo_util.h"
#include "thermo_sensor.h"
#include "thermo_display.h"

static OneWire onewire(SENSOR_PIN);
static DallasTemperature ds18x20(&onewire);
static DeviceAddress sensor_addr[SENSOR_MAX_NUMBER];
static char sensor_str[SENSOR_MAX_NUMBER][16+3+1];

void sensor_init() {
    memset(sensor_str, 0, sizeof(sensor_str));

    ds18x20.begin();
    Serial.print(F("Found "));
    Serial.print(ds18x20.getDeviceCount(), DEC);
    Serial.println(F(" DS18x20 device(s)."));

    // If there are no temperature sensors connected we reboot until
    // at least one is connected.
    if (ds18x20.getDeviceCount() == 0) {
        display_msg(PREFIX_REBOOT, ERR_NO_TEMPERATURE_SENSOR);
        delay(2000);
        display_clear();
        reboot();
    }

    // Look up all sensors and record address in sensor_addr array
    // for later use.
    for (uint8_t i = 0; i < ds18x20.getDeviceCount(); i++) {
        if (!ds18x20.getAddress(sensor_addr[i], i)) {
            // For some reason this failed.  Halt execution.
	  halt_execution(ERR_GET_SENSOR_ADDRESS_FAILED);
        }

        sprintf(sensor_str[i], "%02X%02X-%02X%02X-%02X%02X-%02X%02X",
                sensor_addr[i][0],
                sensor_addr[i][1],
                sensor_addr[i][2],
                sensor_addr[i][3],
                sensor_addr[i][4],
                sensor_addr[i][5],
                sensor_addr[i][6],
                sensor_addr[i][7]);
        
        // Set the resolution
        ds18x20.setResolution(sensor_addr[i], SENSOR_RESULTION_BITS);
        
        Serial.print(" - sensor ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.print(sensor_str[i]);
        Serial.println();
    }
}

void sensor_request_temperatures() {
    ds18x20.requestTemperatures();
}

float sensor_get_celsius(uint8_t index) {
    // If the index is out of bounds we pretend it was disconnected.
    if (index > ds18x20.getDeviceCount()) {
        halt_execution(ERR_SENSOR_INDEX_OUT_OF_BOUNDS);
    }
    return ds18x20.getTempC(sensor_addr[index]);
}

DeviceAddress* sensor_get_address(uint8_t index) {
    if (index > ds18x20.getDeviceCount()) {
        halt_execution(ERR_SENSOR_INDEX_OUT_OF_BOUNDS);
    }
    return &sensor_addr[index];
}

char* sensor_get_name(uint8_t index) {
    return sensor_str[index];
}

char** sensor_get_names() {
    return (char**)sensor_str;
}

uint8_t sensor_count() {
    return ds18x20.getDeviceCount();
}
