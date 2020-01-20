



#include <stdio.h>
#include "switches.h"
#include "buttons.h"
#include "supportFiles/leds.h"
#include "xil_io.h"

int main() {
    switches_runTest(); //Run the code necessary to turn on and off the switches
    buttons_runTest();  //Run the code necessary to turn on and off the buttons
}

// This function must be defined but can be left empty for now.
// You will use this function in a later lab.
// It is called in the timer interrupt service routine (see interrupts.c in supportFiles).
void isr_function() {
    // Empty for now.
}
