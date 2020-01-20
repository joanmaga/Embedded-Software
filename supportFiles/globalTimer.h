/*
 * globalTimer.h
 *
 *  Created on: Mar 27, 2014
 *      Author: hutch
 */

#ifndef GLOBALTIMER_H_
#define GLOBALTIMER_H_
#include <stdbool.h>
#include "xparameters.h"
#include "xil_types.h"

// These defines are used to compute seconds, minutes, etc., from the global timer values.
// The global timer clock is 1/2 the processor frequency (650 MHz for ZYBO board
#define GLOBAL_TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
// one second equals GLOBAL_TIMER_CLOCK_FREQUENCY ticks (by definition).
#define GLOBAL_TIMER_TICKS_PER_SECOND GLOBAL_TIMER_CLOCK_FREQUENCY

#define globalTimer_readRegister(registerOffset) \
 Xil_In32(XPAR_GLOBAL_TMR_BASEADDR + registerOffset)

#define globalTimer_writeRegister(registerOffset, registerValue) \
  Xil_Out32((XPAR_GLOBAL_TMR_BASEADDR + registerOffset), (registerValue))

// These functions are available to the user.

// Returns the current value for the 64-bit ARM global timer.
u64 globalTimer_getTimerValue(void);

// Starts the timer counter.
void globalTimer_startTimer(bool printStatusFlag);

// Stops the timer counter.
void globalTimer_stopTimer(bool printStatusFlag);

// Simple test so that user can verify that the global timer is working properly.
u32 globalTimer_test(bool printStatusFlag);

#endif /* GLOBALTIMER_H_ */
