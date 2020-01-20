/*
 * simonControl.c
 *
 *  Created on: Nov 28, 2017
 *      Author: joanmaga
 */
#include "simonControl.h"
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "SimonDisplay.h"
#include "supportFiles/display.h"
#include "verifySequence.h"
#include "flashSequence.h"

#define SIMON_TEXT_CURSOR_X DISPLAY_WIDTH / 3
#define SIMON_TEXT_CURSOR_Y DISPLAY_HEIGHT / 3 //Offset to help with the positioning of texts 1
#define TOUCH_TO_START_CURSOR_X DISPLAY_WIDTH / 4 //"touch to start" cursor x
#define TOUCH_TO_START_CURSOR_Y (DISPLAY_HEIGHT / 2) //"touch to start" cursor y
#define TOUCH_TO_START_TEXT_SIZE 2 //"touch to start" text size
#define SIMON_TEXT_SIZE 5 //"SIMON" text size
#define YAY_SIZE 5 //"YAY!" text size
#define YAY_TEXT_CURSOR_X DISPLAY_WIDTH / 3 //"YAY!" text cursor x
#define YAY_TEXT_CURSOR_Y DISPLAY_HEIGHT / 3 //Offset to help with the positioning of texts 2
#define TOUCH_FOR_NEW_LEVEL_TEXT_SIZE 2 //"touch for new level" text size
#define CURRENT_SEQUENCE_ITERATION_LENGTH_DEFAULT 1 //current sequence default size
#define TOUCH_FOR_NEW_LEVEL_X DISPLAY_WIDTH / 8 //"touch for new level" cursor x
#define TOUCH_FOR_NEW_LEVEL_Y DISPLAY_HEIGHT / 2 //"touch for new level" cursor y
#define LONGEST_SEQUENCE_CURSOR_X DISPLAY_WIDTH / 8 //"longest sequence: " cursor x
#define LONGEST_SEQUENCE_CURSOR_Y DISPLAY_HEIGHT / 2 //"longest sequence: " cursor y

#define TRUE_FLAG true  //Constant flag to have a value True
#define FALSE_FLAG false  //Constant flag to have a value False

#define SEQUENCE_MIN_NUM 4  //Start Sequence Length Value
#define REGION_MAX_NUM 4    //Number of Regions in the Display
#define RESET_TIMED_OUT 0   //Reset time out with this COnstant
#define TIMED_OUT_MAX 10 //MIGHT NEED TO CHANGE THIS VALUE
#define DEFAULT_ITER_LENGTH 1   //Iteration length starts at 1
#define YAY_MAX 5   //Maximum counter number of Yay can achieve
#define RESET_COUNTER 0 //Reset counters to Zero
#define MESSAGE_DELAY_MAX 25    //Max value that Delay can reach

#define LONGEST_SEQUENCE_ARRAY 25   //Maximum size of the following array
static char lengthSequenceArray[LONGEST_SEQUENCE_ARRAY];    //Declare an array of characters with size 25

enum control_st {
    control_init_st,      // Standard init state.
    control_waitForTouch_st,  // Spin here until the enable flag becomes true.
    control_flash_st,     //This handles the flash sequence
    control_verify_st,  // This handles the verify sequence
    control_yay_st,      // State to display that the Sequence was done correctly
	control_complete_st	//Transition to this State if everything is done
} control_currentState = control_init_st;

static int16_t randSequence = SEQUENCE_MIN_NUM; //Initialize random Sequence Length to 4
static uint16_t seed = 0;   //Seed to be used in the randomizing FUnction
static uint16_t timedOut = 0;   //Timed Out counter that will take care when user times out
static int16_t userSeqLen = 0;		//Length of the Sequence the user is currently doing
static uint16_t currentSeqIterLen = DEFAULT_ITER_LENGTH;    //Initialize the Sequence iteration Length
static uint16_t bestSequence;   //Variable to handle the best Sequence
static uint16_t yay = 0;	//Counter to take care of Congratulating Message
static uint16_t msgDelay = 0;   //Counter to wait a Moment to display message

void startText(bool notErase) { //Function to handle the the writing of the Start Text
	display_setCursor(SIMON_TEXT_CURSOR_X, SIMON_TEXT_CURSOR_Y);    //Set Cursor to set position
	if(notErase == TRUE_FLAG)   //Checks to see if notErase flag is up
	        display_setTextColor(DISPLAY_WHITE);    //Set Color to White
	    else    //notErase flag is not up. Enter this Else Statement
	        display_setTextColor(DISPLAY_BLACK);    //Set Color to Black
	display_setTextSize(SIMON_TEXT_SIZE);
	display_println("SIMON");
	display_setCursor(TOUCH_TO_START_CURSOR_X, TOUCH_TO_START_CURSOR_Y);    //Set Cursor to set position
	if(notErase == TRUE_FLAG)   //Checks to see if notErase flag is up
		display_setTextColor(DISPLAY_WHITE);    //Set Color to White
	else  //notErase flag is not up. Enter this Else Statement
		display_setTextColor(DISPLAY_BLACK);    //Set Color to Black
	display_setTextSize(TOUCH_TO_START_TEXT_SIZE);
	display_println("Touch to start");
}

void touchNewLvl(bool notErase) {
	display_setCursor(TOUCH_FOR_NEW_LEVEL_X, TOUCH_FOR_NEW_LEVEL_Y);    //Set Cursor to set position
	if (notErase == TRUE_FLAG)   //Checks to see if notErase flag is up
		display_setTextColor(DISPLAY_WHITE);    //Set Color to White
	else       //notErase flag is not up. Enter this Else Statement
		display_setTextColor(DISPLAY_BLACK);    //Set Color to Black
	display_setTextSize(TOUCH_FOR_NEW_LEVEL_TEXT_SIZE);
	display_println("Touch for new level");
}

void yayDisplay(bool notErase) {
	display_setCursor(YAY_TEXT_CURSOR_X, YAY_TEXT_CURSOR_Y);    //Set Cursor to set position
	if (notErase == TRUE_FLAG)   //Checks to see if notErase flag is up
		display_setTextColor(DISPLAY_WHITE);
	else       //notErase flag is not up. Enter this Else Statement
		display_setTextColor(DISPLAY_BLACK);    //Set Color to Black
	display_setTextSize(YAY_SIZE);
	display_println("YAY!");
}

//Function to randomize the Items in the Array
void randFunc(uint32_t seed)
{
    srand(seed);    //Randomize function after a certain time
    uint8_t randArray[randSequence];    //Declare an array of size randSequence
    for(uint16_t i = 0; i < randSequence; i++)  //For loop to add a value to each position in the array
    {
        randArray[i] = rand() % REGION_MAX_NUM; //Adds a value to each position in the array
    }
    globals_setSequence(randArray, randSequence);   //Set the Global Sequence
}

// Initialize the state machine
void simonControl_init()
{
    control_currentState = control_init_st; //Set the Initial state to be control_init_st
    display_init(); //Initialize the Display
    display_fillScreen(DISPLAY_BLACK);  //Fill the screen Black
}

// Standard tick function.
void simonControl_tick()
{
    switch(control_currentState){   //Switch Statement to handle the Actions and State Transitions
        case control_init_st:   //Enter the Init_st State
            timedOut = RESET_TIMED_OUT; //Reset TimedOut Counter
            userSeqLen = SEQUENCE_MIN_NUM;  //Reset UserseqLen Variable
			startText(TRUE_FLAG);   //Display Start Text
            control_currentState = control_waitForTouch_st; //Transition to Wait for Touch State
			bestSequence = SEQUENCE_MIN_NUM; //Default Value for Best sequence
            break;
        case control_waitForTouch_st:   //Enter the waitforTouch_st State
			seed++; //Increment Seed Counter
			timedOut++; //Increment TimedOut Counter
			if ((timedOut == TIMED_OUT_MAX) && (userSeqLen > SEQUENCE_MIN_NUM)) {	//User done the First Sequence now we need to give him the option to Time Out
				touchNewLvl(FALSE_FLAG);	//Erase Touch New Level Message
				currentSeqIterLen = DEFAULT_ITER_LENGTH;    //Reset Current Sequence Iter Len variable
				timedOut = RESET_TIMED_OUT; //Reset Timed out Counter
				control_currentState = control_complete_st; //transition to Complete State
			}
            else if(display_isTouched()){   //Check if Display was touched
				startText(FALSE_FLAG);  //Erase Start Text
                globals_setSequenceIterationLength(currentSeqIterLen);  //Update Global Sequence Iteration Length
                flashSequence_enable(); //Enable the Flash Sequence State Machine
                randFunc(seed); //Re-Randomize Array
				touchNewLvl(FALSE_FLAG);	//Erase Touch New level Message
                control_currentState = control_flash_st;    //Transition to Flash State
            }
            break;
        case control_flash_st:     //Enter the control_flash_st State
            seed++; //Increment Seed Counter
            if(flashSequence_isComplete()){ //Check if Flash Sequence is Complete
                flashSequence_disable();    //Disable Flash Sequence State Machine
				verifySequence_enable();    //Enable Verify Sequence State Machine
                simonDisplay_drawAllButtons();  //Draw all buttons in the Screen
                control_currentState = control_verify_st;   //Transition to Verify Sequence State
            }
            break;
        case control_verify_st:      //Enter the control_verify_st State
			if ((verifySequence_isComplete()) && (verifySequence_isTimeOutError() || verifySequence_isUserInputError())) {
				simonDisplay_eraseAllButtons(); //Erase all Buttons in the Screen
				verifySequence_disable();   //Disable verify Sequence State Machine
				userSeqLen = SEQUENCE_MIN_NUM;  //Reset userSeqLen Variable
				control_currentState = control_complete_st; //Transition to the Complete State
			}
			else if (verifySequence_isComplete() && (globals_getSequenceIterationLength() < userSeqLen)) {
				simonDisplay_eraseAllButtons(); //Erase all Buttons in the Screen
				verifySequence_disable(); //Disable verify Sequence State Machine
				flashSequence_enable(); //Enable the Flash Sequence State Machine
				currentSeqIterLen++;    //Increment the Iteration Length
				globals_setSequenceIterationLength(currentSeqIterLen);  //Update the Sequence Iteration Length
				msgDelay = RESET_COUNTER;   //Reset the Message COunter
				control_currentState = control_flash_st;  //Transition to the Flash State
			}
			else if (verifySequence_isComplete() && (globals_getSequenceIterationLength() == userSeqLen)) {
				simonDisplay_eraseAllButtons(); //Erase all Buttons in the Screen
				userSeqLen++;   //Increment the SeqLen
				bestSequence++; //Increment the Best Sequence since user passed the Level
				verifySequence_disable();   //Disable the Verify Sequence State Machine
				currentSeqIterLen = DEFAULT_ITER_LENGTH;    //Reset the Iteration Length Counter
				globals_setSequenceIterationLength(currentSeqIterLen);    //Update the Sequence Iteration Length
				randFunc(seed); //Re-Randomize the Array
				yayDisplay(TRUE_FLAG);  //Display Yay Message
				timedOut = RESET_TIMED_OUT; //Reset Timed Out Counter
				control_currentState = control_yay_st;  //Transition to Yay State
			}
            break;
        case control_yay_st:     //Enter the control_yay_st State
			yay++;  //Increment the yay Counter
			seed++; //Increment Seed Counter
			if (yay >= YAY_MAX) {   //Check if yay Counter has reached its max value
				yayDisplay(FALSE_FLAG); //Clear the Yay Message on Screen
				yay = RESET_COUNTER;    //Reset yay Counter
				control_currentState = control_waitForTouch_st; //Transition to the Wait for Touch State
				touchNewLvl(TRUE_FLAG); //Display Touch for New Level Message
			}
            break;
		case control_complete_st:     //Enter the control_complete_st State
			msgDelay++; //Increment Msg Delay COunter
			if (msgDelay >= MESSAGE_DELAY_MAX) {    //Check if Msg Delay counter has reached its max value
				userSeqLen = SEQUENCE_MIN_NUM;  //Reset User Seq len
				if (bestSequence == SEQUENCE_MIN_NUM) { //Check if the Best Sequence if the Miniumum Sequence
					display_setCursor(LONGEST_SEQUENCE_CURSOR_X,LONGEST_SEQUENCE_CURSOR_Y); //Set Cursor to this position
					display_setTextColor(DISPLAY_BLACK);    //Set Color to Black
					display_println("Longest Sequence: 0"); //Display Longest Sequence is Zero
				}
				else {  //Best Sequence is not 4. Display higher Sequence
					display_setCursor(LONGEST_SEQUENCE_CURSOR_X,LONGEST_SEQUENCE_CURSOR_Y); //Set Cursor to this Position
					display_setTextColor(DISPLAY_BLACK);    //Set Color to Black
					sprintf(lengthSequenceArray, "Longest Sequence: %d\n\r", bestSequence - DEFAULT_ITER_LENGTH);   //update the Length Sequence Array Variable
					display_println(lengthSequenceArray);   //Print this Value on Screen
				}
				touchNewLvl(FALSE_FLAG);    //Erase Tocuh for New Level Message
				msgDelay = RESET_COUNTER;   //Reset the msg Delay Counter
				control_currentState = control_init_st; //Transition to the Init State
			}
			else{
			    userSeqLen = SEQUENCE_MIN_NUM; //Reset User Seq len
                    if (bestSequence == SEQUENCE_MIN_NUM) {//Check if the Best Sequence if the Miniumum Sequence
                        display_setCursor(LONGEST_SEQUENCE_CURSOR_X,LONGEST_SEQUENCE_CURSOR_Y);    //Set Cursor to set position
                        display_setTextColor(DISPLAY_WHITE);    //Set Color to White
                        display_println("Longest Sequence: 0"); //Display Longest Sequence is Zero
                    }
                    else {
                        display_setCursor(LONGEST_SEQUENCE_CURSOR_X,LONGEST_SEQUENCE_CURSOR_Y);    //Set Cursor to set position
                        display_setTextColor(DISPLAY_WHITE);    //Set Color to White
                        sprintf(lengthSequenceArray, "Longest Sequence: %d\n\r", bestSequence - DEFAULT_ITER_LENGTH);   //update the Length Sequence Array Variable
                        display_println(lengthSequenceArray);   //Print this Value on Screen
                    }
			}
			break;
    }
}




