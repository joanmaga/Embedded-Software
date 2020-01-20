/*
 * lcd.c
 *
 *  Created on: Jul 1, 2014
 *      Author: hutch
 */

#include "lcd.h"
#include "arduinoTypes.h"
#include "mio.h"

static XGpio gpioTftControl;  // Provides the RD, WR and CD pins for the LCD controller.
static XGpio gpioTftDataBus;  // Provides an 8-bit data bus for the LCD controller.
static bool initFlag = false; // Make sure that body of init routine only gets invoked once.

// This init intializes all of the hardware that talks to the LCD panel.
void LCD_init() {
//  printf("LCD_init called.\n\r");
  if (initFlag)  // Check to see if init has already been called.
    return;      // Already initialized, just return.
  int status;    // First call, continue...
  status = XGpio_Initialize(&gpioTftControl, LCD_CONTROL_DEVICE_ID);  // Xilinx GPIO init call.
  if (status != XST_SUCCESS) {
    printf("XGPIO_Initialize (TFT) failed\n\r.");
  }
  status = XGpio_Initialize(&gpioTftDataBus, LCD_DATA_BUS_DEVICE_ID); // Xilinx GPIO init call.
  if (status != XST_SUCCESS) {
    printf("XGPIO_Initialize (TFT Data Bus) failed\n\r.");
  }
  // Set the direction for all signals to be outputs (0 = output, 1 = input).
  XGpio_SetDataDirection(&gpioTftControl, 1, 0);  // Control bits are always outputs.
  XGpio_SetDataDirection(&gpioTftDataBus, 1, 0);  // Set up data-bus direction as output (write).
  mio_init(true);
  LCD_negateRd();  // negate the RD control signal.
  LCD_negateWr();  // negate the WR control signal.
  initFlag = true; // Note that init has been invoked.
}

// Sets the logic value on the command/data pin for the LCD controller to command mode.
void LCD_setCommandMode() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTftControl, 1);
  XGpio_DiscreteWrite(&gpioTftControl, 1, regValue & ~LCD_DCX_BIT_MASK);  // Clears the DCX bit.
}

// Sets the logic value on the command/data pin for the LCD controller to data mode.
void LCD_setDataMode() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTftControl, 1);
  XGpio_DiscreteWrite(&gpioTftControl, 1, regValue | LCD_DCX_BIT_MASK);  // Sets the DCX bit.
}

// Set the logic value on the LCD RD pin for read operations for the LCD data bus.
void LCD_assertRd() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTftControl, 1);
  XGpio_DiscreteWrite(&gpioTftControl, 1, regValue & ~LCD_RD_BIT_MASK);  // Asserts RD
}

// Set the logic value on the LCD RD pin to disable read operations on the LCD data bus.
void LCD_negateRd() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTftControl, 1);
  XGpio_DiscreteWrite(&gpioTftControl, 1, regValue | LCD_RD_BIT_MASK);  // Negates RD
}

// Set the logic value on the LCD WR pin to enable write operations on the LCD data bus.
void LCD_assertWr() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTftControl, 1);
  XGpio_DiscreteWrite(&gpioTftControl, 1, regValue & ~LCD_WR_BIT_MASK);  // Asserts WR
}

// Set the logic value on the LCD WR pin to disable write operations on the LCD data bus.
void LCD_negateWr() {
  uint32_t regValue = XGpio_DiscreteRead(&gpioTftControl, 1);
  XGpio_DiscreteWrite(&gpioTftControl, 1, regValue | LCD_WR_BIT_MASK);  // Negates WR
}


// These are utility functions

// Most of these delay functions are disabled as they were not found to be necessary
// during testing. They are kept around to act as place-holders in the original source
// code from the Adafruit site.

// millisecond delay
void LCD_delay(uint16_t delay){
//  volatile uint32_t count;
//  count = delay * LCD_MILLISECOND_DELAY_COUNT;
//  while (count--);
}

// microsecond delay
void LCD_delayMicroseconds(uint16_t delay){
//  volatile uint32_t count;
//  count = delay * LCD_MICROSECOND_DELAY_COUNT;
//  while (count--);
}

// 10 nanosecond delay
void LCD_delay10Nanoseconds(uint16_t delay){
//  volatile uint32_t count;
//  count = delay * LCD_10_NANOSECOND_DELAY_COUNT;
//  while (count--);
}

// Set the GPIO pins on the LCD data bus for read operations.
void LCD_setReadDataDirection() {
  LCD_negateWr();  // Negate the WR pin to be consistent.
  XGpio_SetDataDirection(&gpioTftDataBus, 1, 0xFF);  // LCD data bus is input from ZYNQ perspective.
}

// Set the GPIO pins on the LCD data bus for write operations.
void LCD_setWriteDataDirection() {
  LCD_negateRd();  // Take care to make sure that two drivers are not enabled at the same time.
  XGpio_SetDataDirection(&gpioTftDataBus, 1, 0);  // LCD data bus is output from ZYNQ perspective.
}

// Writes 8 bits to the TFT controller.
void LCD_write8(uint8_t value){
  LCD_assertWr();               // Assert the WR line.
  LCD_writeData(value);         // Copy the data out to the MIO pins.
  LCD_negateWr();               // Negate WR.
}

// Reads 8 bits from the TFT controller.
uint8_t LCD_read8(){
  LCD_assertRd();                 // Assert the RD line.
  LCD_delay10Nanoseconds(4);      // Read access time.
  uint8_t value = LCD_readData(); // Copy the data from the pins.
  return value;                   // Return the data to the caller.
}

// Write cycle time requires at least a minimum of 66 ns for a write-strobe.
void LCD_strobeWriteLine(){
  LCD_negateWr();             // Make sure it is negated.
  LCD_assertWr();             // Assert it.
//  LCD_delay10Nanoseconds(4);  // Wait for 40 ns.
  LCD_negateWr();             // negate it.
//  LCD_delay10Nanoseconds(4);  // Wait for 40 ns.
}

// Copies the argument value to the MIO pins serving as data pins for the LCD.
void LCD_writeData(uint8_t value) {
	XGpio_DiscreteWrite(&gpioTftDataBus, 1, value);  // Perform the write using Xilinx GPIO call.
}

// Copies the value from the MIO pins serving as the data pins for the LCD.
uint8_t LCD_readData() {
  uint8_t value = 0;
  value = XGpio_DiscreteRead(&gpioTftDataBus, 1);
  return value;
}


