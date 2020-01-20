/*
 * interrupts.h
 *
 *  Created on: Mar 24, 2014
 *      Author: hutch
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stdbool.h>
#include <stdint.h>
#include "xil_types.h"
//#include "../src/laserTag/queue.h"

#define INTERRUPT_CUMULATIVE_ISR_INTERVAL_TIMER_NUMBER 0

// Assumes that you are connected to auxiliary port 14: pins JA1 (P) and JA7 (N) on the ZYBO board.
#define XADC_AUX_CHANNEL_14 XSM_CH_AUX_MAX-1

// Uses interval timer 0 to measure time spent in ISR.
//#define ENABLE_INTERVAL_TIMER_0_IN_TIMER_ISR 1

//queue_data_t interrupts_popAdcQueueData();
//bool interrupts_adcQueueEmpty();
//queue_size_t interrupts_adcQueueElementCount();


// Inits all interrupts, which means:
// 1. Sets up the interrupt routine for ARM (GIC ISR) and does all necessary initialization.
// 2. Initializes all supported interrupts and connects their ISRs to the GIC ISR.
// 3. Enables the interrupts at the GIC, but not at the device itself.
// 4. Pretty much does everything but it does not enable the ARM interrupts or any of the device global interrupts.
// if printFailedStatusFlag is true, it prints out diagnostic messages if something goes awry.
int interrupts_initAll(bool printFailedStatusFlag);

// Used to enable and disable ARM ints.
int interrupts_enableArmInts();
int interrupts_disableArmInts();

// Useeed to enable and disable the global timer int output.
int interrupts_enableTimerGlobalInts();
int interrupts_disableTimerGlobalInts();

int interrupts_startArmPrivateTimer();
int interrupts_stopArmPrivateTimer();

u32 interrupts_getPrivateTimerCounterValue(void);
void interrupts_setPrivateTimerLoadValue(u32 loadValue);
void interrupts_setPrivateTimerPrescalerValue(u32 prescalerValue);

// Globally enable/disable SysMon interrupts.
int interrupts_enableSysMonGlobalInts();
int interrupts_disableSysMonGlobalInts();

// Enable End-Of-Conversion interrupts. You can use this to count how often an ADC conversion occurs.
int interrupts_enableSysMonEocInts();
int interrupts_disableSysMonEocInts();

// Keep track of total number of times interrupt_timerIsr is invoked.
u32 interrupts_isrInvocationCount();

// Returns the number of private timer ticks that occur in 1 second.
u32 interrupts_getPrivateTimerTicksPerSecond();

// Use this to read the latest ADC conversion.
uint32_t interrupts_getAdcData();

//u32 interrupts_getTotalXadcSampleCount();
u32 interrupts_getTotalEocCount();
void isr_function();

// Init/Enable/disable interrupts for the bluetooth radio (RDYN line).
uint32_t interrupts_initBluetoothInterrupts();
void interrupts_enableBluetoothInterrupts();
void interrupts_disableBluetoothInterrupts();
void interrupts_ackBluetoothInterrupts();

extern volatile int interrupts_isrFlagGlobal;

#endif /* INTERRUPTS_H_ */
