/*
 * leds.h
 *
 *  Created on: Mar 24, 2014
 *      Author: hutch
 */

#ifndef LEDS_H_
#define LEDS_H_
#include <stdbool.h>

// This will init the GPIO hardware so you can write to the 4 LEDs  (LD3 - LD0) on the ZYBO board.
// if printFailedStatusFlag = 1, it will print out an error message if an internal error occurs.
int leds_init(bool printFailedStatusFlag);

// This write the lower 4 bits of ledValue to the LEDs.
// LED3 gets bit3 and so forth.
// '1' = illuminated.
// '0' = off.
void leds_write(int ledValue);

// These control the LED LD4 attached to MIO 7 on the ZYBO board.
void leds_writeLd4(int ledValue);

// This blinks all of the LEDs for several seconds to provide a visual test of the code.
int leds_runTest();

#endif /* LEDS_H_ */
