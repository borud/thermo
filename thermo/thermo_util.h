#ifndef THERMO_UTIL_H
#define THERMO_UTIL_H

/**
 * Just halt processing.  The idea is that if we call sleep we might
 * burn less power?  Needs to be checked.
 *
 * @param code the error code we want to display when halting.
 */
void halt_execution(uint8_t code);

#endif