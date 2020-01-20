/*
 * globalTimer.c
 *
 *  Created on: Mar 27, 2014
 *      Author: hutch
 */
// Convenience functions for accessing the global timer in the ZYNQ ARM core.

#include <stdio.h>
#include "xil_io.h"  // Used for low-level input and output.
#include "globalTimer.h"

// Define register offsets below from ARM manual
// cortex_19_mpcore_r4p1_trm, Page 4-9, Table 4-4, Global Timer Registers.
// Counter is 64-bits but all accesses must be 32-bit. 64-bit values
// are accessed using two 32-bit accesses.
// These definitions are intentionally kept private here.
#define GLOBAL_TIMER_LOWER_COUNTER_REGISTER 0x0
#define GLOBAL_TIMER_UPPER_COUNTER_REGISTER 0x4
#define GLOBAL_TIMER_CONTROL_REGISTER 0x8
#define GLOBAL_TIMER_INTERRUPT_STATUS_REGISTER 0xC
#define GLOBAL_TIMER_COMPARATOR_LOWER_REGISTER 0x10
#define GLOBAL_TIMER_COMPARATOR_UPPER_REGISTER 0x14
#define GLOBAL_TIMER_AUTO_INCREMENT_REGISTER 0x18

#define GLOBAL_TIMER_TIMER_ENABLE_BIT_POSITION 0
#define GLOBAL_TIMER_COMPARATOR_ENABLE_BIT_POSITION 1
#define GLOBAL_TIMER_IRQ_ENABLE_BIT_POSITION 3
#define GLOBAL_TIMER_AUTO_INCREMENT_BIT_POSITION 4

//u32 globalTimer_readRegister(u32 registerOffset) {
//  u32 registerValue;
////  registerValue = Xil_In32(XPAR_GLOBAL_TMR_BASEADDR + registerOffset);
//  return registerValue;
//}

//void globalTimer_writeRegister(u32 registerOffset, u32 value) {
//  Xil_Out32((XPAR_GLOBAL_TMR_BASEADDR + registerOffset), value);
//}

// Returns the 64-bit timer value from the 64-bit global timer.
u64 globalTimer_getTimerValue(void) {
  u32 upperTimerCounterRegister = globalTimer_readRegister(GLOBAL_TIMER_UPPER_COUNTER_REGISTER);
//  printf("globalTimer_getTimerValue (upper): %lx\n\r", upperTimerCounterRegister);
  u32 lowerTimerCounterRegister = globalTimer_readRegister(GLOBAL_TIMER_LOWER_COUNTER_REGISTER);
//  printf("globalTimer_getTimerValue (lower): %lx\n\r", lowerTimerCounterRegister);
  // Read the upper register again to check for roll-over. If the upper register has changed,
  // reread the lower value to keep things coherent.
  u32 upperTimerCounterRegister2 = globalTimer_readRegister(GLOBAL_TIMER_UPPER_COUNTER_REGISTER);
  if (upperTimerCounterRegister2 != upperTimerCounterRegister) {
	lowerTimerCounterRegister = globalTimer_readRegister(GLOBAL_TIMER_LOWER_COUNTER_REGISTER);
	upperTimerCounterRegister = upperTimerCounterRegister2;
  }
  // Combine the two 32-bit values into a single 64-bit value.
  u64 timerValue = upperTimerCounterRegister;  // Assign the upper 32 bits into lower 32 bits.
  timerValue <<= 32;                           // Move them up to the upper 32 bits of 64.
  timerValue |= lowerTimerCounterRegister;     // OR in the lower 32 bits.
////  printf("globalTimer_getTimervalue (combined): %llx\n\r", timerValue);
  return timerValue;                           // That should be it.
}

// Used to set individual control register bits.
void globalTimer_setControlRegisterBit(u8 bitNumber) {
  // Read the control register first so you can OR specific bits and leave others untouched.
  u32 timerControlRegister = globalTimer_readRegister(GLOBAL_TIMER_CONTROL_REGISTER);
  timerControlRegister |= (0x1 << bitNumber);
  globalTimer_writeRegister(GLOBAL_TIMER_CONTROL_REGISTER, timerControlRegister);
}

// Used to clear individual control register bits.
void globalTimer_clearControlRegisterBit(u8 bitNumber) {
  // Read the control register first so you can OR specific bits and leave others untouched.
  u32 timerControlRegister = globalTimer_readRegister(GLOBAL_TIMER_CONTROL_REGISTER);
  timerControlRegister &= ~(0x1 << bitNumber);// Clear bit by shifting bit, invert and AND.
  globalTimer_writeRegister(GLOBAL_TIMER_CONTROL_REGISTER, timerControlRegister);
}

// Returns the requested bit from the global timer control register.
u32 globalTimer_readControlRegisterBit(u8 bitNumber){
  // Read the control register first so you can OR specific bits and leave others untouched.
  u32 timerControlRegister = globalTimer_readRegister(GLOBAL_TIMER_CONTROL_REGISTER);
  return (timerControlRegister >> bitNumber) & 0x1;
}

void globalTimer_startTimer(bool printStatusFlag) {
  if (globalTimer_readControlRegisterBit(GLOBAL_TIMER_TIMER_ENABLE_BIT_POSITION) && printStatusFlag) {
    printf("globalTimer_startTimer: timer is currently running.\n\r");
  }
  globalTimer_setControlRegisterBit(GLOBAL_TIMER_TIMER_ENABLE_BIT_POSITION);
}

void globalTimer_stopTimer(bool printStatusFlag) {
  if (!globalTimer_readControlRegisterBit(GLOBAL_TIMER_TIMER_ENABLE_BIT_POSITION) && printStatusFlag) {
	printf("globalTimer_startTimer: timer is already stopped.\n\r");
  }
  globalTimer_clearControlRegisterBit(GLOBAL_TIMER_TIMER_ENABLE_BIT_POSITION);
}

// Returns 0 if no problem.
u32 globalTimer_test(bool printStatusFlag) {
  u32 error=0;  // Bee optimistic.
  // Keep track of whether the global timer was running when you started the test so you can restore it.
  u32 timerRunningFlag = globalTimer_readControlRegisterBit(GLOBAL_TIMER_TIMER_ENABLE_BIT_POSITION);
  globalTimer_startTimer(false);  // false prevents the printing of a status message.
  // Now, do a couple of reads and verify that the counter is indeed running.
  unsigned long timerValue1 = globalTimer_getTimerValue();
  unsigned long timerValue2 = globalTimer_getTimerValue();
  if (timerValue1 == timerValue2) {
	if (printStatusFlag)
      printf("globalTimer_test: Error, global timer does not seem to be running.\n\r");
    error = 1;
  }
  // Now, try stopping the timer and read two consecutive values to see if the counter is really stopped.
  globalTimer_stopTimer(false);
  timerValue1 = globalTimer_getTimerValue();
  timerValue2 = globalTimer_getTimerValue();
  if (timerValue1 != timerValue2) {
	if (printStatusFlag)
      printf("globalTimer_test: Error, global timer did not stop running.\n\r");
	error = 1;
  }
  // Now, restore the global timer enable bit.
  if (timerRunningFlag) {
    globalTimer_startTimer(false);  // Was already running, just start it back up.
  } else {
    globalTimer_stopTimer(false);   // Was stopped, so make sure it is stopped.
  }
  if (!error) {
	if (printStatusFlag)
      printf("globalTimer_test: test completed without error.\n\r");
  }
  return error;
}

//// A duration timer consists of a start time and and end time. The duration is the difference.
//typedef struct {
//  bool timerStarted;
//  bool timerRunning;  // Duration timer was "started" and hasn't been stopped.
//  u64 startTime; // global timer was read when it was started.
//  u64 stopTime;  // global timer was read when it was stopped.
//} globalTimer_duration_t;

//// This array is local to this file.
//static globalTimer_duration_t durationTimers[GLOBAL_TIMER_DURATION_TIMER_COUNT];
//
//// This package provides up to GLOBAL_TIMER_DURATION_TIMER_COUNT duration timers.
//// Start the duration timer.
//u32 globalTimer_startDurationTimer(u8 timerNumber) {
//  globalTimer_startTimer(false);  // Start the timer if not already running. false = no status message.
//  durationTimers[timerNumber].timerRunning = true;
//  durationTimers[timerNumber].timerStarted = true;
//  durationTimers[timerNumber].startTime = globalTimer_getTimerValue();
//  return 0;
//}
//
//// Stop the duration timer.
//u32 globalTimer_stopDurationTimer(u8 timerNumber) {
//  u32 error = 0;
//  if (!durationTimers[timerNumber].timerRunning) {
//    printf("globalTimer_stopDurationTimer(): Error - duration timer %d is not running.\n\r", timerNumber);
//    error = true;
//  } else {
//    durationTimers[timerNumber].timerRunning = false;                    // Note that the timer is no longer running.
//    durationTimers[timerNumber].stopTime = globalTimer_getTimerValue();  // Get the timer end time.
//  }
//  return error;
//}
//
//// Get the duration of the timer. The timer must be stopped to use this function.
//u16 globalTimer_getDurationTimerValue(u8 timerNumber, float *seconds) {
//  u16 error = 0;
//  if (durationTimers[timerNumber].timerRunning) {
//    printf("globalTimer_getDurationTimerValue(): duration timer %d is still running.\n\r", timerNumber);
//    error = 1;
//  } else if (!durationTimers[timerNumber].timerStarted){
//    printf("globalTimer_getDurationTimerValue(): duration timer %d was never started.\n\r", timerNumber);
//	error = 1;
//  } else {
//	printf("start value: %llx\n\r", durationTimers[timerNumber].startTime);
//	printf("stop value: %llx\n\r", durationTimers[timerNumber].stopTime);
//    *seconds = (float) (durationTimers[timerNumber].stopTime - durationTimers[timerNumber].startTime)
//    		/ (float) GLOBAL_TIMER_TICKS_PER_SECOND;
//  }
//  return error;
//}
//
//
//
//
//

