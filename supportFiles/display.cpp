/*
 * display.cpp
 *
 *  Created on: Jul 18, 2014
 *      Author: hutch
 */

#include "display.h"
#include "Adafruit_TFTLCD.h"
#include "Adafruit_STMPE610.h"
#include <stdbool.h>

// Just define these values here. They won't change in practice and I want to avoid
// too much tangling between the LCD control code and the touch-controller code.
// Otherwise, it would make sense to always get these values from the LCD controller.
// These #defines are not shared outside of this .cpp file and so don't need to follow the
// naming convention.
#define LCD_WIDTH  320.0
#define LCD_HEIGHT 240.0
#define LCD_LEFT_OFFSCREEN_TOUCH_WIDTH 20


// The touch-controller uses a 12-bit ADC.
#define TOUCH_SCREEN_MAX_X 3950.0  // This is where the touch-screen maxes out in X.
#define TOUCH_SCREEN_MAX_Y 4095.0

static bool initFlag = false;  // Only allow init to be called once.
static Adafruit_TFTLCD lcdDisplay = Adafruit_TFTLCD();  // Handle to the LCD display.
static Adafruit_STMPE610 touchController = Adafruit_STMPE610();

// Will only execute the body once.
void display_init() {
  if (!initFlag) {
    lcdDisplay.begin();
    lcdDisplay.setRotation(1);
    touchController.begin();
  }
}

// These are functions related to display. Functionality comes from Adafruit_GFX.
void display_drawPixel(int16_t x0, int16_t y0, uint16_t color) {
  lcdDisplay.drawPixel(x0, y0, color);
}

void display_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  lcdDisplay.drawLine(x0, y0, x1, y1, color);
}

void display_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  lcdDisplay.drawFastVLine(x, y, h, color);
}

void display_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  lcdDisplay.drawFastHLine(x, y, w, color);
}

void display_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  lcdDisplay.drawRect(x, y, w, h, color);
}

void display_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  lcdDisplay.fillRect(x, y, w, h, color);
}

void display_fillScreen(uint16_t color) {
  lcdDisplay.fillScreen(color);
}

void display_invertDisplay(bool i) {
  lcdDisplay.invertDisplay(i);
}

void display_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  lcdDisplay.drawCircle(x0, y0, r, color);
}

void display_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  lcdDisplay.fillCircle(x0, y0, r, color);
}

void display_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
int16_t x2, int16_t y2, uint16_t color) {
  lcdDisplay.drawTriangle(x0, y0, x1, y1, x2, y2, color);
}

void display_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
int16_t x2, int16_t y2, uint16_t color) {
  lcdDisplay.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void display_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
int16_t radius, uint16_t color) {
  lcdDisplay.drawRoundRect(x0, y0, w, h, radius, color);
}

void display_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
int16_t radius, uint16_t color) {
  lcdDisplay.fillRoundRect(x0, y0, w, h, radius, color);
}

void display_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
int16_t w, int16_t h, uint16_t color) {
  lcdDisplay.drawBitmap(x, y, bitmap, w, h, color);
}

void display_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
uint16_t bg, uint8_t size) {
  lcdDisplay.drawChar(x, y, c, color, bg, size);
}

void display_setCursor(int16_t x, int16_t y) {
  lcdDisplay.setCursor(x, y);
}

void display_setTextColor(uint16_t c) {
  lcdDisplay.setTextColor(c);
}

void display_setTextColor(uint16_t c, uint16_t bg) {
  lcdDisplay.setTextColor(c, bg);
}

void display_setTextSize(uint8_t s) {
  lcdDisplay.setTextSize(s);
}

void display_setTextWrap(bool w) {
  lcdDisplay.setTextWrap(w);
}

void display_setRotation(uint8_t r) {
  lcdDisplay.setRotation(r);
}

int16_t display_height() {
  return lcdDisplay.height();
}

int16_t display_width() {
  return lcdDisplay.width();
}

// Obscure function name = just packs the RGB data into a 16-bit int.
uint16_t display_color565(uint8_t r, uint8_t g, uint8_t b) {
  return lcdDisplay.color565(r, g, b);
}

size_t display_println(const char str[]) {
  return lcdDisplay.println(str);
}

size_t display_println(char c) {
  return lcdDisplay.println(c);
}

size_t display_println(unsigned char c, int base) {
  return lcdDisplay.println(c, base);
}

size_t display_println(int num, int base) {
  return lcdDisplay.println(num, base);
}

size_t display_println(unsigned int num, int base) {
  return lcdDisplay.println(num, base);
}

size_t display_println(long num, int base) {
  return lcdDisplay.println(num, base);
}

size_t display_println(unsigned long num, int base) {
  return lcdDisplay.println(num, base);
}

size_t display_println(double num, int fieldWidth) {
  return lcdDisplay.println(num, fieldWidth);
}

size_t display_println(void) {
  return lcdDisplay.println();
}

size_t display_print(const char str[]) {
	return lcdDisplay.print(str);
}

size_t display_print(char c) {
	return lcdDisplay.print(c);
}

size_t display_print(unsigned char c, int base) {
	return lcdDisplay.print(c, base);
}

size_t display_print(int num, int base) {
	return lcdDisplay.print(num, base);
}

size_t display_print(unsigned int num, int base) {
	return lcdDisplay.print(num, base);
}

size_t display_print(long num, int base) {
	return lcdDisplay.print(num, base);
}

size_t display_print(unsigned long num, int base) {
	return lcdDisplay.print(num, base);
}

size_t display_print(double num, int fieldWidth) {
	return lcdDisplay.print(num, fieldWidth);
}


// These are functions related to the touch-pad.

// True if the display is being touched.
bool display_isTouched(void) {
  return touchController.touched();
}

// These min and max values correspond to the edges of the LCD panel.
// x runs from the min value at the bottom, max value at the top.
// y runs from the min value at the left, max value at the right.
#define MIN_Y_TOUCH_POINT 280.0
#define MAX_Y_TOUCH_POINT 3900.0
#define MIN_X_TOUCH_POINT 350.0
#define MAX_X_TOUCH_POINT 3950.0
// Maps the touch-screen coordinates back to the LCD coordinate space.
void display_mapToLcdCoordinates(int16_t *x, int16_t *y) {
    // Swap incoming x and y to match with the LCD panel.
	float lcdX = *y;
	float lcdY = *x;
    // Flip y so min is at top, max is at bottom.
	lcdY = (MAX_Y_TOUCH_POINT) - lcdY;
	// Scale y.
	lcdY = (lcdY / (MAX_Y_TOUCH_POINT - MIN_Y_TOUCH_POINT)) * LCD_HEIGHT;
	// Subtract off the minimum - you want to go negative on the left of the LCD
	// and offset the maximum value so it matches the max LCD coordinate at the far right
	// of the LCD.
	lcdX -= MIN_X_TOUCH_POINT;
    // x is within the LCD, scale by the full width of the LCD.
	lcdX = (lcdX /(MAX_X_TOUCH_POINT - MIN_X_TOUCH_POINT)) * LCD_WIDTH;
	// Make the final assignment back to the arguments.
    *x = lcdX;
    *y = lcdY;
}

// Returns the x-y coordinate of the touched point and the pressure (z).
void display_getTouchedPoint(int16_t *x, int16_t *y, uint8_t *z) {
  touchController.readData(x, y, z);
  display_mapToLcdCoordinates(x, y);
}

// Throws away all previous touch data.
void display_clearOldTouchData() {
  touchController.clearOldTouchData();
}

// Display test routines, just adapted from the original Adafruit code.

// quick hack for min - to be used for these test functions only.
#define min(x, y) ((x) < (y) ? (x) : (y))

unsigned long display_testLines(uint16_t color) {
  int           x1, y1, x2, y2,
                w = display_width(),
                h = display_height();

  display_fillScreen(DISPLAY_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);

  display_fillScreen(DISPLAY_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);

  display_fillScreen(DISPLAY_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);

  display_fillScreen(DISPLAY_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  for(x2=0; x2<w; x2+=6) display_drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) display_drawLine(x1, y1, x2, y2, color);
  return 0;
}

unsigned long display_testFastLines(uint16_t color1, uint16_t color2) {
  int           x, y, w = display_width(), h = display_height();

  display_fillScreen(DISPLAY_BLACK);
  for(y=0; y<h; y+=5) display_drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) display_drawFastVLine(x, 0, h, color2);
  return 0;
}

unsigned long display_testRects(uint16_t color) {
  int           n, i, i2,
                cx = display_width()  / 2,
                cy = display_height() / 2;

  display_fillScreen(DISPLAY_BLACK);
  n     = min(display_width(), display_height());
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    display_drawRect(cx-i2, cy-i2, i, i, color);
  }
  return 0;
}

unsigned long display_testFilledRects(uint16_t color1, uint16_t color2) {
  int           n, i, i2,
                cx = display_width()  / 2 - 1,
                cy = display_height() / 2 - 1;

  display_fillScreen(DISPLAY_BLACK);
  n = min(display_width(), display_height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    display_fillRect(cx-i2, cy-i2, i, i, color1);
    // Outlines are not included in timing results
    display_drawRect(cx-i2, cy-i2, i, i, color2);
  }
  return 0;
}

unsigned long display_testFilledCircles(uint8_t radius, uint16_t color) {
  int x, y, w = display_width(), h = display_height(), r2 = radius * 2;

  display_fillScreen(DISPLAY_BLACK);
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      display_fillCircle(x, y, radius, color);
    }
  }
  return 0;
}

unsigned long display_testCircles(uint8_t radius, uint16_t color) {
  int           x, y, r2 = radius * 2,
                w = display_width()  + radius,
                h = display_height() + radius;

  // Screen is not cleaDISPLAY_RED for this one -- this is
  // intentional and does not affect the reported time.
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      display_drawCircle(x, y, radius, color);
    }
  }
  return 0;
}

unsigned long display_testTriangles() {
  int           n, i, cx = display_width()  / 2 - 1,
                      cy = display_height() / 2 - 1;

  display_fillScreen(DISPLAY_BLACK);
  n     = min(cx, cy);
  for(i=0; i<n; i+=5) {
    display_drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      display_color565(0, 0, i));
  }
  return 0;
}

unsigned long display_testFilledTriangles() {
  int           i, cx = display_width()  / 2 - 1,
                   cy = display_height() / 2 - 1;

  display_fillScreen(DISPLAY_BLACK);
  for(i=min(cx,cy); i>10; i-=5) {
    display_fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display_color565(0, i, i));
    display_drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display_color565(i, i, 0));
  }
  return 0;
}

unsigned long display_testRoundRects() {
  int           w, i, i2,
                cx = display_width()  / 2 - 1,
                cy = display_height() / 2 - 1;

  display_fillScreen(DISPLAY_BLACK);
  w     = min(display_width(), display_height());
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    display_drawRoundRect(cx-i2, cy-i2, i, i, i/8, display_color565(i, 0, 0));
  }
  return 0;
}

unsigned long display_testFilledRoundRects() {
  int           i, i2,
                cx = display_width()  / 2 - 1,
                cy = display_height() / 2 - 1;

  display_fillScreen(DISPLAY_BLACK);
  for(i=min(display_width(), display_height()); i>20; i-=6) {
    i2 = i / 2;
    display_fillRoundRect(cx-i2, cy-i2, i, i, i/8, display_color565(0, i, 0));
  }
  return 0;
}

unsigned long display_testFillScreen() {
  display_fillScreen(DISPLAY_BLACK);
  display_fillScreen(DISPLAY_RED);
  display_fillScreen(DISPLAY_GREEN);
  display_fillScreen(DISPLAY_BLUE);
  display_fillScreen(DISPLAY_BLACK);
  return 0;
}

unsigned long display_testText() {
  display_fillScreen(DISPLAY_BLACK);
  display_setCursor(0, 0);
  display_setTextColor(DISPLAY_WHITE);  display_setTextSize(1);
  display_println("Hello World!");
  display_setTextColor(DISPLAY_YELLOW); display_setTextSize(2);
  display_println(1234.56);
  display_setTextColor(DISPLAY_RED);    display_setTextSize(3);
  display_println(0xDEADBEEF, HEX);
  display_println();
  display_setTextColor(DISPLAY_GREEN);
  display_setTextSize(5);
  display_println("Groop");
  display_setTextSize(2);
  display_println("I implore thee,");
  display_setTextSize(1);
  display_println("my foonting turlingdromes.");
  display_println("And hooptiously drangle me");
  display_println("with crinkly bindlewurdles,");
  display_println("Or I will rend thee");
  display_println("in the gobberwarts");
  display_println("with my blurglecruncheon,");
  display_println("see if I don't!");
  return 0;
}

// Test everything.
unsigned long display_test() {
	display_testLines(DISPLAY_WHITE);
	display_testFastLines(DISPLAY_GREEN, DISPLAY_BLUE);
	display_testRects(DISPLAY_YELLOW);
	display_testFilledRects(DISPLAY_BLUE, DISPLAY_GREEN);
	display_testFilledCircles(25, DISPLAY_RED);
	display_testCircles(35, DISPLAY_WHITE);
	display_testTriangles();
	display_testFilledTriangles();
	display_testRoundRects();
	display_testFilledRoundRects();
	display_testFillScreen();
	display_testText();
	return 0;
}






