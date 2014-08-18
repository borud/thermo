/*
 * Utilities
 */
#include <Arduino.h>
#include <stdint.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "thermo_display.h"
#include "thermo_config.h"

void halt_execution(uint8_t code) {
    display_msg(PREFIX_HALT, code);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    // Give it some time to flush serial
    delay(5000);

    // Go to sleep
    sleep_mode();
}

void reboot() {
    wdt_enable(WDTO_15MS);
    while(1);
}
