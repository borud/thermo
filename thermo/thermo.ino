/*
 * Software for collecting and displaying temperature data.
 *
 * This is mainly a platform that I use for experimenting with
 * temperature sensors. As such this doesn't really represent a
 * finished product of any sort.  The code is mainly made available in
 * case it might be useful for other people.
 *
 * IMPORTANT: to configure parameters such as pins used on the Arduino
 *   etc. you have to look in the "thermo_config.h" file.  This is
 *   where all global configuration is located.
 *
 * - Bjorn Borud, borud (at) borud.org
 *
 */

// Kludge: This needs to be here in order to trick the IDE to update
// includepath for build process.
#include <DallasTemperature.h>
#include <OneWire.h>
#include <LedControl.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Xively.h>

// Project headers.
#include "thermo_wifi.h"
#include "thermo_config.h"
#include "thermo_display.h"
#include "thermo_sensor.h"
#include "thermo_xively.h"
#include "thermo_util.h"

// Array used for rudimentary smoothing.
static float smoothed_values[SENSOR_MAX_NUMBER];

// Time for last data push
static unsigned long last_data_push = 0;

/**
 * Just prepopulate the array with the current temperature of each
 * sensor so that we don't have to wait for a long ramp-up.  Measured
 * values are better guesses than...well, guesses.
 */
void smoothing_init() {
    sensor_request_temperatures();
    for (byte i = 0; i < sensor_count(); i++) {
        smoothed_values[i] = sensor_get_celsius(i);
    }
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
    smoothing_init();

    // Try to connect to wifi
    display_msg(PREFIX_BOOT, BOOT_CONNECT_WIFI);
    connect_to_wifi();

    // Initialize xively
    xively_init(sensor_count());

    // Boot sequence done
    display_msg(PREFIX_BOOT, BOOT_FINISHED);
}

/**
 * Main loop.
 */
void loop() {
    // Request temperature readings then loop through the sensors.
    sensor_request_temperatures();
    for (byte i = 0; i < sensor_count(); i++) {
        DeviceAddress *address;
        float temp = sensor_get_celsius(i);

        // Giving the measured value a weight of 1/4 seems like a good
        // tradeoff between speed and useful smoothness.
        smoothed_values[i] = (((smoothed_values[i] * 300.0) + (temp * 100.0)) / 400.0);

        xively_update_value(i, smoothed_values[i]);
        
        Serial.print(temp);
        Serial.print(", ");
        Serial.print(smoothed_values[i]);
        Serial.print(", ");
    }
    Serial.println(0);

    // Display only first sensor
    display_temperature(smoothed_values[0]);

    // Push data
    {
        unsigned long now = millis();
        if ((now - last_data_push) > XIVELY_UPDATE_DELAY_MILLIS) {
            xively_push();
            last_data_push = now;
        }
    }
}
