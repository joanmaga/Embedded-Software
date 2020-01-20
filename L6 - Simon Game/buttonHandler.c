#include "buttonHandler.h"
#include "SimonDisplay.h"
#include "supportFiles/display.h"
#include <stdio.h>
#include "supportFiles/utils.h"

#define ADC_COUNTER_MAX_VALUE 0 //Max value to wait for ADC to settle is 1
#define TRUE_FLAG true
#define FALSE_FLAG false

#define RUN_TEST_TERMINATION_MESSAGE1 "buttonHandler_runTest()"  // Info message.
#define RUN_TEST_TERMINATION_MESSAGE2 "terminated."              // Info message.
#define RUN_TEST_TEXT_SIZE 2            // Make text easy to see.
#define RUN_TEST_TICK_PERIOD_IN_MS 100  // Assume a 100 ms tick period.
#define TEXT_MESSAGE_ORIGIN_X 0                  // Text is written starting at the right, and
#define TEXT_MESSAGE_ORIGIN_Y (DISPLAY_HEIGHT/2) // middle.



enum buttonHandler_st {
	buttonHandler_init_st,		// Standard init state.
	buttonHandler_waitForTouch_st,	// Spin here until the enable flag becomes true.
	buttonHandler_press_st,		//This handles when button was pressed
	buttonHandler_adc_counter,  // waiting for the touch-controller ADC to settle.
	buttonHandler_completed_st		// You are here if you have flashed all of the LEDs.
} buttonHandler_currentState = buttonHandler_init_st;

static int16_t x, y; //Parameters of the Touch Screen
static uint8_t z; //Pressure of Touch Screen

// Enables this state machine to leave the init state.
// static ensure that this variable is not visible outside of this file.
static bool enableFlag = false, complete = false, touched = false;
static uint32_t adcCounter;

static bool release; //Checks if the User released the Button or not

//Returns the Region Number based on where user Touched in the Screen
uint8_t buttonHandler_getRegionNumber()
{
	return simonDisplay_computeRegionNumber(x, y);
}

// Provide these accessors for the enable variable. No direct access allowed. Only allow access via functions.
void buttonHandler_enable() { enableFlag = true; }
void buttonHandler_disable() { enableFlag = false; }

bool buttonHandler_releaseDetected()    //Detect if, after user touches the Button, he releases it
{
	return release; //Return value of the Release Flag
}

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void debugStatePrint() {
  static buttonHandler_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state name over and over.
  if (previousState != buttonHandler_currentState || firstPass) {
    firstPass = false;                // previousState will be defined, firstPass is false.
    previousState = buttonHandler_currentState;     // keep track of the last state that you were in.
    switch(buttonHandler_currentState) {            // This prints messages based upon the state that you were in.
      case buttonHandler_init_st: //Enter Case init_st if that is the Current State
        printf("init_st\n\r");  //Print Current State
        break;
      case buttonHandler_waitForTouch_st:     //Enter Case waiting_for_touch_st if that is the Current State
        printf("waiting_for_touch_st\n\r");  //Print Current State
        break;
      case buttonHandler_adc_counter:     //Enter Case adc_Counter_running_st if that is the Current State
        printf("adcCounter: %ld\n\r", adcCounter);    //Check Value of Counter adcCounter for debugging purposes
        break;
      case buttonHandler_press_st:     //Enter Case Press if that is the Current State
        printf("Pressed_st\n\r");  //Print Current State
        break;
      case buttonHandler_completed_st:     //Enter Case Completed if that is the Current State
        printf("Completed\n\r");  //Print Current State
        break;
     }
  }
}

//Initializes the Button Handler State Machine
void buttonHandler_init()
{
	release = true;
	adcCounter = 0;
	buttonHandler_currentState = buttonHandler_init_st;
}

void buttonHandler_tick()
{
    debugStatePrint();  //Calls the Debug State Print Function for debugging purposes
    // Performs the State Update and Actions
	switch (buttonHandler_currentState) {
		case buttonHandler_init_st: //Enter Case init_st if that is the Current State
		    release = false;    //Reinitializes the Release Variable
		    display_clearOldTouchData();
		    if(enableFlag){ //Checks if State Machine is Enabled. If so, then enter the if Statement
		    buttonHandler_currentState = buttonHandler_adc_counter; //Go to Waiting for Touch state
		    }
			break;
		case buttonHandler_waitForTouch_st: //Enter Case waiting_for_touch_st if that is the Current State
		    if(!enableFlag){    //Base case to go back to Initial  State if State Machine was disabled
                buttonHandler_currentState = buttonHandler_init_st;
		    }
		    if(display_isTouched() && enableFlag){  //Checks if User has touched the Display
		        buttonHandler_currentState = buttonHandler_adc_counter; //User touched the Display. Go to State to handle such Action
		        display_clearOldTouchData();    //Clear Data from Touches from Old ticks were any data that is insignificant
		    }
			break;
		case buttonHandler_adc_counter:  //Enter Case adc_Counter_st if that is the Current State
		    if(!enableFlag)     //Base case to go back to Initial  State if State Machine was disabled
                buttonHandler_currentState = buttonHandler_init_st;
		    else if(display_isTouched()) {   //State Machine is still enabled. Get Touch Data
		        display_clearOldTouchData();
		        display_getTouchedPoint(&x, &y, &z);    //Get where user touched and save coordinates in the Variables in the Parameters
		        simonDisplay_drawSquare(buttonHandler_getRegionNumber(), FALSE_FLAG);   //Draw the Square in the Touched Region
		        buttonHandler_currentState = buttonHandler_press_st;  //Since button was pressed, go to new state that handles actions after User has touched the Display

		    }
            break;
		case buttonHandler_press_st:    //Enter Case buttonHandler_press_st if that is the Current State
            if(enableFlag == FALSE_FLAG)        //Base case to go back to Initial  State if State Machine was disabled
                buttonHandler_currentState = buttonHandler_init_st;
            else if (!display_isTouched() && enableFlag){    //Checks if user has relased the Button
                buttonHandler_currentState = buttonHandler_completed_st;    //If Released, go to Complete State
                simonDisplay_drawSquare(buttonHandler_getRegionNumber(), TRUE_FLAG);  // Erase the square.
                simonDisplay_drawButton(buttonHandler_getRegionNumber(), FALSE_FLAG);  //  Draw the button.
                release = true; //Set Release Flag to True
            }
            else    //No nonNeutral actions were taken. Stay in this State
                buttonHandler_currentState = buttonHandler_press_st;    //Stays in Pressed State
            break;
		case buttonHandler_completed_st:     //Enter Case buttonHandler_completed_st if that is the Current State
		    if(enableFlag == FALSE_FLAG)        //Base case to go back to Initial  State if State Machine was disabled
                buttonHandler_currentState = buttonHandler_init_st; //Goes to Initial State
		    break;
		default:
			break;
	}

}

// buttonHandler_runTest(int16_t touchCount) runs the test until
// the user has touched the screen touchCount times. It indicates
// that a button was pushed by drawing a large square while
// the button is pressed and then erasing the large square and
// redrawing the button when the user releases their touch.
void buttonHandler_runTest(int16_t touchCountArg)
{
    int16_t touchCount = 0;                 // Keep track of the number of touches.
    display_init();                         // Always have to init the display.
    display_fillScreen(DISPLAY_BLACK);      // Clear the display.
    // Draw all the buttons for the first time so the buttonHandler doesn't need to do this in an init state.
    // Ultimately, simonControl will do this when the game first starts up.
    simonDisplay_drawAllButtons();
    buttonHandler_init();                   // Initialize the button handler state machine
    buttonHandler_enable();
    while (touchCount < touchCountArg) {    // Loop here while touchCount is less than the touchCountArg
        buttonHandler_tick();               // Advance the state machine.
        utils_msDelay(RUN_TEST_TICK_PERIOD_IN_MS);
        if (buttonHandler_releaseDetected()) {  // If a release is detected, then the screen was touched.
            touchCount++;                       // Keep track of the number of touches.
            // Get the region number that was touched.
            printf("button released: %d\n\r", buttonHandler_getRegionNumber());
            // Interlocked behavior: handshake with the button handler (now disabled).
            buttonHandler_disable();
            utils_msDelay(RUN_TEST_TICK_PERIOD_IN_MS);
            buttonHandler_tick();               // Advance the state machine.
            buttonHandler_enable();             // Interlocked behavior: enable the buttonHandler.
            utils_msDelay(RUN_TEST_TICK_PERIOD_IN_MS);
            buttonHandler_tick();               // Advance the state machine.
        }
    }
    display_fillScreen(DISPLAY_BLACK);        // clear the screen.
    display_setTextSize(RUN_TEST_TEXT_SIZE);  // Set the text size.
    display_setCursor(TEXT_MESSAGE_ORIGIN_X, TEXT_MESSAGE_ORIGIN_Y); // Move the cursor to a rough center point.
    display_println(RUN_TEST_TERMINATION_MESSAGE1); // Print the termination message on two lines.
    display_println(RUN_TEST_TERMINATION_MESSAGE2);

}


