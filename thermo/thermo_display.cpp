#include <LedControl.h>
#include "thermo_config.h"
#include "thermo_display.h"
#include "thermo_util.h"

// Parameters to the shutdown command
#define POWER_DOWN true
#define POWER_SAVE false

// Decimal dot
#define DECIMAL_DOT_ON  true
#define DECIMAL_DOT_OFF false

// MAX7219 LED controller
static LedControl lc = LedControl(MAX7219_DATA_INPUT, MAX7219_CLK, MAX7219_LOAD, MAX7219_NUM_DEVICES);

// Conversion buffer.  Make sure we never exceed 6 characters
// (including terminating null)
static char buffer[6];


/**
 * Convert float to string.  Very limited funcitonality.  Just make
 * sure there is enough room in the buffer or things will become very
 * sad.
 */
void float_to_string(float f, char buffer[]) {
    // Minimum width 5, 2 digits after decimal point.
    dtostrf((double)f, 5, 2, buffer);
}

void display_temperature(float temp) {
    // Show error on negative values
    if (temp < 0.0) {
        display_msg(PREFIX_ERR, ERR_NEGATIVE_TEMPERATURE);
        return;
    }
    
    // Sensor returns 0.0 if unplugged.  Can use this float
    // representation to check for this case.
    if (temp == 0.0) {
        Serial.println(F("Sensor unplugged"));
        halt_execution(ERR_NO_TEMPERATURE_SENSOR);
    }
    
    float_to_string(temp, buffer);
    lc.setChar(LED_DISPLAY,1, buffer[0], DECIMAL_DOT_OFF);
    lc.setChar(LED_DISPLAY,2, buffer[1], DECIMAL_DOT_ON);
    lc.setChar(LED_DISPLAY,3, buffer[3], DECIMAL_DOT_OFF);
    lc.setChar(LED_DISPLAY,4, buffer[4], DECIMAL_DOT_OFF);
}

void display_msg(char prefix, uint8_t code) {
    lc.setChar(LED_DISPLAY,1, prefix, DECIMAL_DOT_OFF);
    lc.setChar(LED_DISPLAY,2, '-', DECIMAL_DOT_OFF);
    if (code < 10) {
        lc.setChar(LED_DISPLAY,3, '0', DECIMAL_DOT_OFF);
        lc.setChar(LED_DISPLAY,4, code, DECIMAL_DOT_OFF);
        return;
    }
    {
        uint8_t first  = code/10;
        uint8_t second = code - (first * 10);
        lc.setChar(LED_DISPLAY, 3, first, DECIMAL_DOT_OFF);
        lc.setChar(LED_DISPLAY, 4, second, DECIMAL_DOT_OFF);
    }
}

void display_clear() {
    lc.shutdown(LED_DISPLAY, POWER_SAVE);
    lc.clearDisplay(LED_DISPLAY);
}

void display_init() {
    display_clear();
    lc.setIntensity(LED_DISPLAY, DEFAULT_INTENSITY);
}
