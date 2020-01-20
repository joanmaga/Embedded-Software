/*
 * lcd.h
 *
 *  Created on: Jul 1, 2014
 *      Author: hutch
 */

#ifndef LCD_H_
#define LCD_H_
#include "arduinoTypes.h"
#include "xgpio.h"
#include "TFTGPIO.h"
#include <stdio.h>

// Provides an API to read/write the LCD controller.

// Processor clock is 650 MHz. Clock period = 1.54 ns.
#define LCD_10_NANOSECOND_DELAY_COUNT    1       // Should be approximately 10 ns.
#define LCD_MICROSECOND_DELAY_COUNT    649       // Should be approximately 1 microsecond.
#define LCD_MILLISECOND_DELAY_COUNT 649000       // Should be approximately 1 millisecond.

#define LCD_CONTROL_DEVICE_ID XPAR_AXI_GPIO_TFT_CONTROL_DEVICE_ID
#define LCD_DATA_BUS_DEVICE_ID XPAR_AXI_GPIO_TFT_DATA_BUS_DEVICE_ID
#define LCD_BIT_WIDTH 2
#define LCD_DCX_BIT_MASK 0x00000001
#define LCD_RD_BIT_MASK  0x00000002
#define LCD_WR_BIT_MASK  0x00000004

// These calls are related to the GPIO control block that is dedicated to the LCD panel.
void LCD_init();
void LCD_setCommandMode();
void LCD_setDataMode();
void LCD_assertRd();
void LCD_negateRd();
void LCD_assertWr();
void LCD_negateWr();

// Utility functions for the TFT LCD Controller.
void LCD_delay(uint16_t delay);              // millisecond delay
void LCD_delayMicroseconds(uint16_t delay);  // microsecond delay
void LCD_delay10Nanoseconds(uint16_t delay);  // delay in 15 ns chunks

// These calls are related to the data bus pins (GPIO) that are connected to the LCD controller.
void LCD_write8(uint8_t value);              // Writes 8 bits to the TFT controller.
uint8_t LCD_read8();                         // Reads 8 bits from the TFT controller.
void LCD_setCommandMode();
void LCD_setDataMode();
void LCD_strobeWriteLine();
void LCD_assertWriteLine();
void LCD_negateWriteLine();
void LCD_assertReadLine();
void LCD_negateReadLine();
void LCD_writeData(uint8_t data);
uint8_t LCD_readData(void);
void LCD_setReadDataDirection();
void LCD_setWriteDataDirection();

#endif /* LCD_H_ */
