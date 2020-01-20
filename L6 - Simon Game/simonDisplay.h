#ifndef SIMONDISPLAY_H_
#define SIMONDISPLAY_H_

#include <stdbool.h>
#include <stdint.h>

// Width, height of the simon "buttons"
#define SIMON_DISPLAY_BUTTON_WIDTH 60
#define SIMON_DISPLAY_BUTTON_HEIGHT 60

// WIdth, height of the simon "squares.
// Note that the video shows the squares as larger but you
// can use this smaller value to make the game easier to implement speed-wise.
#define SIMON_DISPLAY_SQUARE_WIDTH  120
#define SIMON_DISPLAY_SQUARE_HEIGHT 120

// Given coordinates from the touch pad, computes the region number.

// The entire touch-screen is divided into 4 rectangular regions, numbered 0 - 3.
// Each region will be drawn with a different color. Colored buttons remind
// the user which square is associated with each color. When you press
// a region, computeRegionNumber returns the region number that is used
// by the other routines.
/*
|----------|----------|
|          |          |
|    0     |     1    |
|  (RED)   | (YELLOW) |
-----------------------
|          |          |
|     2    |    3     |
|  (BLUE)  |  (GREEN) |
-----------------------
*/

// These are the definitions for the regions.
#define SIMON_DISPLAY_REGION_0 0
#define SIMON_DISPLAY_REGION_1 1
#define SIMON_DISPLAY_REGION_2 2
#define SIMON_DISPLAY_REGION_3 3

int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y);

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
// If erase argument is true, draws the button as black background to erase it.
void simonDisplay_drawButton(uint8_t regionNumber, bool erase);

// Convenience function that draws all of the buttons.
void simonDisplay_drawAllButtons();

// Convenience function that erases all of the buttons.
void simonDisplay_eraseAllButtons();

// Draws a bigger square that completely fills the region.
// If the erase argument is true, it draws the square as black background to "erase" it.
void simonDisplay_drawSquare(uint8_t regionNo, bool erase);

// Runs a brief demonstration of how buttons can be pressed and squares lit up to implement the user
// interface of the Simon game. The routine will continue to run until the touchCount has been reached, e.g.,
// the user has touched the pad touchCount times.

// I used a busy-wait delay (utils_msDelay) that uses a for-loop and just blocks until the time has passed.
// When you implement the game, you CANNOT use this function as we discussed in class. Implement the delay
// using the non-blocking state-machine approach discussed in class.
void simonDisplay_runTest(uint16_t touchCount);

#endif /* SIMONDISPLAY_H_ */
