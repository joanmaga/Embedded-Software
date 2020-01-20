/*
 * TFTGPIO.h
 *
 *  Created on: Apr 30, 2014
 *      Author: hutch
 */

#ifndef TFTGPIO_H_
#define TFTGPIO_H_

#define TFTGPIO_DEVICE_ID XPAR_AXI_GPIO_TFT_CONTROL_DEVICE_ID
#define TFTGPIO_BIT_WIDTH 2
#define TFTGPIO_DCX_BIT_MASK 0x00000001
#define TFTGPIO_RD_BIT_MASK  0x00000002
#define TFTGPIO_WR_BIT_MASK  0x00000004

void TFTGPIO_init();
void TFTGPIO_setCommandMode();
void TFTGPIO_setDataMode();
void TFTGPIO_assertRd();
void TFTGPIO_negateRd();
void TFTGPIO_assertWr();
void TFTGPIO_negateWr();

#endif /* TFTGPIO_H_ */
