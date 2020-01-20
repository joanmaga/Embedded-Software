/*
 * flashSequence.c
 *
 *  Created on: Nov 13, 2017
 *      Author: joanmaga
 */

#include "flashSequence.h"
#include "SimonDisplay.h"
#include "globals.h"
#include "supportFiles/utils.h"
#include "supportFiles/display.h"
#include <stdio.h>

#define WAIT_POP 5  //Wait for Square to Pop for a certain time before blacking it out
#define TRUE_FLAG true  //Constant Boolean of value true
#define FALSE_FLAG false  //Constant Boolean of value false

enum flashSequence_st {
    flashSequence_init_st,      // Standard init state.
    flashSequence_wait_sequence,    //Waits for Control to pass the Sequence to this State Machine
    flashSequence_adc_st,   //Settles the Screen and receives and interprets the actions passed by the Control
    flashSequence_completed_st  //Flash Sequence was completed. Stay here
} flashSequence_currentState = flashSequence_init_st;

// Enables this state machine to leave the init state.
// static ensure that this variable is not visible outside of this file.
static bool enableFlag = false;

static uint16_t index;  //Value to determine at which Index in the Table of Sequences the Flash Sequence is
static uint16_t wait;

static bool complete = false; //Checks if sequence is completed
static bool square_on = false;  //Flag to avoid refreshing square before required time

// Turns on the state machine. Part of the interlock.
void flashSequence_enable() { enableFlag = true; }

// Turns off the state machine. Part of the interlock.
void flashSequence_disable() { enableFlag = false; }

// Other state machines can call this to determine if this state machine is finished.
bool flashSequence_isComplete(){ return complete; }

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void flashDebugStatePrint() {
  static flashSequence_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state name over and over.
  if (previousState != flashSequence_currentState || firstPass) {
    firstPass = false;                // previousState will be defined, firstPass is false.
    previousState = flashSequence_currentState;     // keep track of the last state that you were in.
    switch(flashSequence_currentState) {            // This prints messages based upon the state that you were in.
      case flashSequence_init_st: //Enter Case init_st if that is the Current State
        printf("flashSequence_init_st\n\r");  //Print Current State
        break;
      case flashSequence_wait_sequence:     //Enter Case wait_sequence if that is the Current State
        printf("flashSequence_wait_sequence\n\r");  //Print Current State
        break;
      case flashSequence_adc_st:     //Enter Case adc_Counter_running_st if that is the Current State
        printf("adcCounter\n\r");    //Check Value of Counter adcCounter for debugging purposes
        break;
      case flashSequence_completed_st:     //Enter Case Completed if that is the Current State
        printf("Completed\n\r");  //Print Current State
        break;
     }
  }
}

// Initialize the state machine
void flashSequence_init()
{
    complete = false;   //Initialize Complete Flag to false
    index = 0, wait = 0;    //Initialize the Incrementable variables to Zero
    flashSequence_currentState = flashSequence_init_st; //Initialize the Current State to the Initial State
}

// Standard tick function.
void flashSequence_tick()
{
    flashDebugStatePrint();  //Calls the Debug State Print Function for debugging purposes
    // Performs the State Update and Actions
    switch (flashSequence_currentState) {
    case flashSequence_init_st:     //Enter Case init_st if that is the Current State
        if (enableFlag) {   //Stay here is Enable flag is false
            flashSequence_currentState = flashSequence_wait_sequence;   //If Enable Flag is true, go to Wait_Sequence State
        }
        complete = false;   //Set Complete flag to false
        index = 0;  //Set incrementable variable index to 0
        wait = 0;   //Set incrementable variable wait to 0
        break;
    case flashSequence_wait_sequence:   //Enter Case wait_sequence if that is the Current State
        if (!enableFlag) {    //Base case to go back to Initial  State if State Machine was disabled
            flashSequence_currentState = flashSequence_init_st; //Goes to Initial State
        }
        else {  //Enable flag is true. Enter Else statement

            flashSequence_currentState = flashSequence_adc_st;  //Goes to ADC State
        }
        break;
    case flashSequence_adc_st:  //Enter Case adc_st if that is the Current State
        if (!enableFlag) {    //Base case to go back to Initial  State if State Machine was disabled
            flashSequence_currentState = flashSequence_init_st; //Goes to Initial State
        }
        else {  //Enable flag is true. Enter Else statement
            if (square_on) {    //Check if Square is already on
                simonDisplay_drawSquare(globals_getSequenceValue(index), TRUE_FLAG);    //Erase Square
                index++;    //Increment index
                square_on = false;  //Square is blacked out. Turn off the Square_on Flag
                wait = 0;   //Reset Wait flag
                if (index >= globals_getSequenceIterationLength()) {    //Check if index of Sequence reached end of Sequence
                    flashSequence_currentState = flashSequence_completed_st;    //If so, Flash Sequence is Completed. Go to Completed State
                    printf("COMPLETE\n\r");
                }
            }
            else {  //Square is not On. Enter Else statement
                simonDisplay_drawSquare(globals_getSequenceValue(index), FALSE_FLAG);   //Draw the Square with the Color Coding designed for that Position
                if(wait == WAIT_POP){   //Check if square is on for a certain time
                    square_on = true;   //Set Square on flag on
                }
                wait++; //Increment the wait counter
            }
        }

        break;
    case flashSequence_completed_st:    //Enter Case completed_st if that is the Current State
        if (!enableFlag) {    //Base case to go back to Initial  State if State Machine was disabled
            flashSequence_currentState = flashSequence_init_st; //Goes to Initial State
        }
        complete = true;    //Set Complete Flag to true
        break;
    }
}

// This will set the sequence to a simple sequential pattern.
// It starts by flashing the first color, and then increments the index and flashes the first
// two colors and so forth. Along the way it prints info messages to the LCD screen.
#define TEST_SEQUENCE_LENGTH 8  // Just use a short test sequence.
uint8_t flashSequence_testSequence[TEST_SEQUENCE_LENGTH] = {
    SIMON_DISPLAY_REGION_0,
    SIMON_DISPLAY_REGION_1,
    SIMON_DISPLAY_REGION_2,
    SIMON_DISPLAY_REGION_3,
    SIMON_DISPLAY_REGION_3,
    SIMON_DISPLAY_REGION_2,
    SIMON_DISPLAY_REGION_1,
    SIMON_DISPLAY_REGION_0};    // Simple sequence.
#define INCREMENTING_SEQUENCE_MESSAGE1 "Incrementing Sequence"  // Info message.
#define RUN_TEST_COMPLETE_MESSAGE "Runtest() Complete"          // Info message.
#define MESSAGE_TEXT_SIZE 2     // Make the text easy to see.
#define TWO_SECONDS_IN_MS 2000  // Two second delay.
#define TICK_PERIOD 75          // 200 millisecond delay.
#define TEXT_ORIGIN_X 0                  // Text starts from far left and
#define TEXT_ORIGIN_Y (DISPLAY_HEIGHT/2) // middle of screen.

// Print the incrementing sequence message.
void flashSequence_printIncrementingMessage() {
  display_fillScreen(DISPLAY_BLACK);  // Otherwise, tell the user that you are incrementing the sequence.
  display_setCursor(TEXT_ORIGIN_X, TEXT_ORIGIN_Y);// Roughly centered.
  display_println(INCREMENTING_SEQUENCE_MESSAGE1);// Print the message.
  utils_msDelay(TWO_SECONDS_IN_MS);   // Hold on for 2 seconds.
  display_fillScreen(DISPLAY_BLACK);  // Clear the screen.
}


// Run the test: flash the sequence, one square at a time
// with helpful information messages.
void flashSequence_runTest()
{
    display_init();                 // We are using the display.
      display_fillScreen(DISPLAY_BLACK);    // Clear the display.
      globals_setSequence(flashSequence_testSequence, TEST_SEQUENCE_LENGTH);    // Set the sequence.
      flashSequence_init();               // Initialize the flashSequence state machine
      flashSequence_enable();             // Enable the flashSequence state machine.
      int16_t sequenceLength = 1;         // Start out with a sequence of length 1.
      globals_setSequenceIterationLength(sequenceLength);   // Set the iteration length.
      display_setTextSize(MESSAGE_TEXT_SIZE); // Use a standard text size.
      while (1) {                             // Run forever unless you break.
        flashSequence_tick();             // tick the state machine.
        utils_msDelay(TICK_PERIOD);   // Provide a 1 ms delay.
        if (flashSequence_isComplete()) {   // When you are done flashing the sequence.
          flashSequence_disable();          // Interlock by first disabling the state machine.
          flashSequence_tick();             // tick is necessary to advance the state.
          utils_msDelay(TICK_PERIOD);       // don't really need this here, just for completeness.
          flashSequence_enable();           // Finish the interlock by enabling the state machine.
          utils_msDelay(TICK_PERIOD);       // Wait 1 ms for no good reason.
          sequenceLength++;                 // Increment the length of the sequence.
          if (sequenceLength > TEST_SEQUENCE_LENGTH)  // Stop if you have done the full sequence.
            break;
          // Tell the user that you are going to the next step in the pattern.
          flashSequence_printIncrementingMessage();
          globals_setSequenceIterationLength(sequenceLength);  // Set the length of the pattern.
        }
      }
      // Let the user know that you are finished.
      display_fillScreen(DISPLAY_BLACK);              // Blank the screen.
      display_setCursor(TEXT_ORIGIN_X, TEXT_ORIGIN_Y);// Set the cursor position.
      display_println(RUN_TEST_COMPLETE_MESSAGE);     // Print the message.

}


