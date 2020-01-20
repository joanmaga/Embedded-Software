/*
 * bluetooth.c
 *
 *  Created on: Aug 6, 2015
 *      Author: hutch
 */

#include "supportFiles/bluetooth.h"
#include <Xuartlite.h>
#include <xparameters.h>
#include <stdio.h>

static XUartLite bluetooth_uartInstance;        // Handle to the bluetooth UART.
static XUartLite_Config bluetooth_uartConfig;   // Handle to the bluetooth UART config.

// This implements a dedicated buffer for storing values from the ADC
// until they are read and processed by detector().
// adcBuffer_t is similar to a queue.
#define BLUETOOTH_QUEUE_SIZE 1000
#define BLUETOOTH_UART_FIFO_SIZE 16
typedef struct {
    uint16_t indexIn;   // New values go here.
    uint16_t indexOut;  // Pull old values from here.
    uint8_t data[BLUETOOTH_QUEUE_SIZE];  // Store values here.
    uint16_t elementCount;  // Number of values contained in adcBuffer_t.
    uint16_t size;          // queue can hold this many elements.
} bluetooth_queue_t;

static bluetooth_queue_t bluetooth_receiveQueue;   // characters read from the bluetooth UART go here.
static bluetooth_queue_t bluetooth_transmitQueue;  // characters that need to be transmitted to the bluetooth UART go here.

// Init the q.
void bluetooth_queueInit(bluetooth_queue_t* q, uint16_t size) {
    q->indexIn = 0;
    q->indexOut = 0;
    q->elementCount = 0;
    q->size = size;
}

// Implemented as a fixed-size circular buffer.
// indexIn always points to an empty location (by definition).
// indexOut always points to the oldest element.
// buffer is empty if indexIn == indexOut. Buffer is full if incremented indexIn == indexOut
void bluetooth_queuePush(bluetooth_queue_t* q, uint8_t data) {
    if (q->elementCount < q->size)  // Increment the element count unless you are already full.
        q->elementCount++;
    q->data[q->indexIn] = data;                    // write,
    q->indexIn = (q->indexIn + 1) % q->size;  // then increment.
    if (q->indexIn == q->indexOut) {                  // If you are now pointing at the out pointer,
        q->indexOut = (q->indexOut + 1) % q->size;  // move the out pointer up.
    }
}

// Removes a single item from the ADC buffer.
// Does not signal an error if the ADC buffer is currently
// empty. Simply returns a default value of 0 if the buffer is currently empty.
uint8_t bluetooth_queuePop(bluetooth_queue_t* q) {
    uint8_t returnValue = 0;
    if (q->indexIn == q->indexOut)  // Just return 0 if empty.
        return 0;
    else {
        returnValue = q->data[q->indexOut];  // Not empty, get the return value from buffer.
        q->indexOut = (q->indexOut + 1) % q->size;  // increment the out index.
        q->elementCount--;  // One less element.
    }
    return returnValue;
}

// Provides read access to all elements in the ADC buffer.
// No error messages are provided. In cases of error, a 0 is returned.
// index = 0 indexes the oldest data in the queue.
uint32_t bluetooth_queueReadElementAt(bluetooth_queue_t* q, uint32_t index) {
  // Just return 0 if there if the buffer is empty or not enough elements for the index.
  if ((q->indexIn == q->indexOut) || (index + 1 > q->elementCount))
    return 0;
  // Otherwise, there are enough elements, just do modulo addressing.
  return q->data[(q->indexOut + index) % q->size];
}


// Functional interface to access element count.
uint16_t bluetooth_queueElementCount(bluetooth_queue_t* q) {
    return q->elementCount;
}

// Check if the queue is empty.
bool bluetooth_queueEmpty(bluetooth_queue_t* q) {
    return (q->indexIn == q->indexOut);
}

// Check if the queue is full.
bool bluetooth_queueFull(bluetooth_queue_t* q) {
    return !(q->elementCount < q->size);
}

uint16_t bluetooth_queueSize(bluetooth_queue_t* q) {
    return q->size;
}

// Used to initialize any bluetooth data structures.
// Must be called before accessing any of the bluetooth_ routines.
int bluetooth_init() {
    bluetooth_queueInit(&bluetooth_receiveQueue, BLUETOOTH_QUEUE_SIZE);  // init the receive q.
    bluetooth_queueInit(&bluetooth_transmitQueue, BLUETOOTH_QUEUE_SIZE); // init the transmit q.
    // Init the bluetooth UART.
    int status = XUartLite_CfgInitialize(&bluetooth_uartInstance, &bluetooth_uartConfig, XPAR_BLUETOOTH_UARTLITE_0_BASEADDR);
    if (status != XST_SUCCESS) {
        printf("bluetooth_init(): Unable to initialize bluetooth UART\n\r.");
        return BLUETOOTH_INIT_STATUS_FAIL;
    }
    return BLUETOOTH_INIT_STATUS_OK;
}

// Writes data to the bluetooth modem via the bluetooth_UART.
// Returns the number of characters written.
// Nonblocking.
// Can be data or commands. Switch on bluetooth modem board determines whether is interpreted as data or commands.
uint16_t bluetooth_uartWrite(uint8_t* data, uint16_t size) {
    return XUartLite_Send(&bluetooth_uartInstance, data, size);
}

// Reads data from the bluetooth modem via the bluetooth_UART.
// Returns the number of characters read.
// Nonblocking.
uint16_t bluetooth_uartRead(uint8_t* data, uint16_t maxSize) {
    return XUartLite_Recv(&bluetooth_uartInstance, data, maxSize);
}

// Reads characters from the bluetooth buffer. Characters are placed in the
// bluetooth_receiveQueue by reading the bluetooth UART and pushing them into the queue.
// Will only read upto maxSize characters. Returns the number of characters read.
uint16_t bluetooth_receiveQueueRead(uint8_t* data, uint16_t maxSize) {
    uint16_t bytesRead = 0;
    // Read the characters unless the receive queue empties.
    for (uint16_t i=0; i<maxSize && !bluetooth_queueEmpty(&bluetooth_receiveQueue); i++) {
        data[i] = bluetooth_queuePop(&bluetooth_receiveQueue);
        bytesRead++;
    }
    return bytesRead;   // Let the caller know how many bytes were read.
}

// Writes characters to the bluetooth transmit queue. The characters from the buffer need to be written
// from the queue to the bluetooth UART. Returns the number of characters written.
uint16_t bluetooth_transmitQueueWrite(uint8_t* data, uint16_t size) {
    uint16_t bytesWritten = 0;
    // Write the characters unless the transmit queue fills up.
    for (uint16_t i=0; i<size && !bluetooth_queueFull(&bluetooth_transmitQueue); i++) {
        bluetooth_queuePush(&bluetooth_transmitQueue, data[i]);
        bytesWritten++;
    }
    return bytesWritten;    // Let the caller know how many bytes were written.
}

// Polls the bluetooth for data.
// Received data from the bluetooth UART are placed in the receive queue.
// Data in the transmit queue are sent to the bluetooth UART.
// bluetooth UART only operates at 9600 BAUD, so don't call this more than about every 5 ms or so.
// Presumed that this will be called in a timer ISR.
void bluetooth_poll() {
    // Read characters from the bluetooth UART and copy to the receive queue.
    uint8_t readData[BLUETOOTH_UART_FIFO_SIZE];
    // How much room is in the receive queue?
    uint16_t receiveQueueSpace = bluetooth_queueSize(&bluetooth_receiveQueue) -
            bluetooth_queueElementCount(&bluetooth_receiveQueue);
    // Requested number of bytes will be either all the chars in the UART FIFO, or the available space in the recieve queue.
    uint16_t requestedReadCount = receiveQueueSpace > BLUETOOTH_UART_FIFO_SIZE ? BLUETOOTH_UART_FIFO_SIZE : receiveQueueSpace;
    // Transfer whatever you read to the receive queue.
    uint16_t bytesRead = bluetooth_uartRead(readData, requestedReadCount);
//    if (bytesRead != 0)
//        printf("received %d bytes.\n\r", bytesRead);
    for (uint16_t i=0; i<bytesRead; i++)  {
        bluetooth_queuePush(&bluetooth_receiveQueue, readData[i]);
    }
    // Read chars from the transmit queue and send them to the bluetooth UART.
    // Transmit characters one at a time so you won't have to put anything back into the transmit queue.
    bool transmitOk = true; // This will be set to false if unable to transmit a byte.
    uint16_t bytesToTransmit = bluetooth_queueElementCount(&bluetooth_transmitQueue);
    // Loop will write bytes to the bluetooth UART until the UART is full or all characters are transmitted.
    for (uint16_t i=0; i<bytesToTransmit && transmitOk; i++) {
        uint8_t transmitData[1];    // Only transmit one byte at a time.
        transmitData[0] = bluetooth_queueReadElementAt(&bluetooth_transmitQueue, i);    // Read the byte to be transmitted.
        uint8_t bytesWritten = bluetooth_uartWrite(transmitData, 1);    // Ask for it to be written.
        if (bytesWritten == 1) {    // Successful if one byte was written.
            transmitOk = true;
            bluetooth_queuePop(&bluetooth_transmitQueue);   // Successfully written so pop the data off the queue.
        } else {    // Not successful, so terminate.
            transmitOk = false;
        }
    }
}

// Starts an interactive loop that queries the user for input, transmits that input to the bluetooth UART
// and then prints the result.
// Useful for configuring the bluetooth modem when in command mode.
// Terminates if the user types a single "." on a line of input.
void bluetooth_interactiveLoop() {
}

