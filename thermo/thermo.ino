/*
 * Software for collecting and displaying temperature data.
 *
 * This is mainly a platform that I use for experimenting with
 * temperature sensors. As such this doesn't really represent a
 * finished product of any sort.  The code is mainly made available in
 * case it might be useful for other people.
 *
 * - borud@borud.org
 *
 */

// Kludge: This needs to be here in order to trick the IDE to update
// includepath for build process.
#include <DallasTemperature.h>
#include <OneWire.h>
#include <LedControl.h>
#include <WiFi.h>

// Project headers
#include "networking.h"
#include "thermo_config.h"
#include "thermo_display.h"
#include "thermo_sensor.h"

/**
 * Very simple smoothing function.
 */
float smooth(float temp) {
    static float readings[SMOOTHING_SIZE];
    static byte offset = 0;
    static byte count = 0;

    readings[offset] = temp;
    offset++;
    offset = offset % SMOOTHING_SIZE;

    // Only increment until smoothing array is filled.
    if (count < SMOOTHING_SIZE) {
        count++;
    }

    float sum = 0.0;
    for (byte i = 0; i < count ; i++) {
        sum += readings[i];
    }
    return (sum / (float)count);
}

/**
 * Initialize the display.  Puts it in power-save mode to avoid
 * display flickering and displaying nonsense during startup.  Then
 * set the default intensity and clear the display so the display is
 * in a known state when starting.
 */
void setup() {
    // initialize display before we do anything to avoid flicker and
    // nonsense.
    display_init();
    
    // Initialize serial interface.
    Serial.begin(SERIAL_SPEED);
    
    // Initial scan for DS18x20 sensors.
    display_msg(PREFIX_BOOT, BOOT_TEMP_SENSOR_SCAN);
    sensor_init();

    // Try to connect to wifi
    display_msg(PREFIX_BOOT, BOOT_CONNECT_WIFI);
    connect_to_wifi();

    // Boot sequence done
    display_msg(PREFIX_BOOT, BOOT_FINISHED);
}

/**
 * Main loop.
 */
void loop() {
    // Read thermometer
    float raw_temp = get_temperature_celsius();
    float temperature = smooth(raw_temp);

    // Print raw and smooted temperature
    Serial.print(raw_temp);
    Serial.print(", ");
    Serial.println(temperature);

    display_temperature(temperature);
}
