/*
 * flashSequence.h
 *
 *  Created on: Oct 28, 2014
 *      Author: hutch
 */

#ifndef FLASHSEQUENCE_H_
#define FLASHSEQUENCE_H_

// Turns on the state machine. Part of the interlock.
void flashSequence_enable();

// Turns off the state machine. Part of the interlock.
void flashSequence_disable();

// Other state machines can call this to determine if this state machine is finished.
bool flashSequence_isComplete();

// Initialize the state machine
void flashSequence_init();

// Standard tick function.
void flashSequence_tick();

// Tests the flashSequence state machine.
void flashSequence_runTest();

#endif /* FLASHSEQUENCE_H_ */
