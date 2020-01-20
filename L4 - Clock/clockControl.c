/*
 * clockControl.c
 *
 *  Created on: Oct 7, 2017
 *      Author: joanmaga
 */

#include <stdio.h>
#include "clockDisplay.h"
#include "supportFiles/display.h"

#define ADC_COUNTER_MAX_VALUE 1 //Value to be reached before ADC settles
#define AUTO_COUNTER_MAX_VALUE 10 //Value to be reached before Touch Screen starts to increment
#define RATE_COUNTER_MAX_VALUE 2    //Rate at which the Time will be incremented
#define SEC_COUNTER_MAX 20  //When this value is reached, a second is incremented in the Screen

static uint32_t adcCounter = 0, autoCounter = 0, rateCounter = 0, addOneSec = 0;

// States for the controller state machine.
enum clockControl_st_t {
    init_st,                 // Start here, transition out of this state on the first tick.
    never_touched_st,        // Wait here until the first touch - clock is disabled until set.
    waiting_for_touch_st,    // waiting for touch, clock is enabled and running.
    adc_Counter_running_st,     // waiting for the touch-controller ADC to settle.
    auto_Counter_running_st,   // waiting for the auto-update delay to expire
                                 // (user is holding down button for auto-inc/dec)
    rate_Counter_running_st,   // waiting for the rate-timer to expire to know when to perform the auto inc/dec.
    rate_Counter_expired_st,   // when the rate-timer expires, perform the inc/dec function.
    add_second_to_clock_st   // add a second to the clock time and reset the ms counter.
} currentState;

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void debugStatePrint() {
  static clockControl_st_t previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state name over and over.
  if (previousState != currentState || firstPass) {
    firstPass = false;                // previousState will be defined, firstPass is false.
    previousState = currentState;     // keep track of the last state that you were in.
    //printf("msCounter:%d\n\r", msCounter);
    switch(currentState) {            // This prints messages based upon the state that you were in.
      case init_st: //Enter Case init_st if that is the Current State
        printf("init_st\n\r");  //Print Current State
        break;
      case never_touched_st:     //Enter Case never_touched_st if that is the Current State
        printf("never_touched_st\n\r");  //Print Current State
        break;
      case waiting_for_touch_st:     //Enter Case waiting_for_touch_st if that is the Current State
        printf("waiting_for_touch_st\n\r");  //Print Current State
        printf("addOneSec: %ld\n\r", addOneSec);    //Check Value of Counter addOneSec for debugging purposes
        break;
      case adc_Counter_running_st:     //Enter Case adc_Counter_running_st if that is the Current State
        printf("ad_timer_running_st\n\r");  //Print Current State
        printf("adcCounter: %ld\n\r", adcCounter);    //Check Value of Counter adcCounter for debugging purposes
        break;
      case auto_Counter_running_st:     //Enter Case auto_Counter_running_st if that is the Current State
        printf("auto_timer_running_st\n\r");  //Print Current State
        printf("autoCounter: %ld\n\r", autoCounter);    //Check Value of Counter autoCounter for debugging purposes
        break;
      case rate_Counter_running_st:     //Enter Case rate_Counter_running_st if that is the Current State
        printf("rate_timer_running_st\n\r");  //Print Current State
        printf("rateCounter: %ld\n\r", rateCounter);    //Check Value of Counter rateCounter for debugging purposes
        break;
      case rate_Counter_expired_st:     //Enter Case rate_Counter_expired_st if that is the Current State
        printf("rate_timer_expired_st\n\r");  //Print Current State
        printf("rateCounter: %ld\n\r", rateCounter);    //Check Value of Counter rateCounter for debugging purposes
        break;
      case add_second_to_clock_st:     //Enter Case add_second_to_clock_st if that is the Current State
        printf("second_added_st\n\r");  //Print Current State
        break;
     }
  }
}

// Standard tick function.
void clockControl_init()
{
    currentState = init_st; //Initialize Current STate by setting it to the Init_st State

    adcCounter = 0, autoCounter = 0, rateCounter = 0, addOneSec = 0;    //Set all counters to Zero
}
// Call this before you call clockControl_tick().
void clockControl_tick()
{
    debugStatePrint();
    // Perform state update first.
    switch(currentState) {
        case init_st: //Enter Case init_st if that is the Current State
          currentState = never_touched_st;
          break;
        case never_touched_st:     //Enter Case never_touched_st if that is the Current State
            if(display_isTouched()) //Display was touched. If statement should Execute the body
            {
                display_clearOldTouchData();    //Clear screen from old touches
                currentState = waiting_for_touch_st;    //Go to Waiting for Touch state after first Touch
            }
            else
            {
                currentState = never_touched_st;    //Wait until the Screen is touched for the first time
            }
          break;
        case waiting_for_touch_st:     //Enter Case waiting_for_touch_st if that is the Current State
            if(addOneSec == SEC_COUNTER_MAX)
            {
                currentState = add_second_to_clock_st;  //Go to State that Adds One second to Clock
            }
            else if(display_isTouched()) //Display was touched. If statement should Execute the body
            {
                display_clearOldTouchData();    //Clear screen from old touches
                currentState = adc_Counter_running_st;  //Go to adc_Counter_running_st State
            }
            else
            {
                currentState = waiting_for_touch_st;    //Wait for user to Touch Screen
            }
          break;
        case adc_Counter_running_st:     //Enter Case adc_Counter_running_st if that is the Current State
            if(!display_isTouched() && adcCounter == ADC_COUNTER_MAX_VALUE)
            {
                clockDisplay_performIncDec();   //Perform Increment or Decrement based on position of touch
                currentState = waiting_for_touch_st;    //Wait for user to Touch Screen
            }
            else if(display_isTouched() && adcCounter == ADC_COUNTER_MAX_VALUE)
            {
                currentState = auto_Counter_running_st;  //Go to auto_Counter_running_st State
            }
            else
            {
                currentState = adc_Counter_running_st;  //Stay on the adc_Counter_running_st State
            }
          break;
        case auto_Counter_running_st:     //Enter Case auto_Counter_running_st if that is the Current State
            if(!display_isTouched() && adcCounter == ADC_COUNTER_MAX_VALUE)
            {
                clockDisplay_performIncDec();   //Perform Increment or Decrement based on position of touch
                currentState = waiting_for_touch_st;    //Wait for user to Touch Screen
            }
            else if(display_isTouched() && autoCounter == AUTO_COUNTER_MAX_VALUE)
            {
                clockDisplay_performIncDec();   //Perform Increment or Decrement based on position of touch
                currentState = rate_Counter_running_st;  //Go to rate_Counter_running_st State
            }
            else
            {
                currentState = auto_Counter_running_st;  //Stay on auto_Counter_running_st State
            }
          break;
        case rate_Counter_running_st:     //Enter Case rate_Counter_running_st if that is the Current State
            if(!display_isTouched())
            {
                currentState = waiting_for_touch_st;    //Wait for user to Touch Screen
            }
            else if(display_isTouched() && rateCounter == RATE_COUNTER_MAX_VALUE)
            {
                currentState = rate_Counter_expired_st;  //Go to rate_Counter_expired_st State
            }
            else
            {
                currentState = rate_Counter_running_st;  //Stay on rate_Counter_running_st State
            }
          break;
        case rate_Counter_expired_st:     //Enter Case rate_Counter_expired_st if that is the Current State
            if(display_isTouched()) //Display was touched. If statement should Execute the body
            {
                clockDisplay_performIncDec();   //Perform Increment or Decrement based on position of touch
                currentState = rate_Counter_running_st;  //Go to rate_Counter_running_st State
            }
            else if(!display_isTouched())
            {
                currentState = waiting_for_touch_st;    //Wait for user to Touch Screen
            }
          break;
        case add_second_to_clock_st:     //Enter Case add_second_to_clock_st if that is the Current State
            currentState = waiting_for_touch_st;    //Wait for user to Touch Screen
          break;
        default:
          printf("clockControl_tick state update: hit default\n\r");    //No state Was Selected
          break;
      }

      // Perform state action next.
      switch(currentState) {
        case init_st: //Enter Case init_st if that is the Current State
          break;
        case never_touched_st:     //Enter Case never_touched_st if that is the Current State
          break;
        case waiting_for_touch_st:     //Enter Case waiting_for_touch_st if that is the Current State
            adcCounter = 0;     //Set counter to zero. Only incremented when Screen is engaged
            autoCounter = 0;     //Set counter to zero. Only incremented when Screen is engaged
            rateCounter = 0;     //Set counter to zero. Only incremented when Screen is engaged
            addOneSec++;    //Display isn't touched. Start counting Time
          break;
        case adc_Counter_running_st:     //Enter Case adc_Counter_running_st if that is the Current State
            adcCounter++;   //Increment ADC Counter
          break;
        case auto_Counter_running_st:     //Enter Case auto_Counter_running_st if that is the Current State
            autoCounter++;  //Increment Auto Counter
          break;
        case rate_Counter_running_st:     //Enter Case rate_Counter_running_st if that is the Current State
            rateCounter++;  //Increment Rate Counter
          break;
        case rate_Counter_expired_st:     //Enter Case rate_Counter_expired_st if that is the Current State
            rateCounter = 0;
          break;
        case add_second_to_clock_st:     //Enter Case add_second_to_clock_st if that is the Current State
            clockDisplay_advanceTimeOneSecond();    //Increment a Second on the Screen
            addOneSec = 0;  //Restart Counter Once Second was incremented
          break;
         default:
          printf("clockControl_tick state action: hit default\n\r");    //No state Was Selected
          break;
      }


}



