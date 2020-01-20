/*
 * leds.c
 *
 *  Created on: Mar 24, 2014
 *      Author: hutch
 */

#include "leds.h"
#include "mio.h"
#include "xgpio.h"
#include "stdio.h"

static XGpio Gpio_LEDs;  // Instance of the LED GPIO Driver
static int initLedFlag = 0;  // This will be '1' if LEDs have been initialized.

#define LD3_LD2_LD1_LD0_ON 0xF
#define LD3_LD2_LD1_LD0_OFF 0x0
#define LED_LD4_ON 1
#define LED_LD4_OFF 0

#define GPIO_DIRECTION_IS_OUTPUT 0x0
#define GPIO_DIRECTION_IS_INPUT 0xF
#define GPIO_CHANNEL_NUMBER 1

#define STATUS_OK 0
#define STATUS_FAIL 1

// This will init the GPIO hardware so you can write to the 4 LEDs  (LED3 - LED0) on the ZYBO board.
int leds_init(bool printFailedStatusFlag) {
  int status;				// Used to report Xilinx status.
	if (initLedFlag)  // Only invoke the init routine once.
		return STATUS_OK;
  status = XGpio_Initialize(&Gpio_LEDs, XPAR_LEDS_DEVICE_ID);
  if (status != XST_SUCCESS) {
  	if (printFailedStatusFlag) {
  		printf("XGPIO_Initialize (leds) failed\n\r.");
  	}
  	return STATUS_FAIL;
  }
  // Also init LD4, connected to MIO7. Also inits the entire MIO system.
  mio_init(printFailedStatusFlag);
  // Set the direction for all signals to be outputs (0 = output).
  XGpio_SetDataDirection(&Gpio_LEDs, GPIO_CHANNEL_NUMBER, GPIO_DIRECTION_IS_OUTPUT);
  initLedFlag=1;
  return STATUS_OK;
}

// This write the lower 4 bits of ledValue to the LEDs.
// LED3 gets bit3 and so forth.
// '1' = illuminated.
// '0' = off.
void leds_write(int ledValue) {
  if (initLedFlag) {
    XGpio_DiscreteWrite(&Gpio_LEDs, GPIO_CHANNEL_NUMBER, ledValue);
  } else {
  	printf("Error: you must invoke initializeLeds() prior to calling writeLeds()\n\r.");
  }
}

// Access LD4 via MIO.
void leds_writeLd4(int ledValue){
  mio_writePin(MIO_LD4_MIO_PIN, ledValue);
}

// This blinks all of the LEDs for several seconds to provide a visual test of the code.
// This will use a simple for-loop as a delay to keep the code independent of other code.
// Always returns 0 because this is strictly a visual test.
#define LEDS_TEST_LENGTH_IN_SECONDS 5
#define LEDS_FOR_LOOP_COUNT_FOR_ONE_BLINK 1000000
int leds_runTest() {
  leds_init(true);
  int i, j;
  for (i=0; i<LEDS_TEST_LENGTH_IN_SECONDS; i++) {
	  for (j=0; j<LEDS_FOR_LOOP_COUNT_FOR_ONE_BLINK; j++) {
	    leds_write(LD3_LD2_LD1_LD0_ON);  // All LEDs are turned on.
	    leds_writeLd4(LED_LD4_ON);
	  }
	  for (j=0; j<LEDS_FOR_LOOP_COUNT_FOR_ONE_BLINK; j++) {
	    leds_write(LED_LD4_OFF);  // All LEDs are turned on.
	    leds_writeLd4(LD3_LD2_LD1_LD0_OFF);
	  }
  }
  return 0;
}








