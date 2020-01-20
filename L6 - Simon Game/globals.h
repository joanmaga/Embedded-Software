#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <stdint.h>

#define GLOBALS_MAX_FLASH_SEQUENCE 1000                  // Make it big so you can use it for a splash screen.

// This is the length of the complete sequence at maximum length.
// You must copy the contents of the sequence[] array into the global variable that you maintain.
// Do not just grab the pointer as this will fail.
void globals_setSequence(const uint8_t sequence[], uint16_t length);

// This returns the value of the sequence at the index.
uint8_t globals_getSequenceValue(uint16_t index);

// Retrieve the sequence length.
uint16_t globals_getSequenceLength();

// This is the length of the sequence that you are currently working on.
void globals_setSequenceIterationLength(uint16_t length);

// This is the length of the sequence that you are currently working on,
// not the maximum length but the interim length as
// the use works through the pattern one color at a time.
uint16_t globals_getSequenceIterationLength();

#endif /* GLOBALS_H_ */
