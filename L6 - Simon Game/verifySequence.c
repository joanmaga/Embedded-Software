/*
 * verifySequence.c
 *
 *  Created on: Nov 13, 2017
 *      Author: joanmaga
 */

#include "verifySequence.h"
#include "globals.h"
#include "buttonHandler.h"
#include "supportFiles/utils.h"
#include "buttonHandler.h"
#include "SimonDisplay.h"
#include "supportFiles/display.h"
#include <stdio.h>
#include "src/Lab2/buttons.h"

#define USER_WAIT_TOUCH 10   //Max Wait for User to make the Touches following the first Touches

enum verifySequence_st {
    verifySequence_init_st,     // Standard init state.
    verifySequence_wait_for_touch_st,
    verifySequence_adc_st,
    verifySequence_button_control_st,   //Disables and Enables the Button Handler State Machine. It restarts the State machine
    verifySequence_completed_st
} verifySequence_currentState = verifySequence_init_st;

// Enables this state machine to leave the init state.
// static ensure that this variable is not visible outside of this file.
static bool enableFlag = false;
static bool complete = false; //Checks if sequence is completed
static bool timedOut = false;
static bool inError = false;
static bool btnEnabled = false;

static uint16_t index;
static uint8_t wait_touch;

// State machine will run when enabled.
void verifySequence_enable() { enableFlag = true; }

// This is part of the interlock. You disable the state-machine and then enable it again.
void verifySequence_disable() { enableFlag = false; }

// Used to detect if there has been a time-out error.
bool verifySequence_isTimeOutError() { return timedOut; }

// Used to detect if the user tapped the incorrect sequence.
bool verifySequence_isUserInputError() { return inError;  }

// Used to detect if the verifySequence state machine has finished verifying.
bool verifySequence_isComplete() { return complete; }

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void verifyDebugStatePrint() {
  static verifySequence_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state name over and over.
  if (previousState != verifySequence_currentState || firstPass) {
    firstPass = false;                // previousState will be defined, firstPass is false.
    previousState = verifySequence_currentState;     // keep track of the last state that you were in.
    switch(verifySequence_currentState) {            // This prints messages based upon the state that you were in.
      case verifySequence_init_st: //Enter Case init_st if that is the Current State
        printf("verifySequence_init_st\n\r");  //Print Current State
        break;
      case verifySequence_wait_for_touch_st:     //Enter Case wait_for_touch if that is the Current State
        printf("verifySequence_wait_for_touch_st\n\r");  //Print Current State
        break;
      case verifySequence_button_control_st:    //Enter Case button_control if that is the Current State
        printf("verifySequence_button_control_st\n\r"); //Print Current State
        break;
      case verifySequence_adc_st:     //Enter Case adc_Counter_running_st if that is the Current State
        printf("adcCounter\n\r");    //Check Value of Counter adcCounter for debugging purposes
        break;
      case verifySequence_completed_st:     //Enter Case Completed if that is the Current State
        printf("Completed Verification\n\r");  //Print Current State
        break;
     }
  }
}

// Initialize the state machine
void verifySequence_init()
{
    index = 0, wait_touch = 0;  //Set Counters to Zero
    complete = false, timedOut = false, inError = false;    //Initialize Flags to False
    verifySequence_currentState = verifySequence_init_st;   //Set Current State to Initial State
}

// Standard tick function.
void verifySequence_tick()
{
    verifyDebugStatePrint();  //Calls the Debug State Print Function for debugging purposes
    // Performs the State Update and Actions
    switch (verifySequence_currentState) {
    case verifySequence_init_st: //Enter Case init_st if that is the Current State
        if (enableFlag) { //Checks if State Machine is Enabled. If so, then enter the if Statement
            verifySequence_currentState = verifySequence_wait_for_touch_st; //Go to Waiting for Touch state
        }
        timedOut = false;   //Set timedOut flag to false
        inError = false;   //Set inError flag to false
        complete = false;   //Set complete flag to false
        wait_touch = 0; //Set wait_touch Counter to 0
        index = 0; //Set index Counter to 0
        break;
    case verifySequence_wait_for_touch_st:     //Enter Case wait_for_touch if that is the Current State
        if (!enableFlag) {    //Base case to go back to Initial  State if State Machine was disabled
            verifySequence_currentState = verifySequence_init_st; //Goes to Initial State
        }
        if (display_isTouched() && enableFlag) {    //Checks if Display was touched while the State Machine is Enabled
            buttonHandler_enable(); //Enable the Button Handler State Machine
            verifySequence_currentState = verifySequence_adc_st;    //Goes to ADC State
        }
        else if (wait_touch == USER_WAIT_TOUCH) //Checks if wait_touch Counter reached the Maximum Value
        {
            timedOut = true;    //Set Timed out flag to True
            buttonHandler_disable();    //Disable Button Handler State Machine
            verifySequence_currentState = verifySequence_completed_st;  //Go to Complete State
        }
        wait_touch++;   //Increment wait_touch Counter while State Machine ticks in this State
        break;
    case verifySequence_adc_st:     //Enter Case adc_Counter_running_st if that is the Current State
        timedOut = false;   //Set timedOut Flag to False
        if (!enableFlag) {    //Base case to go back to Initial  State if State Machine was disabled
            verifySequence_currentState = verifySequence_init_st; //Goes to Initial State
        }
        else {  //If State Machine is still enabled, enter this Statement
            if (buttonHandler_releaseDetected()) {  //Checks if is has let a Button go after a Touch
                if (globals_getSequenceValue(index) != buttonHandler_getRegionNumber()) {   //Checks if User touched the wrong Button that was required for this Sequence
                    inError = true; //User made an input Error. Set this flag to True
                    printf("Input Error\n\r");  //Debugging Print Statement to make sure this If Statement is being accessed, and that the User pressed the wrong button
                    verifySequence_currentState = verifySequence_completed_st;  //No other Actions are needed. Go to Completed State
                }
                else{   //If user did not press the wrong button, enter this Statement
                printf("Index: %d\n\r", index); //Debugging Print Statement to check the Value of Variable Index
                printf("Region Number: %d\n\r", buttonHandler_getRegionNumber()); //Debugging Print Statement to check the Value of the Region Number being pressed
                printf("Sequence Value: %d\n\r", globals_getSequenceValue(index)); //Debugging Print Statement to check the Value of the Current required Sequence button that needs to be pressed

                verifySequence_currentState = verifySequence_button_control_st; //Go to Button Control State

                index++;    //User pressed the right button, increment Index to check next Move
                wait_touch = 0; //Reset wait touch Counter
                }
            }
            else {   //User hasn't released the Button. Enter this State
                wait_touch++;   //Increment the wait touch Counter
                printf("Wait_touch: %d\n\r", wait_touch); //Debugging Print Statement to check value of Wait Touch Counter
            }
        }
        if (wait_touch == USER_WAIT_TOUCH)  //Checks if Wait Touch Counter has reached its Maximum Value
        {
            timedOut = true;    //Set Timed out flag to True
            buttonHandler_disable();    //Disable Button Handler State Machine
            verifySequence_currentState = verifySequence_completed_st;  //Go to Complete State
        }
        if (index == globals_getSequenceIterationLength()) {    //Check if Index has reached its Maximum Value
            verifySequence_currentState = verifySequence_completed_st;  //Go to Complete State
        }
        break;
    case verifySequence_button_control_st:    //Enter Case button_control if that is the Current State
        if (!enableFlag) {    //Base case to go back to Initial  State if State Machine was disabled
            verifySequence_currentState = verifySequence_init_st; //Goes to Initial State
        }
        else{
            if  (btnEnabled){   //Checks if Button Enable flag is true
                verifySequence_currentState = verifySequence_wait_for_touch_st; //Go to Waiting for Touch state
                btnEnabled = false; //Reset the Button Enabled Flag
            }
            else if (buttonHandler_releaseDetected()){  //Checks if User has released the Button
                buttonHandler_disable();    //Disable the Button Handler State Machine
                btnEnabled = true;  //Set the Button Enabled Flag to true
            }
        }
        break;
    case verifySequence_completed_st:     //Enter Case Completed if that is the Current State
        if (!enableFlag) {    //Base case to go back to Initial  State if State Machine was disabled
            verifySequence_currentState = verifySequence_init_st;    //Goes to Initial State
        }
        index = 0;  //Reset the index
        complete = true;    //Set Complete Flag to True
        buttonHandler_disable();    //Disable Button Handler State Machine
        break;
    }
}

#define MESSAGE_X 0
//#define MESSAGE_Y (display_width()/4)
#define MESSAGE_Y (display_height()/2)
#define MESSAGE_TEXT_SIZE 2
//#define MESSAGE_STARTING_OVER
#define BUTTON_0 0  // Index for button 0
#define BUTTON_1 1  // Index for button 1
#define BUTTON_2 2  // Index for button 2
#define BUTTON_3 3  // Index for button 3
// Prints the instructions that the user should follow when
// testing the verifySequence state machine.
// Takes an argument that specifies the length of the sequence so that
// the instructions are tailored for the length of the sequence.
// This assumes a simple incrementing pattern so that it is simple to
// instruct the user.
void verifySequence_printInstructions(uint8_t length, bool startingOver) {
    display_fillScreen(DISPLAY_BLACK);              // Clear the screen.
    display_setTextSize(MESSAGE_TEXT_SIZE);     // Make it readable.
    display_setCursor(MESSAGE_X, MESSAGE_Y);    // Rough center.
    if (startingOver) {                                             // Print a message if you start over.
        display_fillScreen(DISPLAY_BLACK);          // Clear the screen if starting over.
        display_setTextColor(DISPLAY_WHITE);        // Print whit text.
        display_println("Starting Over. ");         // Starting over message.
    }
    // Print messages are self-explanatory, no comments needed.
    // These messages request that the user touch the buttons in a specific sequence.
    display_println("Tap: ");
    display_println();
    switch (length) {
    case 1:
        display_println("red");
        break;
    case 2:
        display_println("red, yellow ");
        break;
    case 3:
        display_println("red, yellow, blue ");
        break;
    case 4:
        display_println("red, yellow, blue, green ");
        break;
    default:
        break;
    }
    display_println("in that order.");
    display_println();
    display_println("hold BTN0 to quit.");
}

// Just clears the screen and draws the four buttons used in Simon.
void verifySequence_drawButtons() {
    display_fillScreen(DISPLAY_BLACK);  // Clear the screen.
    simonDisplay_drawAllButtons();      // Draw all the buttons.
}

// This will set the sequence to a simple sequential pattern.
#define MAX_TEST_SEQUENCE_LENGTH 4  // the maximum length of the pattern
uint8_t verifySequence_testSequence[MAX_TEST_SEQUENCE_LENGTH] = {0, 1, 2, 3};  // A simple pattern.
#define MESSAGE_WAIT_MS 4000  // Display messages for this long.

// Increment the sequence length making sure to skip over 0.
// Used to change the sequence length during the test.
int16_t incrementSequenceLength(int16_t sequenceLength) {
    int16_t value = (sequenceLength + 1) % (MAX_TEST_SEQUENCE_LENGTH+1);
    if (value == 0) value++;
    return value;
}

// Used to select from a variety of informational messages.
enum verifySequence_infoMessage_t {
    user_time_out_e,            // means that the user waited too long to tap a color.
    user_wrong_sequence_e,      // means that the user tapped the wrong color.
    user_correct_sequence_e,    // means that the user tapped the correct sequence.
    user_quit_e                 // means that the user wants to quite.
};

// Prints out informational messages based upon a message type (see above).
void verifySequence_printInfoMessage(verifySequence_infoMessage_t messageType) {
    // Setup text color, position and clear the screen.
    display_setTextColor(DISPLAY_WHITE);
    display_setCursor(MESSAGE_X, MESSAGE_Y);
    display_fillScreen(DISPLAY_BLACK);
  switch(messageType) {
  case user_time_out_e:  // Tell the user that they typed too slowly.
        display_println("Error:");
        display_println();
        display_println("  User tapped sequence");
        display_println("  too slowly.");
    break;
  case user_wrong_sequence_e:  // Tell the user that they tapped the wrong color.
        display_println("Error: ");
        display_println();
        display_println("  User tapped the");
        display_println("  wrong sequence.");
    break;
  case user_correct_sequence_e:  // Tell the user that they were correct.
        display_println("User tapped");
        display_println("the correct sequence.");
    break;
  case user_quit_e:             // Acknowledge that you are quitting the test.
    display_println("quitting runTest().");
    break;
  default:
    break;
  }
}

#define TICK_PERIOD_IN_MS 100
// Tests the verifySequence state machine.
// It prints instructions to the touch-screen. The user responds by tapping the
// correct colors to match the sequence.
// Users can test the error conditions by waiting too long to tap a color or
// by tapping an incorrect color.
void verifySequence_runTest()
{
    display_init();  // Always must do this.
        buttons_init();  // Need to use the push-button package so user can quit.
        int16_t sequenceLength = 1;  // Start out with a sequence length of 1.
        verifySequence_printInstructions(sequenceLength, false);  // Tell the user what to do.
        utils_msDelay(MESSAGE_WAIT_MS);  // Give them a few seconds to read the instructions.
        verifySequence_drawButtons();    // Now, draw the buttons.
        // Set the test sequence and it's length.
        globals_setSequence(verifySequence_testSequence, MAX_TEST_SEQUENCE_LENGTH);
        globals_setSequenceIterationLength(sequenceLength);
        verifySequence_init();    // Initialize the verifySequence state machine
        // Enable the verifySequence state machine.
        verifySequence_enable();  // Everything is interlocked, so first enable the machine.
        // Need to hold button until it quits as you might be stuck in a delay.
        while (!(buttons_read() & BUTTONS_BTN0_MASK)) {
            // verifySequence uses the buttonHandler state machine so you need to "tick" both of them.
            verifySequence_tick();  // Advance the verifySequence state machine.
            buttonHandler_tick();   // Advance the buttonHandler state machine.
            utils_msDelay(TICK_PERIOD_IN_MS);       // Wait for a tick period.
            // If the verifySequence state machine has finished, check the result,
            // otherwise just keep ticking both machines.
            if (verifySequence_isComplete()) {
                if (verifySequence_isTimeOutError()) {                // Was the user too slow?
                    verifySequence_printInfoMessage(user_time_out_e); // Yes, tell the user that they were too slow.
                } else if (verifySequence_isUserInputError()) {       // Did the user tap the wrong color?
                    verifySequence_printInfoMessage(user_wrong_sequence_e); // Yes, tell them so.
                } else {
                    verifySequence_printInfoMessage(user_correct_sequence_e); // User was correct if you get here.
                }
                utils_msDelay(MESSAGE_WAIT_MS);                            // Allow the user to read the message.
                sequenceLength = incrementSequenceLength(sequenceLength);  // Increment the sequence.
                globals_setSequenceIterationLength(sequenceLength);        // Set the length for the verifySequence state machine.
                verifySequence_printInstructions(sequenceLength, true);    // Print the instructions.
                utils_msDelay(MESSAGE_WAIT_MS);                            // Let the user read the instructions.
                verifySequence_drawButtons();                              // Draw the buttons.
                verifySequence_disable();                                  // Interlock: first step of handshake.
                verifySequence_tick();                                     // Advance the verifySequence machine.
                utils_msDelay(TICK_PERIOD_IN_MS);                          // Wait for tick period.
                verifySequence_enable();                                   // Interlock: second step of handshake.
                utils_msDelay(TICK_PERIOD_IN_MS);                          // Wait for tick period.
            }
        }
        verifySequence_printInfoMessage(user_quit_e);  // Quitting, print out an informational message.

}


