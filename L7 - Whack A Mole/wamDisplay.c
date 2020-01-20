/*
 * wamDisplay.c
 *
 *  Created on: Dec 4, 2017
 *      Author: joanmaga
 */

#include "supportFiles/display.h"
#include "wamDisplay.h"
#include "wamControl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supportFiles/utils.h"
#include "src/Lab2/switches.h"

#define WHACK_TEXT_SIZE 3   //Text Size of Whack x
#define WHACK_X_CURSOR DISPLAY_WIDTH/6  //X Offset of Whack Message
#define WHACK_Y_CURSOR DISPLAY_HEIGHT/3   //Y Offset of Whack Message

#define START_TEXT_SIZE 2   //Text Size of Start Message
#define START_X_CURSOR DISPLAY_WIDTH/7    //X Offset of Start Message
#define START_Y_CURSOR DISPLAY_HEIGHT/2   //Y Offset of Start Message

#define GAME_OVER_TEXT_SIZE 3   //Text Size of Game Over Message
#define OVER_X_CURSOR DISPLAY_WIDTH/4    //X Offset of Game Over Message
#define OVER_Y_CURSOR DISPLAY_HEIGHT/4   //Y Offset of Game Over Message

#define OVER_STATS_TEXT_SIZE 2   //Y Offset of Stats Message
#define HITS_X_CURSOR 110    //X Offset of Stats Message
#define HITS_Y_CURSOR DISPLAY_HEIGHT - DISPLAY_HEIGHT/2   //Y Offset of Stats Message

#define MISSES_X_CURSOR 95  //X Offset of Misses
#define MISSES_Y_CURSOR DISPLAY_HEIGHT - DISPLAY_HEIGHT/2 + 20  //Y Offset of Misses

#define LEVEL_X_CURSOR 80   //X Offset of Level
#define LEVEL_Y_CURSOR DISPLAY_HEIGHT - DISPLAY_HEIGHT/2 + 40   //Y Offset of Level

#define TRY_AGAIN_X_CURSOR DISPLAY_WIDTH/8   //X Offset of Try Again Message
#define TRY_AGAIN_Y_CURSOR (DISPLAY_HEIGHT*8)/10   //Y Offset of Try Again Message

#define WIDTH_EDGE 0    //Helper Define to set Edge of Screen
#define HEIGHT_EDGE 0    //Helper Define to set Edge of Screen
#define GREEN_HEIGHT DISPLAY_HEIGHT - 30    //Height of Green Background

#define MAX_SIZE 9  //Maximum Size of Array
#define RADIUS 30   //Radius of Circle
#define OFF_X_1 DISPLAY_WIDTH/6 //X Offset of Mole Number 1
#define OFF_X_2 DISPLAY_WIDTH/2 //X Offset of Mole Number 2
#define OFF_X_3 DISPLAY_WIDTH - DISPLAY_WIDTH/6 //X Offset of Mole Number 3
#define OFF_Y_1 DISPLAY_HEIGHT/6 - 10 //Y Offset of Mole Number 1
#define OFF_Y_2 DISPLAY_HEIGHT/2 - 15 //Y Offset of Mole Number 1
#define OFF_Y_3 DISPLAY_HEIGHT - DISPLAY_HEIGHT/6 - 25 //Y Offset of Mole Number 1
#define OFF_X_STATS 0   //X Offset of the Stats Message
#define OFF_Y_STATS 212   //Y Offset of the Stats Message
#define MOLE_RADIUS 28  //Radius of pop up Mole

#define HIT_SCORE_OFFSET 4*6*OVER_STATS_TEXT_SIZE   //Offset of Hit Score Message
#define LEVEL_OFFSET 21*6*OVER_STATS_TEXT_SIZE     //Offset of Level Number Message
#define FINAL_LEVEL_OFFSET 12*6*OVER_STATS_TEXT_SIZE   //Offset of Final Level Number Message
#define MISS_OFFSET 12*6*OVER_STATS_TEXT_SIZE      //Offset of Miss Score Message
#define FINAL_MISS_OFFSET 7*6*OVER_STATS_TEXT_SIZE   //Offset of Final Miss Score Message
#define FINAL_HIT_SCORE_OFFSET 5*6*OVER_STATS_TEXT_SIZE    //Offset of Final Hit Score Message

#define SWITCH_VALUE_9 9  // Binary 9 on the switches indicates 9 moles.
#define SWITCH_VALUE_6 6  // Binary 6 on the switches indicates 6 moles.
#define SWITCH_VALUE_4 4  // Binary 9 on the switches indicates 4 moles.
#define SWITCH_MASK 0xf   // Ignore potentially extraneous bits.


//Constants to compare Number of Moles
#define MOLE_NUM_9 9
#define MOLE_NUM_6 6
#define MOLE_NUM_4 4

#define MOLE_NUM_0 0	//Index of Mole 1
#define MOLE_NUM_1 1	//Index of Mole 2
#define MOLE_NUM_2 2	//Index of Mole 3
#define MOLE_NUM_3 3	//Index of Mole 4
#define MOLE_NUM_4 4	//Index of Mole 5
#define MOLE_NUM_5 5	//Index of Mole 6
#define MOLE_NUM_6 6	//Index of Mole 7
#define MOLE_NUM_7 7	//Index of Mole 8
#define MOLE_NUM_8 8	//Index of Mole 9

#define X_OFFSET_1 0    //X Offset of Touch Number 1
#define X_OFFSET_2 320/3    //X Offset of Touch Number 2
#define X_OFFSET_3 (320*2)/3    //X Offset of Touch Number 3

#define Y_OFFSET_1 0    //Y Offset of Touch Number 1
#define Y_OFFSET_2 70    //Y Offset of Touch Number 1
#define Y_OFFSET_3 140    //Y Offset of Touch Number 1

#define ON 1    //Boolean flag with Value True
#define OFF 0   //Boolean flag with Value False

#define TRUE_FLAG true  //Boolean Flag with Value True to avoid Magic Numbers when dealing with if Statements
#define FALSE_FLAG false  //Boolean Flag with Value False to avoid Magic Numbers when dealing with if Statements

#define DELAY 100   //Delay used in Run Test

#define RUN_TEST_CASE_1 0   //Case Number 1 for the Run test if Statement
#define RUN_TEST_CASE_2 1   //Case Number 2 for the Run test if Statement
#define RUN_TEST_CASE_3 2   //Case Number 3 for the Run test if Statement

#define INVALID_INDEX -1

static uint16_t displayLevel = 0;   //Variable to Hold the Current Level Number
static uint16_t displayHit = 0;   //Variable to Hold the Current Hit Score
static uint16_t displayMiss = 0;   //Variable to Hold the Current Number of Misses
static int16_t moleNum = 0;    //Variable to Hold the Number of Max Moles

static uint16_t activeMoleNum = 0;  //Variable to hold the Number of Active Moles

static int16_t x[MAX_SIZE] = {OFF_X_1,OFF_X_3,OFF_X_1,OFF_X_3,OFF_X_2,
            OFF_X_2,OFF_X_1,OFF_X_2,OFF_X_3};
static int16_t y[MAX_SIZE] = {OFF_Y_1,OFF_Y_1,OFF_Y_3,OFF_Y_3,OFF_Y_1,
        OFF_Y_3,OFF_Y_2,OFF_Y_2,OFF_Y_2};

#define CHAR_MAX 100    //Max Value of a Char array

static int16_t untilAwakeCounter = 0, untilDormantCouner = 0;

/********************** typedefs **********************/
// This keeps track of all mole information.
typedef struct {
        wamDisplay_point_t origin;  // This is the origin of the hole for this mole.
        // A mole is active if either of the tick counts are non-zero. The mole is dormant otherwise.
        // During operation, non-zero tick counts are decremented at a regular rate by the control state machine.
        // The mole remains in his hole until ticksUntilAwake decrements to zero and then he pops out.
        // The mole remains popped out of his hole until ticksUntilDormant decrements to zero.
        // Once ticksUntilDomant goes to zero, the mole hides in his hole and remains dormant until activated again.
        wamDisplay_moleTickCount_t ticksUntilAwake;  // Mole will wake up (pop out of hole) when this goes from 1 -> 0.
        wamDisplay_moleTickCount_t ticksUntilDormant; // Mole will go dormant (back in hole) this goes 1 -> 0.
} wamDisplay_moleInfo_t;

// This will contain pointers to all of the mole info records.
// This will ultimately be treated as an array of pointers.
static wamDisplay_moleInfo_t** wamDisplay_moleInfo;

// Provide support to set games with varying numbers of moles. This function
// would be called prior to calling wamDisplay_init();
void wamDisplay_selectMoleCount(wamDisplay_moleCount_e moleCount)
{

    switch(moleCount) {
        case wamDisplay_moleCount_9:    // this switch pattern = 9 moles.
            moleNum = MOLE_NUM_9;		//Set Max Number of Moles to 9
            break;
        case wamDisplay_moleCount_6:    // this switch pattern = 6 moles.
            moleNum = MOLE_NUM_6;       //Set Max Number of Moles to 6
            break;
        case wamDisplay_moleCount_4:   // this switch pattern = 4 moles.
            moleNum = MOLE_NUM_4;       //Set Max Number of Moles to 4
            break;
        default:    // Any other pattern of switches = 9 moles.
            moleNum = MOLE_NUM_9;       //Set Max Number of Moles to 9
            break;
        }

}

// Allocates the memory for wamDisplay_moleInfo_t records.
// Computes the origin for each mole assuming a simple row-column layout:
// 9 moles: 3 rows, 3 columns, 6 moles: 2 rows, 3 columns, 4 moles: 2 rows, 2 columns
// Also inits the tick counts for awake and dormant.
void wamDisplay_computeMoleInfo() {
    // Step 2: allocate enough memory to hold NUMBER_OF_POINTERS. I need enough storage for 4
    // pointers. sizeof(myStruct*) returns the number of bytes for a single pointer to a myStruct.
    // Multiply that value by 4 to get enough memory for 4 pointers.
    // malloc() returns a (void *) so you must cast it to match the type of arrayOfPointers.
    // Rule of thumb: the number of stars (*) must match on both sides of the equation.
    // arrayOfPointers is pointer-to-pointer (myStruct**) so you need two stars (*) for the cast.
    wamDisplay_moleInfo = (wamDisplay_moleInfo_t**) malloc(moleNum * sizeof(wamDisplay_moleInfo_t*));

    // Step 3: Allocate memory for each myStruct.
    // sizeof(myStruct) returns the number of bytes required by a single myStruct.
    // The for-loop iterates 4 times, during each iteration, it grabs enough memory
    // for a single myStruct and stores it address (the pointer value) in a location in arrayOfPointers.
    for (uint16_t i=OFF; i<moleNum; i++) {
        // Second, allocate an instance of myStruct and point to it.
        wamDisplay_moleInfo[i] = (wamDisplay_moleInfo_t*) malloc(sizeof(wamDisplay_moleInfo_t));
    }
    // Step 4: assign values to these structs.
    for (uint16_t j=OFF; j<moleNum; j++) {
        wamDisplay_moleInfo[j]->origin.x = x[j];  // Just need a number.
        wamDisplay_moleInfo[j]->origin.y = y[j];  // Just need a number.
		wamDisplay_moleInfo[j]->ticksUntilAwake = 0;    //Reset ticksUntilAwake Member
        wamDisplay_moleInfo[j]->ticksUntilDormant = 0;    //Reset ticksUntilDormant Member
    }

}

// Call this before using any wamDisplay_ functions.
void wamDisplay_init()
{
    wamDisplay_computeMoleInfo();   //Set all the Mole Information
}

void wamDisplay_drawMole(int16_t moleNum, bool erase)
{
    uint16_t moleColor = 0; //Variable to hold the Mole Color
    if(!erase)  //Check if Erase Flag is up
        moleColor = DISPLAY_RED;    //Set Color to White
    else    //erase flag is not up. Enter this Else Statement
        moleColor = DISPLAY_BLACK;    //Set Color to Black

    display_fillCircle(x[moleNum], y[moleNum], MOLE_RADIUS, moleColor); //Draw the Mole. Red or Black depending on Erase Flag
}

// Draw the initial splash (instruction) screen.
void wamDisplay_drawSplashScreen()
{
	display_fillScreen(DISPLAY_BLACK);  //Fill the screen Black

    display_setTextColor(DISPLAY_WHITE);    //Set Color to White

	display_setTextSize(WHACK_TEXT_SIZE);   //Set the Text Size of this Message
    display_setCursor(WHACK_X_CURSOR, WHACK_Y_CURSOR);  //Set offset of this Message
    display_println("Whack a Mole!");   //Display Whack a Mole message

    display_setTextSize(START_TEXT_SIZE);   //Set the Text Size of this Message
    display_setCursor(START_X_CURSOR, START_Y_CURSOR);  //Set offset of this Message
    display_println("Touch Screen to Start");   //Display Touch Screen to Start message
}

// Draw the game display with a background and mole holes.
void wamDisplay_drawMoleBoard()
{
    display_fillRect(WIDTH_EDGE, HEIGHT_EDGE, DISPLAY_WIDTH, GREEN_HEIGHT, DISPLAY_GREEN);  //Draws the Mole Board

	for (int16_t i = OFF; i < moleNum; i++){  //Iterator to Draw all Circles

        display_fillCircle(x[i], y[i], RADIUS, DISPLAY_BLACK);  //Draw a Black Circle

    }

    wamDisplay_drawScoreScreen();   //Draws the Score Screen that will be output when the Game is Running


}

// Draw the game-over screen.
void wamDisplay_drawGameOverScreen()
{
    display_fillScreen(DISPLAY_BLACK);  //Fill the screen Black

	display_setTextColor(DISPLAY_WHITE);    //Set Color to White

    display_setTextSize(GAME_OVER_TEXT_SIZE);   //Set the Text Size of this Message
    display_setCursor(OVER_X_CURSOR, OVER_Y_CURSOR);  //Set offset of this Message
    display_println("Game Over");  //Display Game Over message

    display_setTextSize(OVER_STATS_TEXT_SIZE);   //Set the Text Size of this Message
    display_setCursor(HITS_X_CURSOR, HITS_Y_CURSOR);  //Set offset of this Message
    display_println("Hits:");  //Display Hits message
    display_setTextSize(START_TEXT_SIZE);   //Set the Text Size of this Message
    display_setCursor(HITS_X_CURSOR + FINAL_HIT_SCORE_OFFSET, HITS_Y_CURSOR);  //Set offset of this Message
    char text[CHAR_MAX];    //Define a Char array to take care of Variables to be displayed
    sprintf(text,"%d",displayHit);
    display_println(text);  //Display the Number of hits

    display_setCursor(MISSES_X_CURSOR, MISSES_Y_CURSOR);  //Set offset of this Message
    display_println("Misses:");  //Display Misses message
    display_setTextSize(START_TEXT_SIZE);   //Set the Text Size of this Message
    display_setCursor(MISSES_X_CURSOR + FINAL_MISS_OFFSET, MISSES_Y_CURSOR);  //Set offset of this Message
    sprintf(text,"%d",displayMiss);
    display_println(text);  //Display the Number of Misses

    display_setCursor(LEVEL_X_CURSOR, LEVEL_Y_CURSOR);  //Set offset of this Message
    display_println("Final Level:");  //Display Final Level message
    display_setCursor(LEVEL_X_CURSOR + FINAL_LEVEL_OFFSET, LEVEL_Y_CURSOR);  //Set offset of this Message
    sprintf(text,"%d",displayLevel);
    display_println(text);  //Display the Final level reached

    display_setCursor(TRY_AGAIN_X_CURSOR, TRY_AGAIN_Y_CURSOR);  //Set offset of this Message
    display_println("(Touch to Try Again)");      //Display Touch to Try Again message
}

// Test function that can be called from main() to demonstrate milestone 1.
// Invoking this function should provide the same behavior as shown in the Milestone 1 video.
void wamDisplay_runMilestone1_test()
{
    int16_t k = 0;  //Variable to handle which If statement to run
    wamDisplay_drawSplashScreen();  //Draw the First Splash Screen
    while(ON)   //Run forever
    {
        if(display_isTouched() && (k == RUN_TEST_CASE_1)){
            utils_msDelay(DELAY);   //Delay added for ADC Settlement
            wamDisplay_drawMoleBoard(); //Draw Mole Board
            k++;    //Increment k variable
        }
        if(display_isTouched() && (k == RUN_TEST_CASE_2)){
            utils_msDelay(DELAY);   //Delay added for ADC Settlement
            wamDisplay_drawGameOverScreen();    //Draw Game Over Screen
            k++;    //Increment k variable

        }
        if(display_isTouched() && (k == RUN_TEST_CASE_3)){
            utils_msDelay(DELAY);   //Delay added for ADC Settlement
            wamDisplay_drawSplashScreen();  //Draw Splash Screen
            k = 0;  //Reset k variable
        }
    }
}

// Selects a random mole and activates it.
// Activating a mole means that the ticksUntilAwake and ticksUntilDormant counts are initialized.
// See the comments for wamDisplay_moleInfo_t for details.
// Returns true if a mole was successfully activated. False otherwise. You can
// use the return value for error checking as this function should always be successful
// unless you have a bug somewhere.
bool wamDisplay_activateRandomMole()
{
    int16_t randMole = 0;   //Variable to hold the Index of the Random Mole
	randMole = rand() % moleNum;    //Randomize Variable

    untilAwakeCounter = wamControl_getRandomMoleAsleepInterval(), untilDormantCouner = wamControl_getRandomMoleAwakeInterval(); //Set COunters to Random Numbers under a Certain Range of Numbers
    if((wamDisplay_moleInfo[randMole]->ticksUntilAwake == OFF) && (wamDisplay_moleInfo[randMole]->ticksUntilDormant == OFF)){   //Checks if Mole is Active
        wamDisplay_moleInfo[randMole]->ticksUntilAwake = untilAwakeCounter;     //Update the tickUntilAwake Member
        wamDisplay_moleInfo[randMole]->ticksUntilDormant = untilDormantCouner;     //Update the ticksUntilDormant Member
        return TRUE_FLAG;   //Everything is fine. Return a nonzero Value
    }

    return FALSE_FLAG;  //Something didn't go well return a Zero Value
}

// This takes the provided coordinates and attempts to whack a mole. If a
// mole is successfully whacked, all internal data structures are updated and
// the display and score is updated. You can only whack a mole if the mole is awake (visible).
// The return value can be used during testing (you could just print which mole is
// whacked without having to implement the entire game).
wamDisplay_moleIndex_t wamDisplay_whackMole(wamDisplay_point_t* whackOrigin)
{
	uint16_t index = 0; //Initialize Index Variable

	if(whackOrigin->x < X_OFFSET_2){	//Index is either 0, 6, 2
	    if(whackOrigin->y < Y_OFFSET_2){    //Checks if Index is 0
		    index = MOLE_NUM_0;     //Updates the Index
		}
		else if(whackOrigin->y > Y_OFFSET_2 && whackOrigin->y < Y_OFFSET_3){    //Checks if Index is 6
			index = MOLE_NUM_6;     //Updates the Index
		}
		else{       //Checks if Index is 2
			index = MOLE_NUM_2;     //Updates the Index
		}
	}
	else if(whackOrigin->x > X_OFFSET_2 && whackOrigin->x < X_OFFSET_3){	//Index is either 4, 7, 5
		if(whackOrigin->y < Y_OFFSET_2){        //Checks if Index is 4
			index = MOLE_NUM_4;     //Updates the Index
		}
		else if(whackOrigin->y > Y_OFFSET_2 && whackOrigin->y < Y_OFFSET_3){        //Checks if Index is 7
			index = MOLE_NUM_7;     //Updates the Index
		}
		else{       //Checks if Index is 5
			index = MOLE_NUM_5;     //Updates the Index
		}
	}
	else{	//Index is either 1, 8, 3
		if(whackOrigin->y < Y_OFFSET_2){        //Checks if Index is 1
			index = MOLE_NUM_1;     //Updates the Index
		}
		else if(whackOrigin->y > Y_OFFSET_2 && whackOrigin->y < Y_OFFSET_3){        //Checks if Index is 8
			index = MOLE_NUM_8;     //Updates the Index
		}
		else{       //Checks if Index is 3
			index = MOLE_NUM_3;     //Updates the Index
		}
	}

	if(index >= moleNum){	//Index is INVALID
		return INVALID_INDEX;    //Return an Invalid Index
	}

	if((wamDisplay_moleInfo[index]->ticksUntilAwake == OFF) && (wamDisplay_moleInfo[index]->ticksUntilDormant != OFF)){     //Checks to see if Mole is Awake
        displayHit++;   //Increment Hit Count
		activeMoleNum--;    //Decrement Active Mole Count
		wamDisplay_moleInfo[index]->ticksUntilDormant = 0;  //Mole was Hit. Deactivate it
		wamDisplay_setHitScore(displayHit); //Updates Hits on Display
		wamDisplay_drawMole(index, TRUE_FLAG);  //Blacks out the Mole
		return index;   //Returns the Index of the Mole
	}
    return INVALID_INDEX;   //Returns an Invalid index
}

// This updates the ticksUntilAwake/ticksUntilDormant clocks for all of the moles.
void wamDisplay_updateAllMoleTickCounts()
{
    for(int16_t i = OFF; i < moleNum; i++){    //Thats MAX Mole Board
        if(wamDisplay_moleInfo[i]->ticksUntilAwake != OFF){ //Cheks to see if Mole is asleep
            wamDisplay_moleInfo[i]->ticksUntilAwake--;  //Decrements the Tick
            if(wamDisplay_moleInfo[i]->ticksUntilAwake == OFF){ //Checks to see if Mole is awake
                wamDisplay_drawMole(i, FALSE_FLAG); //If so, Pop Mole out
            }
        }
        else if(wamDisplay_moleInfo[i]->ticksUntilDormant != OFF){ //Cheks to see if Mole is not Dormant
            wamDisplay_moleInfo[i]->ticksUntilDormant--;    //Decrements the Tick
            if(wamDisplay_moleInfo[i]->ticksUntilDormant == OFF){   //Checks to see if Mole is Dormant
				displayMiss++;	//User missed the Mole. Increment Miss Variable
				wamDisplay_incrementMissScore();	//Update the Miss Score on the Display
				activeMoleNum--;    //Decrements the Active Mole Count
                wamDisplay_drawMole(i, TRUE_FLAG);  //Black Out Mole

            }
        }
    }
}

// Returns the count of currently active moles.
// A mole is active if it is not dormant, if:
// ticksUntilAwake or ticksUntilDormant are non-zero (in the moleInfo_t struct).
uint16_t wamDisplay_getActiveMoleCount()
{

    uint16_t moleCount = 0; //Variable that return the Number of Active Moles
    for(int16_t i = OFF; i < moleNum; i++){   //Goes over all the moles to see which ones are awake
        if((wamDisplay_moleInfo[i]->ticksUntilAwake != OFF) || (wamDisplay_moleInfo[i]->ticksUntilDormant != OFF)){
            moleCount++;    //Increments the Mole Count if the Mole is awake
        }
    }
    return moleCount;   //Returns the Active Mole Count
}

// Sets the hit value in the score window.
void wamDisplay_setHitScore(uint16_t hits)
{
    display_setTextColor(DISPLAY_BLACK);    //Set the COlot of Text to Black
    display_setTextSize(START_TEXT_SIZE);   //Set Text Size
    display_setCursor(OFF_X_STATS + HIT_SCORE_OFFSET, OFF_Y_STATS);  //Set offset of this Message
    uint16_t prevHits = hits - ON;   //Set the previous Number of Hits
    char text[CHAR_MAX];    //Declare Char array with Decent Number of Characters
    sprintf(text,"%d",prevHits);
    display_println(text);  //Display Text

    display_setTextColor(DISPLAY_WHITE);    //Set the COlot of Text to White
    display_setTextSize(START_TEXT_SIZE);  //Set Text Size
    display_setCursor(OFF_X_STATS + HIT_SCORE_OFFSET, OFF_Y_STATS); //Set offset of this Message
    sprintf(text,"%d",hits);
    display_println(text);  //Display Text
}

// Gets the current hit value.
uint16_t wamDisplay_getHitScore()
{
    return displayHit;  //Returns the Number of Hits
}

// Sets the level value on the score board.
void wamDisplay_incrementLevel()
{
	displayLevel++; //Increments the Level Count
    display_setTextColor(DISPLAY_BLACK);    //Set the COlot of Text to Black
    display_setTextSize(START_TEXT_SIZE);   //Set Text Size
    display_setCursor(OFF_X_STATS + LEVEL_OFFSET, OFF_Y_STATS);  //Set offset of this Message
    uint16_t prevLevel = displayLevel - ON;   //Set the previous Level Number
    char text[CHAR_MAX];    //Declare Char array with Decent Number of Characters
    sprintf(text,"%d",prevLevel);
    display_println(text);  //Display Text

    display_setTextColor(DISPLAY_WHITE);    //Set the COlot of Text to White
    display_setTextSize(START_TEXT_SIZE);   //Set Text Size
    display_setCursor(OFF_X_STATS + LEVEL_OFFSET, OFF_Y_STATS);  //Set offset of this Message
    sprintf(text,"%d",displayLevel);
    display_println(text);  //Display Text
}

// Retrieves the current level value.
// Can be used for testing and other functions.
uint16_t wamDisplay_getLevel()
{
    return displayLevel;    //Returns Level Count
}

// Completely draws the score screen.
// This function renders all fields, including the text fields for "Hits" and "Misses".
// Usually only called once when you are initializing the game.
void wamDisplay_drawScoreScreen()
{
    display_setTextColor(DISPLAY_WHITE);    //Set the COlot of Text to White
    display_setTextSize(START_TEXT_SIZE);   //Set Text Size
    display_setCursor(OFF_X_STATS, OFF_Y_STATS);  //Set offset of this Message
    display_println("Hit:0  Miss:0  Level:0");  //Display Score Screen Text
}

// Make this function available for testing purposes.
void wamDisplay_incrementMissScore()
{
    display_setTextColor(DISPLAY_BLACK);    //Set the COlot of Text to Black
    display_setTextSize(START_TEXT_SIZE);   //Set Text Size
    display_setCursor(OFF_X_STATS + MISS_OFFSET, OFF_Y_STATS);  //Set offset of this Message
    uint16_t prevMiss = displayMiss - ON;   //Set the previous Number of Misses
    char text[CHAR_MAX];    //Declare Char array with Decent Number of Characters
    sprintf(text,"%d",prevMiss);
    display_println(text);  //Display Text

    display_setTextColor(DISPLAY_WHITE);    //Set the COlot of Text to White
    display_setTextSize(START_TEXT_SIZE);   //Set Text Size
    display_setCursor(OFF_X_STATS + MISS_OFFSET, OFF_Y_STATS);  //Set offset of this Message
    sprintf(text,"%d",displayMiss);
    display_println(text);  //Display Text
}

// Sets the miss value in the score window.
void wamDisplay_setMissScore(uint16_t misses)
{
    displayMiss = misses;   //Sets the Number of Misses
}

// Gets the miss value.
// Can be used for testing and other functions.
uint16_t wamDisplay_getMissScore()
{
    return displayMiss; //Returns the Number of Misses
}

// Reset the scores and level to restart the game.
void wamDisplay_resetAllScoresAndLevel()
{
    //Reset all Counters to Zero
    displayLevel = 0;
    displayHit = 0;
    displayMiss = 0;
	activeMoleNum = 0;
	wamControl_setMaxActiveMoles(ON);   //Set Max Number of Active moles to 1

    // When you are done, you must return the memory to the system or you will create a memory leak.
        // First deallocate all of the structs.
        for (uint16_t l=0; l<moleNum; l++) {
            free(wamDisplay_moleInfo[l]);   // This deallocates the memory.
            wamDisplay_moleInfo[l] = NULL;  // This step is not necessary but will keep you from reusing deallocated memory.
        }
        // Next, deallocate the array that contains the pointers to the structs.
        free(wamDisplay_moleInfo);   // Deallocates the container of arrays.
        wamDisplay_moleInfo = NULL;  // Also keeps you from reusing the deallocated memory.
}
