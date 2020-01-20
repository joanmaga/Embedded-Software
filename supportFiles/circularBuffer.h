/*
 * circularBuffer.h
 *
 *  Created on: Dec 17, 2014
 *      Author: hutch
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <stdint.h>
#include <stdbool.h>

// Keep things integer powers of 2 so that address calculations are fast.
#define CIRCULAR_BUFFER_INDEX_MASK 0xFF

typedef struct {
	uint32_t writeIndex;	// You write new data here.
	uint32_t readIndex;		// You read data from here.
	bool wrapAroundFlag;	// True if you have rolled over.
	bool firstPassFlag;				// True if this is your first write pass through the array.
	uint32_t *data;				// Data are stored here.
} circularBuffer_t;

// Init's the buffer to the empty state, allocating fresh memory.
void circularBuffer_init(circularBuffer_t* cb);

// Just resets the index pointers to start at the zero position, and resets the overflow flag.
void circularBuffer_reset(circularBuffer_t* cb);

// Adds a new data item to the buffer.
void circularBuffer_addData(circularBuffer_t* cb, uint32_t datum);

// Reads a data item at the given index. The index is relative, an index of 0 just means the first
// element of the circular buffer.
uint32_t circularBuffer_readDataAt(circularBuffer_t* cb, uint32_t index);

// Returns the number of elements contained in the buffer.
uint32_t circularBuffer_size(circularBuffer_t* cb);

#endif /* CIRCULARBUFFER_H_ */
