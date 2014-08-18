/*
 * Code for managing the WiFi connection.
 *
 */
#include <WiFi.h>
#include "thermo_util.h"
#include "thermo_wifi.h"
#include "private.h"

// Max number of times to retry conneciton to
// wifi network before giving up.
#define WIFI_MAX_RETRY 4

// Wifi variables - you need to add a wifi_pass.h file where you
// define WIFI_SSID and WIFI_PASS.  This is so you won't have to
// check this file in and reveal your password.
//
static char ssid[] = WIFI_SSID;
static char pass[] = WIFI_PASS;

// Wifi Status
static int status = WL_IDLE_STATUS;

bool connect_to_wifi() {
    byte retry = 0;

    Serial.println("[WIFI] Initializing...");
    while (retry < WIFI_MAX_RETRY) {
        Serial.print("[WIFI] attempt ");
        Serial.println(retry + 1);

        status = WiFi.begin(ssid, pass);
        if (status != WL_CONNECTED) {
            Serial.print("[WIFI] Unable to connect to ");
            Serial.print(ssid);
            Serial.println(" trying again");
            retry++;
        }

        Serial.print("[WIFI] Connected to ");
        Serial.println(ssid);
        return true;
    }

    Serial.println("[WIFI] Giving up");
    return false;
}
