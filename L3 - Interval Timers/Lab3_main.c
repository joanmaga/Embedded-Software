/*
 * Lab3_main.c
 *
 *  Created on: Sep 23, 2017
 *      Author: joanmaga
 */

#include "intervalTimer.h"  // Modify this to reflect the location of your intervalTimer.h
#include "src/Lab2/buttons.h"        // Modify this to reflect the location of your buttons.h
#include "supportFiles/utils.h"
#include <stdio.h>
#include "xil_io.h"

#define TCR0_OFFSET 0x08  // register offset for TCR0
#define TCR1_OFFSET 0x18  // register offset for TCR1

// Reads the timer1 registers based upon the offset.
u32 readTimer1Register(uint32_t registerOffset) {
  uint32_t address = XPAR_AXI_TIMER_0_BASEADDR + registerOffset;  // Add the offset to the base address.
  return Xil_In32(address);  // Read the register at that address.
}

#define DELAY_COUNT 3
// Simple busy-wait function.
void waitALongTime() {
  volatile int32_t a = 0;        // Use volatile so that optimizer doesn't mess things up.
  int32_t i, j;  // Simple index variables.
  for (i=0; i<DELAY_COUNT; i++)        // Outer delay-loop.
    for (j=0; j<INT32_MAX; j++)  // Inner delay-loop.
      a++;
}

void milestone1() {
  printf("=============== Starting milestone 1 ===============\n\r");
  intervalTimer_init(INTERVAL_TIMER_TIMER_0);  // Init timer 0.
  intervalTimer_reset(INTERVAL_TIMER_TIMER_0); // Reset timer 0.
  // Show that the timer is reset.
  // Check lower register.
  printf("timer_0 TCR0 should be 0 at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  // Check upper register.
  printf("timer_0 TCR1 should be 0 at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));
  intervalTimer_start(INTERVAL_TIMER_TIMER_0);  // Start timer 0.
  // Show that the timer is running.
  printf("The following register values should be changing while reading them.\n\r");
  // Just checking multiple times to see if the timer is running.
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  printf("timer_0 TCR0 should be changing at this point:%ld\n\r", readTimer1Register(TCR0_OFFSET));
  // Wait about 2 minutes so that you roll over to TCR1.
  // If you don't see a '1' in TCR1 after this long wait you probably haven't programmed the timer correctly.
  waitALongTime();
  // Check lower register.
  printf("timer_0 TCR0 value after wait:%lx\n\r", readTimer1Register(TCR0_OFFSET));
  // Check upper register.
  printf("timer_0 TCR1 should have changed at this point:%ld\n\r", readTimer1Register(TCR1_OFFSET));
}

#define TEST_ITERATION_COUNT 4
#define ONE_SECOND_DELAY 1000
void milestone2() {
  printf("=============== Starting milestone 2 ===============\n\r");
  double duration0, duration1, duration2;  // Will hold the duration values for the various timers.
  buttons_init();           // init the buttons package.
  intervalTimer_initAll();  // init all of the interval timers.
  intervalTimer_resetAll(); // reset all of the interval timers.
  // Poll the push-buttons waiting for BTN0 to be pushed.
  printf("Interval Timer Accuracy Test\n\r");     // User status message.
  printf("waiting until BTN0 is pressed.\n\r");   // Tell user what you are waiting for.
  while (!(buttons_read() & BUTTONS_BTN0_MASK));  // Loop here until BTN0 pressed.
  // Start all of the interval timers.
  intervalTimer_start(INTERVAL_TIMER_TIMER_0);
  intervalTimer_start(INTERVAL_TIMER_TIMER_1);
  intervalTimer_start(INTERVAL_TIMER_TIMER_2);
  printf("started timers.\n\r");
  printf("waiting until BTN1 is pressed.\n\r");  // Poll BTN1.
  while (!(buttons_read() & BUTTONS_BTN1_MASK)); // Loop here until BTN1 pressed.
  // Stop all of the timers.
  intervalTimer_stop(INTERVAL_TIMER_TIMER_0);
  intervalTimer_stop(INTERVAL_TIMER_TIMER_1);
  intervalTimer_stop(INTERVAL_TIMER_TIMER_2);
  printf("stopped timers.\n\r");
  // Get the duration values for all of the timers.
  duration0 = intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_0);
  duration1 = intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_1);
  duration2 = intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_2);
  // Print the duration values for all of the timers.
  printf("Time Duration 0: %6.2e seconds.\n\r", duration0);
  printf("Time Duration 1: %6.2e seconds.\n\r", duration1);
  printf("Time Duration 2: %6.2e seconds.\n\r", duration2);
  // Now, test to see that all timers can be restarted multiple times.
  printf("Iterating over fixed delay tests\n\r");
  printf("Delays should approximately be: 1, 2, 3, 4 seconds.\n\r");
  printf("Note that delays may be about 25%%-30%% lower if your compiler optimizations are set to -O2 or -O3\n\r");
  for (int i=0; i<TEST_ITERATION_COUNT; i++) {
    // Reset all the timers.
    intervalTimer_resetAll();
    // Start all the timers.
    intervalTimer_start(INTERVAL_TIMER_TIMER_0);
    intervalTimer_start(INTERVAL_TIMER_TIMER_1);
    intervalTimer_start(INTERVAL_TIMER_TIMER_2);
    // Delay is based on the loop count.
    utils_msDelay((i+1)*ONE_SECOND_DELAY);
    // Stop all of the timers.
    intervalTimer_stop(INTERVAL_TIMER_TIMER_0);
    intervalTimer_stop(INTERVAL_TIMER_TIMER_1);
    intervalTimer_stop(INTERVAL_TIMER_TIMER_2);
    // Print the duration of all of the timers. The delays should be approximately 1, 2, 3, and 4 seconds.
    printf("timer:(%d) duration:%f\n\r", INTERVAL_TIMER_TIMER_0, intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_0));
    printf("timer:(%d) duration:%f\n\r", INTERVAL_TIMER_TIMER_1, intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_1));
    printf("timer:(%d) duration:%f\n\r", INTERVAL_TIMER_TIMER_2, intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_2));
  }
  // Now, test for increment timing (start-stop-start-stop...)
  // Reset all the timers.
  intervalTimer_resetAll();
  for (int i=0; i<TEST_ITERATION_COUNT; i++) {
    // Start all the timers.
    intervalTimer_start(INTERVAL_TIMER_TIMER_0);
    intervalTimer_start(INTERVAL_TIMER_TIMER_1);
    intervalTimer_start(INTERVAL_TIMER_TIMER_2);
    // Delay is based on the loop count.
    utils_msDelay((i+1)*ONE_SECOND_DELAY);
    // Stop all of the timers.
    intervalTimer_stop(INTERVAL_TIMER_TIMER_0);
    intervalTimer_stop(INTERVAL_TIMER_TIMER_1);
    intervalTimer_stop(INTERVAL_TIMER_TIMER_2);
    // Print the duration of all of the timers. The delays should be approximately 1, 3, 6, and 10 seconds.
    printf("Delays should approximately be: 1, 3, 6, 10 seconds keeping in mind that you times may be lower due to compiler-optimization settings.\n\r");
    printf("timer:(%d) duration:%f\n\r", INTERVAL_TIMER_TIMER_0, intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_0));
    printf("timer:(%d) duration:%f\n\r", INTERVAL_TIMER_TIMER_1, intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_1));
    printf("timer:(%d) duration:%f\n\r", INTERVAL_TIMER_TIMER_2, intervalTimer_getTotalDurationInSeconds(INTERVAL_TIMER_TIMER_2));
  }

  printf("intervalTimer Test Complete.\n\r");
}

// main executes both milestones.
int main() {
  //milestone1();  // Execute milestone 1
  milestone2();  // Execute milestone 2


  return 0;
}

void isr_function(){}


