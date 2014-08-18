/*
 * Utilities
 */
#include <Arduino.h>
#include <stdint.h>
#include "thermo_display.h"
#include "thermo_config.h"

void halt_execution(uint8_t code) {
    display_msg(PREFIX_HALT, code);
    while(true) {
        delay(60000);
    }
}
