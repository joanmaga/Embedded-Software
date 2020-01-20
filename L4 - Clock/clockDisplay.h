/*
 * clockDisplay.h
 *
 *  Created on: Sep 29, 2017
 *      Author: joanmaga
 */

#ifndef CLOCKDISPLAY_H_
#define CLOCKDISPLAY_H_

#include <stdbool.h>

// Called only once - performs any necessary inits.
// This is a good place to draw the triangles and any other
// parts of the clock display that will never change.
void clockDisplay_init();

// Updates the time display with latest time, making sure to update only those digits that
// have changed since the last update.
// if forceUpdateAll is true, update all digits.
void clockDisplay_updateTimeDisplay(bool forceUpdateAll);

// Reads the touched coordinates and performs the increment or decrement,
// depending upon the touched region.
void clockDisplay_performIncDec();

// Advances the time forward by 1 second and update the display.
void clockDisplay_advanceTimeOneSecond();

// Run a test of clock-display functions.
void clockDisplay_runTest();


#endif /* CLOCKDISPLAY_H_ */
