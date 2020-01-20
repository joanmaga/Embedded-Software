/*
 * bluetooth.h
 *
 *  Created on: Aug 6, 2015
 *      Author: hutch
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include <stdbool.h>

#define BLUETOOTH_INIT_STATUS_FAIL 0
#define BLUETOOTH_INIT_STATUS_OK 1

// Used to initialize any bluetooth data structures.
int bluetooth_init();

// Reads characters from the bluetooth buffer. Characters are placed in the
// bluetooth_receiveQueue by reading the bluetooth UART and pushing them into the queue.
// Will only read upto maxSize characters. Returns the number of characters read.
uint16_t bluetooth_receiveQueueRead(uint8_t* data, uint16_t maxSize);

// Writes characters to the bluetooth transmit queue. The characters from the buffer need to be written
// from the queue to the bluetooth UART. Returns the number of characters written.
uint16_t bluetooth_transmitQueueWrite(uint8_t* data, uint16_t size);

// Starts an interactive loop that queries the user for input, transmits that input to the bluetooth UART
// and then prints the result.
// Useful for configuring the bluetooth modem when in command mode.
// Terminates if the user types a single "." on a line of input.
void bluetooth_interactiveLoop();

// Polls the bluetooth for data.
// Received data from the bluetooth UART are placed in the receive queue.
// Data in the transmit queue are sent to the bluetooth UART.
// bluetooth UART only operates at 9600 BAUD, so don't call this more than about every 5 ms or so.
// Presumed that this will be called in a timer ISR.
void bluetooth_poll();

#endif /* BLUETOOTH_H_ */
