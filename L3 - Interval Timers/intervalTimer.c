/*
 * intervalTimer.c
 *
 *  Created on: Sep 23, 2017
 *      Author: joanmaga
 */

#include "intervalTimer.h"
#include "xil_io.h"
#include "supportFiles/utils.h"
#include <stdio.h>

#define INIT_COUNTER 0x0    //Zero the Counter Registers
#define SET_CASC 0x800  //Code to Set the Casc Bit
#define SET_LOAD 0x20   //Code to set the Load bit
#define SET_ENT0 0x80   //Code to set the Ent0 bit

#define TCSR0 0x00  //Offset of TCSR0 Register
#define TCSR1 0x10  //Offset of TCSR1 Register
#define TLR0 0x04   //Offset of TLR0 Register
#define TLR1 0x14   //Offset of TLR1 Register
#define TCR0 0x08   //Offset of TCR0 Register
#define TCR1 0x18   //Offset of TCR1 Register
#define DELAY 2
#define SHIFT 32



uint32_t timer_base(uint32_t timerNumber) //This Function is a convenience function that returns the timer base based on the TImer Number
{
    uint32_t base = 0;  //Initialize the Base value
    if (timerNumber == INTERVAL_TIMER_TIMER_0)  //Timer 0 is the Parameter
        base = XPAR_AXI_TIMER_0_BASEADDR;   //Set the Base to Timer 0 Base
    else if (timerNumber == INTERVAL_TIMER_TIMER_1) //Timer 1 is the Parameter
        base = XPAR_AXI_TIMER_1_BASEADDR;   //Set the Base to Timer 1 Base
    else if (timerNumber == INTERVAL_TIMER_TIMER_2)  //Timer 2 is the Parameter
        base = XPAR_AXI_TIMER_2_BASEADDR;   //Set the Base to Timer 2 Base
    return base;    //Return Base Value
}

uint32_t timer_readRegister(uint32_t base, int32_t offset) //Read a Register, the Offset
{
    return Xil_In32(base  + offset);
}

void timer_writeRegister(uint32_t base, int32_t offset, int32_t value) //Write to a Register, and pass which value I want to write to it
{
        Xil_Out32(base  + offset, value);
}

// You must initialize the timers before you use them the first time.
// It is generally only called once but should not cause an error if it
// is called multiple times.
// timerNumber indicates which timer should be initialized.
// returns INTERVAL_TIMER_STATUS_OK if successful, some other value otherwise.
intervalTimer_status_t intervalTimer_init(uint32_t timerNumber)
{
        uint32_t base = timer_base(timerNumber);    //Get base from timer_base Function
        timer_writeRegister(base, TCSR0, INIT_COUNTER); //Pass a 0 to the TCSR0 Register
        timer_writeRegister(base, TCSR1, INIT_COUNTER); //Pass a 0 to the TCSR1 Register

        timer_writeRegister(base, TCSR0, SET_CASC); //Set the CASC Bit in the TCSR0

        if(timer_readRegister(base, TCSR0) & SET_CASC) {
            //The Timer was initalized correctly
            return INTERVAL_TIMER_STATUS_OK;
        } else {
            return INTERVAL_TIMER_STATUS_FAIL;   //The Timer was not initialized correctly
        }
}

// This is a convenience function that initializes all interval timers.
// Simply calls intervalTimer_init() on all timers.
// returns INTERVAL_TIMER_STATUS_OK if successful, some other value otherwise.
intervalTimer_status_t intervalTimer_initAll()
{
    intervalTimer_init(INTERVAL_TIMER_TIMER_0); //Initialize Timer 0
    intervalTimer_init(INTERVAL_TIMER_TIMER_1); //Initialize Timer 1
    intervalTimer_init(INTERVAL_TIMER_TIMER_2); //Initialize Timer 2
    if(intervalTimer_init(INTERVAL_TIMER_TIMER_0) && intervalTimer_init(INTERVAL_TIMER_TIMER_1)) //Check if Values are being passed correctly
    {
        return INTERVAL_TIMER_STATUS_OK ;     //The Timer was initalized correctly
    }
    else
    {
       return INTERVAL_TIMER_STATUS_FAIL;   //The Timer was not initialized correctly
    }
}

// This function starts the interval timer running.
// If the interval timer is already running, this function does nothing.
// timerNumber indicates which timer should start running.
void intervalTimer_start(uint32_t timerNumber)
{
    uint32_t base = timer_base(timerNumber);    //Get base from timer_base Function
    uint32_t ent0_on = timer_readRegister(base, TCSR0) & SET_ENT0;  //Variable to check if base is on
    if(!ent0_on)    //If Base is not on, then turn it on
    {
        timer_writeRegister(base, TCSR0 , timer_readRegister(base, TCSR0) | SET_ENT0);  //Write a Masked value the TCSR0 Register in case the ENT0 bit is already set
    }
}

// This function stops a running interval timer.
// If the interval time is currently stopped, this function does nothing.
// timerNumber indicates which timer should stop running.
void intervalTimer_stop(uint32_t timerNumber)
{
    uint32_t base = timer_base(timerNumber);    //Get base from timer_base Function
    uint32_t ent0_off = timer_readRegister(base, TCSR0) & SET_ENT0; //Variable to check if base is off
    if(!ent0_off)   //If Base is not off, then turn it off
    {
        timer_writeRegister(base, TCSR0 , (timer_readRegister(base, TCSR0) | SET_ENT0) | SET_CASC);
    }
}

// This function is called whenever you want to reuse an interval timer.
// For example, say the interval timer has been used in the past, the user
// will call intervalTimer_reset() prior to calling intervalTimer_start().
// timerNumber indicates which timer should reset.
void intervalTimer_reset(uint32_t timerNumber)
{
    uint32_t base = timer_base(timerNumber);    //Get base from timer_base Function
    timer_writeRegister(base, TLR0 , 0);    //Pass a 0 to the TLR0 Register
    timer_writeRegister(base, TCSR0 , timer_readRegister(base, TCSR0) | SET_LOAD);  //Allow Value in Load Register to be passed to the TCR0 Counter


    timer_writeRegister(base, TLR1 , 0);    //Pass a 0 to the TLR1 Register
    timer_writeRegister(base, TCSR1 , timer_readRegister(base, TCSR1) | SET_LOAD);  //Allow Value in Load Register to be passed to the TCR1 Counter

    intervalTimer_init(timerNumber);    //Reinitialize the Timer after being reset
}

// Convenience function for intervalTimer_reset().
// Simply calls intervalTimer_reset() on all timers.
void intervalTimer_resetAll()
{
    intervalTimer_reset(INTERVAL_TIMER_TIMER_0);    //Reset Timer 0
    intervalTimer_reset(INTERVAL_TIMER_TIMER_1);    //Reset Timer 1
    intervalTimer_reset(INTERVAL_TIMER_TIMER_2);    //Reset Timer 2
}

// Runs a test on a single timer as indicated by the timerNumber argument.
// Returns INTERVAL_TIMER_STATUS_OK if successful, something else otherwise.
intervalTimer_status_t intervalTimer_test(uint32_t timerNumber)
{
    uint32_t base = timer_base(timerNumber);    //Get base from timer_base Function
    intervalTimer_reset(timerNumber);
    uint32_t readTcsr0 = timer_readRegister(base, TCR0);    //Read Value of Counter Register 0
    uint32_t readTcsr1 = timer_readRegister(base, TCR1);    //Read Value of Counter Register 1
    uint32_t resetWork = (readTcsr0 & readTcsr1) == 0;      //Check if both values are cleared


    intervalTimer_start(timerNumber);
    uint32_t startTcsr0First = timer_readRegister(base, TCR0);  //Read Value of Counter Register 0
    utils_msDelay(DELAY);   //Delay to check if value is changing
    uint32_t startTcsr0Second = timer_readRegister(base, TCR0); //Read Value of Counter Register 0
    uint32_t startWork = startTcsr0First != startTcsr0Second;   //If 1, it's working


    intervalTimer_stop(timerNumber);
    uint32_t stopTcsr0First = timer_readRegister(base, TCR1);   //Read Value of Counter Register 1
    uint32_t stopTcsr0Second = timer_readRegister(base, TCR1);  //Read Value of Counter Register 1
    uint32_t stopWork = stopTcsr0First == stopTcsr0Second;  //If 1, it's working

    if(resetWork && startWork && stopWork)  //If True, every function is working
    {
       return INTERVAL_TIMER_STATUS_OK;     //The Timer was initalized correctly
    }
    else
    {
       return INTERVAL_TIMER_STATUS_FAIL;   //The Timer was not initialized correctly
    }

}


// Convenience function that invokes test on all interval timers.
// Returns INTERVAL_TIMER_STATUS_OK if successful, something else otherwise.
intervalTimer_status_t intervalTimer_testAll()
{
    if(intervalTimer_test(INTERVAL_TIMER_TIMER_0) &&
            intervalTimer_test(INTERVAL_TIMER_TIMER_1) &&
            intervalTimer_test(INTERVAL_TIMER_TIMER_2))
    {
        return INTERVAL_TIMER_STATUS_OK ;     //The Timer was initalized correctly
    }
    else
    {
        return INTERVAL_TIMER_STATUS_FAIL;   //The Timer was not initialized correctly
    }
}

// Use this function to ascertain how long a given timer has been running.
// Note that it should not be an error to call this function on a running timer
// though it usually makes more sense to call this after intervalTimer_stop() has
// been called.
// The timerNumber argument determines which timer is read.
double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber)
{
    uint32_t base = timer_base(timerNumber);    //Get base from timer_base Function
    double doubleTcrRegs = ((((uint64_t) timer_readRegister(base, TCR1)) << SHIFT)+((uint64_t) timer_readRegister(base, TCR0)));    //COmbine Values of both Registers and combine into one 64 bit Variable

    double seconds = doubleTcrRegs/XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;  //Devide Number by Clock Frequency to get value in Seconds
    return seconds; //Return the Value in Seconds
}


