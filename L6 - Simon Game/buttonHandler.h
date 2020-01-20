#ifndef BUTTONHANDLER_H_
#define BUTTONHANDLER_H_
#include <stdint.h>
// Get the simon region numbers. See the source code for the region numbering scheme.
uint8_t buttonHandler_getRegionNumber();

// Turn on the state machine. Part of the interlock.
void buttonHandler_enable();

// Turn off the state machine. Part of the interlock.
void buttonHandler_disable();

// The only thing this function does is return a boolean flag set by the buttonHandler state machine. To wit:
// Once enabled, the buttonHandler state-machine first waits for a touch. Once a touch is detected, the
// buttonHandler state-machine computes the region-number for the touched area. Next, the buttonHandler
// state-machine waits until the player removes their finger. At this point, the state-machine should
// set a bool flag that indicates the the player has removed their finger. Once the buttonHandler()
// state-machine is disabled, it should clear this flag.
// All buttonHandler_releasedDetected() does is return the value of this flag.
// As such, the body of this function should only contain a single line of code.
bool buttonHandler_releaseDetected();

// Initialize the state machine
void buttonHandler_init();

// Standard tick function.
void buttonHandler_tick();

// This tests the functionality of the buttonHandler state machine.
// buttonHandler_runTest(int16_t touchCount) runs the test until
// the user has touched the screen touchCount times. It indicates
// that a button was pushed by drawing a large square while
// the button is pressed and then erasing the large square and
// redrawing the button when the user releases their touch.
void buttonHandler_runTest(int16_t touchCount);

#endif /* BUTTONHANDLER_H_ */
