/*
 * circularBuffer.c
 *
 *  Created on: Dec 17, 2014
 *      Author: hutch
 */

#include "circularBuffer.h"
#include <stdlib.h>
#include <stdio.h>

// Init's the buffer to the empty state, allocating fresh memory.
void circularBuffer_init(circularBuffer_t* cb) {
	circularBuffer_reset(cb);
	cb->data = (uint32_t *) malloc((CIRCULAR_BUFFER_INDEX_MASK + 1) * sizeof(uint32_t));
}

// Just resets the index pointers to start at the zero position, and resets the overflow flag.
// The buffer becomes empty after reset().
// Buffer must be reset prior to writing anytime it has been read.
void circularBuffer_reset(circularBuffer_t* cb) {
	cb->writeIndex = cb->readIndex = 0;
	cb->wrapAroundFlag = false;
	cb->firstPassFlag = true;
}

// Adds a new data item to the buffer. Addressing scheme
// assumes that the initial data was added starting at index = 0.
// writeIndex always points to the location to be written and then is incremented.
void circularBuffer_addData(circularBuffer_t* cb, uint32_t datum) {
	cb->data[cb->writeIndex] = datum;		// Write the data to the buffer.
	if (cb->writeIndex == 0 && !cb->firstPassFlag)	// Check for wrap-around.
		cb->wrapAroundFlag = true;
	// Check for wrap-around and compute addresses and update the firstPassFlag.
	cb->writeIndex = (cb->writeIndex + 1) & CIRCULAR_BUFFER_INDEX_MASK;
	if (cb->writeIndex == 0) {  // You know that you are starting the second pass
		cb->firstPassFlag = false;		// but don't know if you will wrap-around until next write.
	}
}

// Returns the number of items in the buffer.
// User must call this to determine how many elements are in the buffer.
uint32_t circularBuffer_size(circularBuffer_t* cb) {
	if (cb->wrapAroundFlag)
		return CIRCULAR_BUFFER_INDEX_MASK + 1;
	else
		return (cb->writeIndex - cb->readIndex + 1);
}

// Reads a data item at a given index. The index is relative, an index of 0 just means the first
// element (oldest) of the circular buffer. Index is always masked so it will always be in range.
// Does not check the index to see if it is accessing stored data (for speed). User is responsible
// to access data in range.
uint32_t circularBuffer_readDataAt(circularBuffer_t* cb, uint32_t index) {
	if (cb->wrapAroundFlag) {
		printf("address->%ld\r\n", (cb->writeIndex + 1 + index) & CIRCULAR_BUFFER_INDEX_MASK);
		return cb->data[(cb->writeIndex + 1 + index) & CIRCULAR_BUFFER_INDEX_MASK];
	} else {
		return cb->data[index & CIRCULAR_BUFFER_INDEX_MASK];
	}
}





