/*
 * verifySequence.h
 *
 *  Created on: Nov 13, 2017
 *      Author: joanmaga
 */

#ifndef VERIFYSEQUENCE_H_
#define VERIFYSEQUENCE_H_

// State machine will run when enabled.
void verifySequence_enable();

// This is part of the interlock. You disable the state-machine and then enable it again.
void verifySequence_disable();

// Used to detect if there has been a time-out error.
bool verifySequence_isTimeOutError();

// Used to detect if the user tapped the incorrect sequence.
bool verifySequence_isUserInputError();

// Used to detect if the verifySequence state machine has finished verifying.
bool verifySequence_isComplete();

// Initialize the state machine
void verifySequence_init();

// Standard tick function.
void verifySequence_tick();

// Standard runTest function.
void verifySequence_runTest();

#endif /* VERIFYSEQUENCE_H_ */
