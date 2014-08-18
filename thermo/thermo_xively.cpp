#include <HttpClient.h>
#include <WiFi.h>
#include <Xively.h>
#include "private.h"
#include "thermo_util.h"
#include "thermo_config.h"
#include "thermo_sensor.h"

/*
 * Need to rewrite this code so it sucks a bit less.  This is a pretty
 * inelegant way of doing it. :-(
 */

static char xivelyKey[] = XIVELY_API_KEY;

// Prototype datastream to avoid wasting more memory than we need.
static XivelyDatastream prototype = XivelyDatastream("0", 1, DATASTREAM_FLOAT);

// Initial array of prototype datastreams.  Yech.
static XivelyDatastream datastreams[] = {
    prototype,
    prototype,
    prototype,
    prototype
};
    
static XivelyFeed feed(XIVELY_FEED, datastreams, 4 /* Update this if you change the max number of datastreams*/);
static WiFiClient client;
static XivelyClient xivelyclient(client);
static uint8_t n;

void xively_init(uint8_t num_sensors) {
    n = num_sensors;
    for (uint8_t i = 0; i < n; i++) {
        // Replace data streams with datastreams that are properly
        // named so we can map sensor ID to Xively Channels.
        datastreams[i] = XivelyDatastream(sensor_get_name(i),
                                          strlen(sensor_get_name(i)),
                                          DATASTREAM_FLOAT);
        // Initialize them to 0.0
        datastreams[i].setFloat(0.0);
    }
    feed = XivelyFeed(XIVELY_FEED, datastreams, n);
}

void xively_update_value(uint8_t sensor, float value) {
    if (sensor > n) {
        // Halt if we get a sensor that is out of bounds.  Programming
        // error.  Stop and display error.
        halt_execution(ERR_SENSOR_INDEX_OUT_OF_BOUNDS_XIVELY);
    }
    datastreams[sensor].setFloat(value);
}

void xively_push() {
    int ret = xivelyclient.put(feed, xivelyKey);
    Serial.print("[xivelyclient.put] res = ");
    Serial.println(ret);
}
