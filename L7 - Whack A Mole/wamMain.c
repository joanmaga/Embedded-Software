#include "wamDisplay.h"
#include "wamControl.h"
#include "supportFiles/utils.h"
#include "supportFiles/display.h"
#include "src/Lab3/intervalTimer.h"  // Modify as necessary to point to your intervalTimer.h
#include "supportFiles/leds.h"
#include "supportFiles/interrupts.h"
#include "src/Lab2/switches.h"  // Modify as necessary to point to your switches.h
#include "src/Lab2/buttons.h"   // Modify as necessary to point to your buttons.h
#include <stdio.h>
#include <xparameters.h>

// The formula for computing the load value is based upon the formula from 4.1.1 (calculating timer intervals)
// in the Cortex-A9 MPCore Technical Reference Manual 4-2.
// Assuming that the prescaler = 0, the formula for computing the load value based upon the desired period is:
// load-value = (period * timer-clock) - 1
#define TIMER_PERIOD 50.0E-3    // 50 ms.
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)

#define MAX_ACTIVE_MOLES 1  // Start out with this many moles.
#define MAX_MISSES 50       // Game is over when there are this many misses.
#define FOREVER 1           // Syntactic sugar for while (1) statements.
#define MS_PER_TICK (TIMER_PERIOD * 1000)   // Just multiply the timer period by 1000 to get ms.

#define SWITCH_VALUE_9 9  // Binary 9 on the switches indicates 9 moles.
#define SWITCH_VALUE_6 6  // Binary 6 on the switches indicates 6 moles.
#define SWITCH_VALUE_4 4  // Binary 9 on the switches indicates 4 moles.
#define SWITCH_MASK 0xf   // Ignore potentially extraneous bits.

// Mole count is selected by setting the slide switches. The binary value for the switches
// determines the mole count (1001 - nine moles, 0110 - 6 moles, 0100 - 4 moles).
// All other switch values should default to 9 moles).
void wamMain_selectMoleCountFromSwitches(uint16_t switchValue) {
    switch(switchValue & SWITCH_MASK) {
    case SWITCH_VALUE_9:    // this switch pattern = 9 moles.
        wamDisplay_selectMoleCount(wamDisplay_moleCount_9);
        break;
    case SWITCH_VALUE_6:    // this switch pattern = 6 moles.
        wamDisplay_selectMoleCount(wamDisplay_moleCount_6);
        break;
    case SWITCH_VALUE_4:   // this switch pattern = 4 moles.
        wamDisplay_selectMoleCount(wamDisplay_moleCount_4);
        break;
    default:    // Any other pattern of switches = 9 moles.
        wamDisplay_selectMoleCount(wamDisplay_moleCount_9);
        break;
    }
}

// Top-level control loop is implemented with while-loops in main().
int main() {
    /************************* System Initialization Code ***********************/
    switches_init();  // Init the slide switches.
    buttons_init();   // Init the push buttons.
    leds_init(true);  // You need to init the LEDs so that LD4 can function as a heartbeat.
    // Init all interrupts (but does not enable the interrupts at the devices).
    // Prints an error message if an internal failure occurs because the argument = true.
    interrupts_initAll(true);   // Init the interrupt code.
    interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);  // Set the timer period.
    interrupts_enableTimerGlobalInts();  // Enable interrupts at the timer.
    interrupts_startArmPrivateTimer();   // Start the private ARM timer running.
    uint32_t personalInterruptCount = 0; // Used to compare with internal count from interrupt code.
    /******************** Game-Specific Code ********************/
    uint32_t randomSeed;    // Used to make the game seem more random.
    display_init();         // Init the display (make sure to do it only once).
    wamControl_setMaxActiveMoles(MAX_ACTIVE_MOLES); // Start out with this many simultaneous active moles.
    wamControl_setMaxMissCount(MAX_MISSES);         // Allow this many misses before ending game.
    wamControl_setMsPerTick(MS_PER_TICK);           // Let the controller know how ms per tick..
    wamDisplay_drawSplashScreen();  // Draw the game splash screen.
    while (FOREVER) {               // Endless loop.
        wamMain_selectMoleCountFromSwitches(switches_read());  // Mole count selected via slide switches.
        wamDisplay_init();              // Initialize the WAM display.
        wamControl_init();              // Initialize the WAM controller.
        while (!display_isTouched()) {  // Wait for the user to touch the screen.
            randomSeed++;               // Increment a random seed while you wait.
        }
        while (display_isTouched());            // Now wait for the user to remove their finger.
        wamControl_setRandomSeed(randomSeed);   // Set the random-seed.
        wamDisplay_drawMoleBoard();             // Draw the WAM mole board.
        interrupts_enableArmInts();             // Enable interrupts at the ARM.
        while (!wamControl_isGameOver() && !buttons_read()) {// Game runs until over or interrupted.
            if (interrupts_isrFlagGlobal) {     // If an interrupt occurs, time to call tick.
                interrupts_isrFlagGlobal = 0;   // Reset the interrupt flag.
                personalInterruptCount++;       // Count interrupts.
                wamControl_tick();              // tick the WAM controller.
            }
        }
        interrupts_disableArmInts();            // Game is over, turn off interrupts.
        // Print out the interrupt counts to ensure that you didn't miss any interrupts.
        printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());
        printf("internal interrupt count: %ld\n\r", personalInterruptCount);
        wamDisplay_drawGameOverScreen();        // Draw the game-over screen.
        while (!display_isTouched());           // Wait here until the user touches the screen to try again.
        wamDisplay_resetAllScoresAndLevel();    // Reset all game statistics so you can start over.
    }
}

void isr_function() {
    // Empty for now.
}
