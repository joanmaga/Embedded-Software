/*
 * spi.h
 *
 *  Created on: Apr 29, 2014
 *      Author: hutch
 */

// Provides an API to use the AXI SPI controller provided by Xilinx. I am accessing the SPI
// controller directly and am not using any Xilinx driver code. I wanted to have better
// control over the SPI hardware. All of the register offsets, etc., are derived from the
// Xilinx SPI controller documentation: LogiCORE IP AXI Serial Peripheral Interface (AXI SPI) (v1.02.a),
// AKA, axi_spi_ds742.pdf

#ifndef SPI_H_
#define SPI_H_

#include "arduinoTypes.h"

// All of the types below come directly from the SPI documentation provided by Xilinx.

#define SPI_MSBFIRST 0
#define SPI_LSBFIRST 1
#define SPI_MODE_0 0
#define SPI_MODE_1 1
#define SPI_MODE_2 2
#define SPI_MODE_3 3

#define SPI_CORE_BASE_ADDRESS XPAR_SPI_0_BASEADDR  // from xparameters.h

#define SPI_RESET_REG_OFFSET 0x40
#define SPI_RESET_REG_MASK 0x0000000A  // Write this value to force a software reset of the SPI core.

#define SPI_CNTRL_REG_OFFSET 0x60
#define SPI_CNTRL_REG_LSB_FIRST_MASK                       0x00000200
#define SPI_CNTRL_REG_MASTER_TRANSACTION_INHIBIT_MASK      0x00000100
#define SPI_CNTROL_REG_MANUAL_SLAVE_ASSERTION_ENABLE_MASK  0x00000080
#define SPI_CNTRL_REG_RX_FIFO_RESET_MASK                   0x00000040
#define SPI_CNTRL_REG_TX_FIFO_RESET_MASK                   0x00000020
#define SPI_CNTRL_CPHA_MASK                                0x00000010
#define SPI_CNTRL_CPOL_MASK                                0x00000008
#define SPI_CNTRL_MASTER_MASK                              0x00000004
#define SPI_CNTRL_SPE_MASK                                 0x00000002
#define SPI_CNTRL_LOOP_MASK                                0x00000001

#define SPI_STATUS_REG_OFFET 0x64
#define SPI_STATUS_REG_SLAVE_MODE_SELECT_MASK 0x00000020
#define SPI_STATUS_REG_MODF_MASK              0x00000010
#define SPI_STATUS_REG_TX_FULL_MASK           0x00000008
#define SPI_STATUS_REG_TX_EMPTY_MASK          0x00000004
#define SPI_STATUS_REG_RX_FULL_MASK           0x00000002
#define SPI_STATUS_REG_RX_EMPTY_MASK          0x00000001

#define SPI_DATA_TRANSMIT_REG_OFFSET 0x68
#define SPI_DATA_RECEIVE_REG_OFFSET 0x6C
#define SPI_SLAVE_SELECT_REG_OFFSET 0x70
#define SPI_TRANSMIT_FIFO_OCC_REG_OFFSET 0x74
#define SPI_RECEIVE_FIFO_OCC_REG_OFFSET 0x78

#define SPI_DELAY_FUDGE_FACTOR 10000  // This is the multiplier to get the delay value of 1 to be 1 millisecond.

#define SPI_TFT_SLAVE_SELECT_MASK 0x00000001  // TFT SPI slave select is bit 0 (only used if LCD is accessed via SPI - deprecated).
#define SPI_TOUCH_SCREEN_CONTROLLER_SLAVE_SELECT_MASK 0x00000002 // Touch-screen controller slave select is bit 1.
#define SPI_BLUETOOTH_RADIO_SLAVE_SELECT_MASK 0x00000004 // Bluetooth-radio slave-select is bit 3.

// The following named routines provide functionality necessary for the adafruit touch-controller code.
// Some of the functions are empty but are provided to minimize modifications to the adafruit code.
// See spi.c for functionality and comments.
void spi_begin(void); // init routine, used begin to mirror the adafruit source code.
uint32_t spi_beginTransaction(uint8_t mode, uint8_t bitOrder, bool block);
void spi_endTransaction();

void spi_end(void);
void spi_setBitOrder(uint8_t order);
void spi_setClockDivider(uint8_t divider);
void spi_setTransmissionMode(uint8_t mode);
uint8_t spi_transfer(uint8_t val);
uint32_t spi_readRegister(uint32_t regOffset);
void spi_writeRegister(uint32_t regOffset, uint32_t value);
void spi_setControlRegisterBits(uint32_t mask);
void spi_clearControlRegisterBits(uint32_t mask);
uint32_t spi_readControlRegister();
uint32_t spi_writeControlRegister(uint32_t value);
void spi_delay(uint32_t delay);
void spi_setTouchScreenControllerSlaveSelect();
void spi_setBluetoothRadioSlaveSelect();
void spi_clearAllSlaveSelects();
void spi_softwareReset();
void spi_configure();
void spi_waitUntilTxRegisterIsEmpty();
// Blocking call to wait until the RX FIFO is not empty (waiting for the transfer to complete).
void spi_waitUntilRxFifoIsNotEmpty();

bool spi_isReceiveFifoFull();

#endif /* SPI_H_ */
