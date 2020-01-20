/*
 * mio.c
 *
 *  Created on: Mar 25, 2014
 *      Author: hutch
 */

#include <stdio.h>
#include "mio.h"
#include "xgpiops.h"

// The MIO system is the PS GPIO that communicates with the MIO pins on the ZYBO board.
// MIO pins: 13, 10, 11, 12, 0, 9, 14, 15 are connected to JF pins: 1, 2, 3, 4, 7, 8, 9, 10.
// MIO pin 7 is connected to LED LD4 (located to the right of JF).
// BTN5 is connected to MIO51
// BTN4 is connected to MIO50

static bool mioInitFlag = false;  // Ensures the init routine will be invoked only once.
static XGpioPs_Config *ConfigPtr; // Handle to the mio config handle.
XGpioPs mioGpio;	              // The driver instance for GPIO Device.

// Initializes the MIO system and sets up directions for the LEDs and buttons.
// The boolean parameter allows you to enable/disable the printing of error messages.
int mio_init(bool printFailedStatusFlag){
  if (!mioInitFlag) {  // Only do this once.
    ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);  // Lookup the config info for the MIO GPIO device.
	int status = XGpioPs_CfgInitialize(&mioGpio, ConfigPtr, ConfigPtr->BaseAddr);  // Initialize it..
	if (status != XST_SUCCESS) {   // Check for errors.
	  if (printFailedStatusFlag) { // Print an error message if enabled.
	    printf("XGpioPs_CfgInitialize filed in initMio().\n\r");
	  }
      return XST_FAILURE;  // Return the value of the failure code to aid debugging.
	}
  }
  // Setup direction and enable for LED4 on ZYBO board.
  XGpioPs_SetDirectionPin(&mioGpio, MIO_LD4_MIO_PIN, 1);
  XGpioPs_SetOutputEnablePin(&mioGpio, MIO_LD4_MIO_PIN, 1);
  // Setup direction for BTN4 and BTN5 on ZYBO board.
  XGpioPs_SetDirectionPin(&mioGpio, MIO_BTN4_MIO_PIN, 0);
  XGpioPs_SetDirectionPin(&mioGpio, MIO_BTN5_MIO_PIN, 0);
  mioInitFlag = true;
  return 0;
}

// Provides a more readable function name and also checks to see if the init routine was called.
void mio_writePin(u8 mioPinNumber, u8 value){
  if (!mioInitFlag){
    printf("writeMioPin: must call initMio() first.\n\r");
  } else {
    XGpioPs_WritePin(&mioGpio, mioPinNumber, value);
  }
}

// Does the write as a single bank write. Checks to see that the init routine was called.
void mio_WriteBank0(u32 value) {
  if (!mioInitFlag){
    printf("mio_writeBank: must call initMio() first.\n\r");
  } else {
    XGpioPs_Write(&mioGpio, 0, value);
  }
}

uint16_t mio_readBank0() {
  if (!mioInitFlag){
    printf("mio_readBank: must call initMio() first.\n\r");
  }
  return XGpioPs_Read(&mioGpio, 0);
}

// Checks to see if the init routine was called and then reads a pin.
u8 mio_readPin(u8 mioPinNo) {
  if (!mioInitFlag){
	printf("writeMioPin: must call initMio() first.\n\r");
  } else {
    return XGpioPs_ReadPin(&mioGpio, mioPinNo);
  }
  return 0;
}

// Checks for proper init and configures the pin as an input.
void mio_setPinAsInput(u8 mioPinNo) {
  if (!mioInitFlag){
	printf("setMioPinDirection(): must call initMio() first.\n\r");
  } else {
	  XGpioPs_SetDirectionPin(&mioGpio, mioPinNo, MIO_INPUT_PIN_CONFIGURATION);
  }
}

// Checks for proper init and configures the pin as an output.
void mio_setPinAsOutput(u8 mioPinNo) {
  if (!mioInitFlag){
	printf("setMioPinDirection(): must call initMio() first.\n\r");
  } else {
    XGpioPs_SetDirectionPin(&mioGpio, mioPinNo, MIO_OUTPUT_PIN_CONFIGURATION);  // This configures the output direction.
    XGpioPs_SetOutputEnablePin(&mioGpio, mioPinNo, 1);  // This enables the output.
  }
}




