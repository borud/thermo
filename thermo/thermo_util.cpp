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

int free_memory () {
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void print_free_memory() {
    Serial.print(F("free memory : "));
    Serial.println(free_memory());
}
