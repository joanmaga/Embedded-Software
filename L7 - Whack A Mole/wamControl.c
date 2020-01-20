#include "wamControl.h"
#include "supportFiles/display.h"
#include "wamDisplay.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_MISS_COUNT 50   //Misses to get Game Over
#define HITS_PER_LEVEL 5    //Max Number of Hits before advancing a Level

#define TRUE_FLAG true  //Boolean Flag with Value True
#define FALSE_FLAG false  //Boolean Flag with Value False

#define ON 0
#define OFF 0

//Arithmetic Variables to hold numbers for Algorithm of Rand Functions
#define RAND_FUNC_VARIABLE_1 21
#define RAND_FUNC_VARIABLE_2 10

#define INVALID -1  //Variable to hold Invalid number

//Create and initialize all Counters used in the Control
static uint16_t maxMoles = 0;
static uint32_t controlSeed = 0;
static uint16_t maxMiss = 0;
static uint16_t tickMiliseconds = 0;

//Variables that will hold the Coordinates of the Touch on the Screen
static int16_t x = 0, y = 0;
static uint8_t z = 0;

static wamDisplay_point_t whackLocation;   //used to send the Location of the Whack Touch

enum wamControl_st {
    wamControl_waitForTouch_st,  // Spin here until the enable flag becomes true.
    wamControl_adc_counter,  // waiting for the touch-controller ADC to settle.
    wamControl_coordinates_st      // You are here if you have flashed all of the LEDs.
} wamControl_currentState = wamControl_waitForTouch_st;

void debugStatePrint() {
  static wamControl_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state name over and over.
  if (previousState != wamControl_currentState || firstPass) {
    firstPass = false;                // previousState will be defined, firstPass is false.
    previousState = wamControl_currentState;     // keep track of the last state that you were in.
    switch(wamControl_currentState) {            // This prints messages based upon the state that you were in.
      case wamControl_waitForTouch_st: //Enter Case init_st if that is the Current State
        printf("wamControl_waitForTouch_st\n\r");  //Print Current State
        break;
      case wamControl_adc_counter:     //Enter Case wait_display_screen if that is the Current State
        printf("wamControl_adc_counter\n\r");  //Print Current State
        break;
      case wamControl_coordinates_st:     //Enter Case waiting_for_touch_st if that is the Current State
        printf("wamControl_coordinates_st\n\r");  //Print Current State
        break;
     }
  }
}

// Call this before using any wamControl_ functions.
void wamControl_init()
{
    wamControl_currentState = wamControl_waitForTouch_st;   //Initialize the State Machine to be at Wait For Touch
}

// Standard tick function.
void wamControl_tick()
{
    debugStatePrint();  //Call Debug Function
    wamDisplay_updateAllMoleTickCounts();   //Updates all Tick Counts
    switch (wamControl_currentState) {
        case wamControl_waitForTouch_st: //Enter Case init_st if that is the Current State
            if(display_isTouched()){    //Checks if Display was Touched
                display_clearOldTouchData();    //Clear any Previous Touch Data
                wamControl_currentState = wamControl_adc_counter;   //Display was touched. Go to ADC Counter State
            }
            if (wamDisplay_getActiveMoleCount() < wamControl_getMaxActiveMoles()){  //Keep Track of Active Moles and CHecks if Theres less than the Number of Active Moles Needed
                wamDisplay_activateRandomMole();    //If so, Activate another Mole
            }
            break;
        case wamControl_adc_counter:
            if (wamDisplay_getActiveMoleCount() < wamControl_getMaxActiveMoles()){ //Keep Track of Active Moles and CHecks if Theres less than the Number of Active Moles Needed
                wamDisplay_activateRandomMole();    //If so, Activate another Mole
            }
            wamControl_currentState = wamControl_coordinates_st;    //Go to Coordinates State
            break;
        case wamControl_coordinates_st:
            if (wamDisplay_getActiveMoleCount() < wamControl_getMaxActiveMoles()){  //Keep Track of Active Moles and CHecks if Theres less than the Number of Active Moles Needed
                wamDisplay_activateRandomMole();    //If so, Activate another Mole
            }
            display_getTouchedPoint(&x, &y, &z);    //Get Touch Information
            //Sets the Coordinates of the Touch
            whackLocation.x = x;
            whackLocation.y = y;
        int16_t moleIndex = wamDisplay_whackMole(&whackLocation);   //Variable to hold the Index of the Mole Location that was touched
            if(moleIndex != INVALID){   //Checks to see if Mole Index is Valid
                if((wamDisplay_getHitScore() > OFF) && (wamDisplay_getHitScore() % HITS_PER_LEVEL == OFF)){ ////Checks to see if we need to Update the Level
                    wamDisplay_incrementLevel();    //If so, Update the Level
                }
            }
            wamControl_currentState = wamControl_waitForTouch_st;   //Go to Wait For Touch State
            maxMoles = wamDisplay_getLevel() + ON;  //Set the Max Number of Moles that can be active at the Same Time
            wamControl_setMaxActiveMoles(maxMoles); //Sets the Number into the Function to update the Global variable

           break;
    }
}

// Call this to set how much time is consumed by each tick of the controlling state machine.
// This information makes it possible to set the awake and sleep time of moles in ms, not ticks.
void wamControl_setMsPerTick(uint16_t msPerTick)
{
    tickMiliseconds = msPerTick;    //Sets the Variable of msPerTicks
}

// This returns the time consumed by each tick of the controlling state machine.
uint16_t wamControl_getMsPerTick()
{
    return tickMiliseconds; //Returns the Number of Ms Per Tick
}

// Returns a random value that indicates how long the mole should sleep before awaking.
wamDisplay_moleTickCount_t wamControl_getRandomMoleAsleepInterval()
{
    return rand()%(RAND_FUNC_VARIABLE_1 + wamDisplay_getLevel()) + RAND_FUNC_VARIABLE_2;
}

// Returns a random value that indicates how long the mole should stay awake before going dormant.
wamDisplay_moleTickCount_t wamControl_getRandomMoleAwakeInterval()
{
    return rand()%(RAND_FUNC_VARIABLE_1 + wamDisplay_getLevel()) + RAND_FUNC_VARIABLE_2;
}

// Set the maximum number of active moles.
void wamControl_setMaxActiveMoles(uint16_t count)
{
    maxMoles = count;   //Assign number of Active Moles to a Variable
}

// Get the current allowable count of active moles.
uint16_t wamControl_getMaxActiveMoles()
{
    return maxMoles;    //Returns Number of Active Moles
}

// Set the seed for the random-number generator.
void wamControl_setRandomSeed(uint32_t seed)
{
    srand(seed);    //Randomizes the seed Number
}

uint32_t wamControl_getRandomSeed()
{
    return controlSeed;     //Returns the Value of Seed
}

// Set the maximum number of misses until the game is over.
void wamControl_setMaxMissCount(uint16_t missCount)
{
    maxMiss = missCount;       //Assign number of Max Misses to a Variable
}

// Use this predicate to see if the game is finished.
bool wamControl_isGameOver()
{
    if(wamDisplay_getMissScore() == MAX_MISS_COUNT) //Checks to see if the Miss Score has reached its max value
        return TRUE_FLAG;   //If so, return true

    return FALSE_FLAG;  //If not, return False
}
