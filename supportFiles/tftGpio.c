/*
 * TFTGPIO.c
 *
 *  Created on: Apr 30, 2014
 *      Author: hutch
 */

#include "xgpio.h"
#include "TFTGPIO.h"
#include <stdio.h>
#include "arduinoTypes.h"

static XGpio gpioTft;  // Instance of the LED GPIO Driver
static bool initFlag = false;

void TFTGPIO_init() {
  if (initFlag)
    return;  // Already initialized, just return.
  int status;
  status = XGpio_Initialize(&gpioTft, TFTGPIO_DEVICE_ID);
  if (status != XST_SUCCESS) {
    printf("XGPIO_Initialize (TFT) failed\n\r.");
  }
  // Set the direction for all signals to be outputs (0 = output).
  XGpio_SetDataDirection(&gpioTft, 1, 0);  // 0 is a bit-mask that will set all pins as outputs.
  initFlag = true;
}

void TFTGPIO_setCommandMode() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTft, 1);
  XGpio_DiscreteWrite(&gpioTft, 1, regValue & ~TFTGPIO_DCX_BIT_MASK);  // Clears the DCX bit.
}

void TFTGPIO_setDataMode() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTft, 1);
  XGpio_DiscreteWrite(&gpioTft, 1, regValue | TFTGPIO_DCX_BIT_MASK);  // Sets the DCX bit.
}

void TFTGPIO_assertRd() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTft, 1);
  XGpio_DiscreteWrite(&gpioTft, 1, regValue & ~TFTGPIO_RD_BIT_MASK);  // Asserts RD
}

void TFTGPIO_negateRd() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTft, 1);
  XGpio_DiscreteWrite(&gpioTft, 1, regValue | TFTGPIO_RD_BIT_MASK);  // Negates RD
}

void TFTGPIO_assertWr() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTft, 1);
  XGpio_DiscreteWrite(&gpioTft, 1, regValue & ~TFTGPIO_WR_BIT_MASK);  // Asserts WR
}

void TFTGPIO_negateWr() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTft, 1);
  XGpio_DiscreteWrite(&gpioTft, 1, regValue | TFTGPIO_WR_BIT_MASK);  // Negates WR
}

