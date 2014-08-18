#ifndef THERMO_DISPLAY_H
#define THERMO_DISPLAY_H

/**
 * Display temperature on LED display.  If an error is detected the
 * appropriate error code is printed.
 */
void display_temperature(float temp);

/**
 * Display numeric error code.
 *
 * @param prefix single character prefix.  Can be one of
 *   '0','1','2','3','4','5','6','7','8','9','0',
 *   'A','b','c','d','E','F','H','L','P',
 * @param code a numeric code between 0 and 99.
 */
void display_msg(char prefix, uint8_t code);

/**
 * Initialize display.  This sets the display in power save mode,
 * clears the display and sets the intensity.
 */
void display_init();

/**
 * Clear display
 */
void display_clear();

#endif
