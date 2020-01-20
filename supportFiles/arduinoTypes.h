/*
 * arduinoTypes.h
 *
 *  Created on: Apr 25, 2014
 *      Author: hutch
 */

// This is just a software shim that maps types used in the Adafruit code to Xilinx types.

#ifndef ARDUINOTYPES_H_
#define ARDUINOTYPES_H_

#include "xil_types.h"

typedef s32 int32_t;
typedef u32 uint32_t;
typedef s16 int16_t;
typedef u16 uint16_t;
typedef u8 uint8_t;
//typedef s8 int8_t;
typedef signed char int8_t;  // Should be compatible with Xilinx and avoids conflict in stdint.h
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

#endif /* ARDUINOTYPES_H_ */
