/*
 * Code for managing the WiFi connection.
 *
 */
#include <WiFi.h>
#include "thermo_util.h"
#include "thermo_wifi.h"
#include "thermo_config.h"
#include "thermo_display.h"
#include "private.h"

// Max number of times to retry conneciton to
// wifi network before giving up.
#define WIFI_MAX_RETRY 4

#define WIFI_RETRY_DELAY 2000

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

    // Make sure we have a wifi shield
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println(F("No wifi shield detected"));
        halt_execution(ERR_NO_WIFI_SHIELD);
    }
    display_msg(PREFIX_BOOT, BOOT_FOUND_WIFI_SHIELD);
    
    Serial.println(F("[WIFI] Initializing..."));
    while (retry < WIFI_MAX_RETRY) {
        Serial.print(F("[WIFI] attempt "));
        Serial.println(retry + 1);

        status = WiFi.begin(ssid, pass);
        if (status != WL_CONNECTED) {
            Serial.print(F("[WIFI] Unable to connect to "));
            Serial.print(ssid);
            Serial.println(F(" trying again"));
            retry++;
            delay(WIFI_RETRY_DELAY);
        }

        Serial.print(F("[WIFI] Connected to "));
        Serial.println(ssid);
        IPAddress localIp = WiFi.localIP();
        Serial.print(F("[WIFI]   - IP: "));
        Serial.println(localIp);

        return true;
    }

    Serial.println(F("[WIFI] Giving up"));
    return false;
}
