/*
 * display.h
 *
 *  Created on: Jul 18, 2014
 *      Author: hutch
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdlib.h>

#define DISPLAY_DEC 10
#define DISPLAY_HEX 16
#define DISPLAY_OCT 8
#define DISPLAY_BIN 2

#define DISPLAY_INIT_STATUS_OK 1
#define DISPLAY_INIT_STATUS_FAIL 0

#define DISPLAY_CHAR_WIDTH 6
#define DISPLAY_CHAR_HEIGHT 8

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define	DISPLAY_BLACK   0x0000
#define	DISPLAY_BLUE    0x001F
#define	DISPLAY_RED     0xF800
#define	DISPLAY_GREEN   0x07E0
#define DISPLAY_CYAN    0x07FF
#define DISPLAY_MAGENTA 0xF81F
#define DISPLAY_YELLOW  0xFFE0
#define DISPLAY_WHITE   0xFFFF

#define DISPLAY_LANDSCAPE_MODE_ORIGIN_UPPER_LEFT 1
#define DISPLAY_LANDSCAPE_MODE_ORIGIN_LOWER_RIGHT 3
#define DISPLAY_PORTRAIT_MODE_ORIGIN_LOWER_LEFT 0
#define DISPLAY_PORTRAIT_MODE_ORIGIN_UPPER_RIGHT 2

typedef uint16_t display_pixel_t; // Standard pixel type.

// This provides the primary high-level API to the LCD display, including the touch-panel. The interface
// will be C-like, with a functional interface that does not require the user to use constructors or objects.
// These functions are mostly just wrappers around C++ methods so they can be used for C programming.

// Constructs the necessary LCD and touch-controller objects and performs necessary initializations.
void display_init();

// The functionality for these functions comes from Adafruit_GFX.cpp and Adafruit_TFTLCD.cpp.
void
  display_drawPixel(int16_t x0, int16_t y0, uint16_t color),
  display_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
  display_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
  display_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
  display_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
  display_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
  display_fillScreen(uint16_t color),
  display_invertDisplay(bool i),
  display_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
  display_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
  display_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
  int16_t x2, int16_t y2, uint16_t color),
  display_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
  int16_t x2, int16_t y2, uint16_t color),
  display_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
  int16_t radius, uint16_t color),
  display_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
  int16_t radius, uint16_t color),
  display_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
  int16_t w, int16_t h, uint16_t color),
  display_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
  uint16_t bg, uint8_t size),
  display_setCursor(int16_t x, int16_t y),
  display_setTextColor(uint16_t c),
  display_setTextColor(uint16_t c, uint16_t bg),
  display_setTextSize(uint8_t s),
  display_setTextWrap(bool w),
  display_setRotation(uint8_t r);
  int16_t display_height();
  int16_t display_width();
  uint16_t display_color565(uint8_t r, uint8_t g, uint8_t b);  // Packs r,g,b into 16 bits.

  // Print routines
  size_t display_println(const char str[]);
  size_t display_println(char c);
  size_t display_println(unsigned char c, int base = DISPLAY_DEC);
  size_t display_println(int num, int base = DISPLAY_DEC);
  size_t display_println(unsigned int num, int base = DISPLAY_DEC);
  size_t display_println(long num, int base = DISPLAY_DEC);
  size_t display_println(unsigned long num, int base = DISPLAY_DEC);
  size_t display_println(double num, int fieldWidth = 2);
  size_t display_println(void);

  size_t display_print(const char str[]);
  size_t display_print(char c);
  size_t display_print(unsigned char c, int base = DISPLAY_DEC);
  size_t display_print(int num, int base = DISPLAY_DEC);
  size_t display_print(unsigned int num, int base = DISPLAY_DEC);
  size_t display_print(long num, int base = DISPLAY_DEC);
  size_t display_print(unsigned long num, int base = DISPLAY_DEC);
  size_t display_print(double num, int fieldWidth = 2);

  // Display test routines.
  unsigned long display_testLines(uint16_t color);
  unsigned long display_testFastLines(uint16_t color1, uint16_t color2);
  unsigned long display_testRects(uint16_t color);
  unsigned long display_testFilledRects(uint16_t color1, uint16_t color2);
  unsigned long display_testFilledCircles(uint8_t radius, uint16_t color);
  unsigned long display_testCircles(uint8_t radius, uint16_t color);
  unsigned long display_testTriangles();
  unsigned long display_testFilledTriangles();
  unsigned long display_testRoundRects();
  unsigned long display_testFilledRoundRects();
  unsigned long display_testFillScreen();
  unsigned long display_testText();
  // Calles all of the test routines.
  unsigned long display_test();

// The functionality for these routines comes from Adafruit_STMPE610 (touch controller).
// True if the display is being touched.
bool display_isTouched(void);
// Returns the x-y coordinate point and the pressure (z).
void display_getTouchedPoint(int16_t *x, int16_t *y, uint8_t *z);
// Throws away all previous touch data.
void display_clearOldTouchData();


#endif /* DISPLAY_H_ */
