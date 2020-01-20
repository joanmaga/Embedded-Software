/*
 * mio.h
 *
 *  Created on: Mar 25, 2014
 *      Author: hutch
 */

#ifndef MIO_H_
#define MIO_H_
#include <stdbool.h>
#include <stdint.h>
#include "xil_types.h"

// Provides an API for easy access to various MIO pins as they are allocated on the ZYBO board.
// This includes LED4 and BTN4 and BTN5.

// Pin numbers for MIO pins on the ZYBO board.
#define MIO_LD4_MIO_PIN 7
#define MIO_BTN5_MIO_PIN 51
#define MIO_BTN4_MIO_PIN 50
#define MIO_OUTPUT_PIN_CONFIGURATION 1
#define MIO_INPUT_PIN_CONFIGURATION 0

// Needs to be called before trying to read or write MIO pins.
int mio_init(bool printFailedStatusFlag);

// Reads an MIO pin.
u8 mio_readPin(u8 mioPinNumber);

// Writes an MIO pin.
void mio_writePin(u8 mioPinNumber, u8 value);

// Writes 16 bits to bank 0.
void mio_WriteBank0(u32 value);

// Reads 16 bits from bank 1.
uint16_t mio_readBank0();

// Set MIO pin as input from ZYNQ perspective.
void mio_setPinAsInput(u8 mioPinNo);

// Set MIO pin as output and enabled it, from ZYNQ perspective.
void mio_setPinAsOutput(u8 mioPinNo);

#endif /* MIO_H_ */
