/*
 * spi.c
 *
 *  Created on: Apr 29, 2014
 *      Author: hutch
 */

#include <stdio.h>
#include "spi.h"
#include "arduinoTypes.h"
#include "xil_io.h"
#include "utils.h"

void spi_begin(void) {
  spi_softwareReset();  // Just reset the SPI hardware in the ZYNQ fabric.
}

void spi_end(void) {
}

// The SPI controller allows the user to specify the bit-order.
// Default after reset is MSB first.
// This function preserves other bits in the SPI control registers.
// The bit-order is specified using #defines from the spi.h file: SPI_LSBFIRST or SPI_MSBFIRST.
void spi_setBitOrder(uint8_t bitOrder) {
  uint32_t regValue;
  switch(bitOrder) {
  case SPI_LSBFIRST:
    regValue = spi_readRegister(SPI_CNTRL_REG_OFFSET);
    spi_writeRegister(SPI_CNTRL_REG_OFFSET, regValue | SPI_CNTRL_REG_LSB_FIRST_MASK);
    break;
  case SPI_MSBFIRST:
	regValue = spi_readRegister(SPI_CNTRL_REG_OFFSET);
	spi_writeRegister(SPI_CNTRL_REG_OFFSET, regValue & ~SPI_CNTRL_REG_LSB_FIRST_MASK);
    break;
  }
}

// Does nothing but is provided for compatability with the adafruit touch-controller code.
void spi_setClockDivider(uint8_t divider) {
  // Does nothing for this core. Divider is set in hardware when hardware is configured and built.
}

// Immediately updates the SPI control register.
// http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
// Also see the Xilinx SPI document (see above).
void spi_setTransmissionMode(uint8_t mode) {
  switch (mode) {
  case SPI_MODE_0:
	// CPOL = 0, CPHA = 0.
	spi_clearControlRegisterBits(SPI_CNTRL_CPOL_MASK);
	spi_clearControlRegisterBits(SPI_CNTRL_CPHA_MASK);
   break;
  case SPI_MODE_1:
	// CPOL = 0, CPHA = 1.
	spi_clearControlRegisterBits(SPI_CNTRL_CPOL_MASK);
	spi_setControlRegisterBits(SPI_CNTRL_CPHA_MASK);
    break;
  case SPI_MODE_2:
	// CPOL = 1, CPHA = 0.
	spi_setControlRegisterBits(SPI_CNTRL_CPOL_MASK);
	spi_clearControlRegisterBits(SPI_CNTRL_CPHA_MASK);
    break;
  case SPI_MODE_3:
	// CPOL = 1, CPHA = 1.
	spi_setControlRegisterBits(SPI_CNTRL_CPOL_MASK);
	spi_setControlRegisterBits(SPI_CNTRL_CPHA_MASK);
    break;
  }
}

// This actually does the bit transfer. It will block until the transfer is complete.
// It uses the transfer sequence (mostly) outlined in the Xilinx SPI documentation (see above).
uint8_t spi_transfer(uint8_t val) {
  // Assume that bit-order and the mode are configured elsewhere (spiread, spiwrite).
  // Clear all of the slave selects.
//  spi_clearAllSlaveSelects();  // BLH Comment out for now - 6/16/2014.
  // This sets:
  // (1) the master-transaction-inhibit bit to 1 = master transaction inhibited.
  // (2) manual-slave-select-assertion-enable to 1 = slave select register is asserted at pin when configured
  //     as master and the device is enabled (SPI-system-enable = 1).
  // (3) sets the Master-mode bit = 1.
  // (4) sets the SPI-system-enable bit = 1 (enables the system, makes outputs valid.

  spi_setControlRegisterBits(SPI_CNTRL_REG_MASTER_TRANSACTION_INHIBIT_MASK     |
		                     SPI_CNTROL_REG_MANUAL_SLAVE_ASSERTION_ENABLE_MASK |
		                     SPI_CNTRL_MASTER_MASK                             |
		                     SPI_CNTRL_SPE_MASK);

	// Copy the value to be transmitted to the transmit register.

   spi_writeRegister(SPI_DATA_TRANSMIT_REG_OFFSET, val);

  // Assert the slave select.
  //  spi_setTftSlaveSelect(); // digitalWrite(_cs, LOW);  // Comment out for now. 6/16/2014.
  // Clear the inhibit bit. This starts the transfer.

   spi_clearControlRegisterBits(SPI_CNTRL_REG_MASTER_TRANSACTION_INHIBIT_MASK);

  //  printf("Waiting until the SPI TX register is empty.\n\r");
  // Wait here until the transmit register is empty.
//  spi_waitUntilTxRegisterIsEmpty();
  //  printf("SPI TX register is empty.\n\r");
  // Read any values that were transmitted from the TFT controller.
  //  if (!spi_isReceiveFifoFull()) {
  //	printf("nothing in the RX FIFO.\n\r");
  //  }
//  utils_msDelay(1);
  // Blocking call to wait until the RX FIFO is not empty (waiting for the transfer to complete).
  spi_waitUntilRxFifoIsNotEmpty();

  uint32_t readValue = spi_readRegister(SPI_DATA_RECEIVE_REG_OFFSET);
  // Inhibit master operation.
  spi_setControlRegisterBits(SPI_CNTRL_REG_MASTER_TRANSACTION_INHIBIT_MASK);
  //  spi_clearAllSlaveSelects();  // Comment out for now. 6/16/2014.
  return readValue;
}

// Read the current SPI control-register value and OR the bits of the mask in.
void spi_setControlRegisterBits(uint32_t mask) {
  uint32_t regValue = spi_readRegister(SPI_CNTRL_REG_OFFSET);
  spi_writeRegister(SPI_CNTRL_REG_OFFSET, regValue | mask);
}

// Read the current SPI control-register value and clear the specified bits.
void spi_clearControlRegisterBits(uint32_t mask) {
  uint32_t regValue = spi_readRegister(SPI_CNTRL_REG_OFFSET);
  uint32_t invertedMask = ~mask;
  spi_writeRegister(SPI_CNTRL_REG_OFFSET, regValue & invertedMask);
}

// Directly write the specified SPI register (offsets are defined in spi.h).
void spi_writeRegister(uint32_t regOffset, uint32_t value) {
  Xil_Out32(SPI_CORE_BASE_ADDRESS + regOffset, value);
}

// Directly read the specified SPI register (offset are defined in spi.h).
uint32_t spi_readRegister(uint32_t regOffset) {
  return Xil_In32(SPI_CORE_BASE_ADDRESS + regOffset);
}

// The slave-select bits determine which of slaves attached to the SPI controller are active.
// Should use only the specific slave-select functions shown below.
void spi_setSlaveSelect(uint32_t slaveMask) {
  spi_writeRegister(SPI_SLAVE_SELECT_REG_OFFSET, ~slaveMask);
}

// Selects the touch-screen controller slave.
void spi_setTouchScreenControllerSlaveSelect() {
  spi_setSlaveSelect(SPI_TOUCH_SCREEN_CONTROLLER_SLAVE_SELECT_MASK);
}

// Clears out the slave select register so that no SPI slave is selected.
void spi_clearAllSlaveSelects() {
  spi_writeRegister(SPI_SLAVE_SELECT_REG_OFFSET, 0xFFFFFFFF);
}

// Uses for-loop for delay. Delay will be delay * milliseconds, approximately.
// These delays are generally used by the adafruit code.
void spi_delay(uint32_t delay) {
  volatile uint32_t count;
  count = delay * SPI_DELAY_FUDGE_FACTOR;
  while (count--);
}

// Software reset of the SPI controller. Needs to be invoked prior to programming the SPI controller
// for consistent operation.
void spi_softwareReset() {
  spi_writeRegister(SPI_RESET_REG_OFFSET, SPI_RESET_REG_MASK);
}

// Blocking call to wait until the TX register is empty (the transmission has completed).
void spi_waitUntilTxRegisterIsEmpty() {
  while (!(spi_readRegister(SPI_STATUS_REG_OFFET) & SPI_STATUS_REG_TX_EMPTY_MASK));
}

// Blocking call to wait until the RX FIFO is not empty (waiting for the transfer to complete).
void spi_waitUntilRxFifoIsNotEmpty() {
  while (spi_readRegister(SPI_STATUS_REG_OFFET) & SPI_STATUS_REG_RX_EMPTY_MASK);
}

// Check to see if the SPI receive FIFO is full.
bool spi_isReceiveFifoFull() {
  return (spi_readRegister(SPI_STATUS_REG_OFFET) & SPI_STATUS_REG_RX_FULL_MASK);
}

void spi_setBluetoothRadioSlaveSelect() {
  spi_setSlaveSelect(SPI_BLUETOOTH_RADIO_SLAVE_SELECT_MASK);
}

// Block does nothing for now. NYI.
uint32_t spi_beginTransaction(uint8_t mode, uint8_t bitOrder, bool block) {
  spi_setTransmissionMode(mode);
  spi_setBitOrder(bitOrder);
  return 0;
}

void spi_endTransaction() {}





