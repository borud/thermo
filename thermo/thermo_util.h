#ifndef THERMO_UTIL_H
#define THERMO_UTIL_H

/**
 * Just halt processing.  The idea is that if we call sleep we might
 * burn less power?  Needs to be checked.
 *
 * @param code the error code we want to display when halting.
 */
void halt_execution(uint8_t code);

/**
 * Reboot the board.  Experimental.  I have no idea how this really
 * works.  Assumes you do not have any working watchdog timers.
 */
void reboot();

/**
 * Estimate how much free memory there is.
 */
int free_memory();

/**
 * Output amount of free memory to serial port.  Assumes serial port
 * has been initialized.
 */
void print_free_memory();

#endif
