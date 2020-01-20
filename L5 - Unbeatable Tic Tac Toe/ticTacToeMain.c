/*
 * ticTacToeMain.c
 *
 *  Created on: Oct 19, 2017
 *      Author: joanmaga
 */

#include "minimax.h"
#include "ticTacToeDisplay.h"
#include "ticTacToeControl.h"

#include "supportFiles/display.h"
#include "supportFiles/utils.h"

/***********************************
************ Flag Method ***********

************************************/
#include <stdio.h>
#include "supportFiles/leds.h"
#include "supportFiles/globalTimer.h"
#include "supportFiles/interrupts.h"
#include <stdbool.h>
#include <stdint.h>
#include "supportFiles/display.h"

#include "xparameters.h"

#define R0 0 //used to call row 0 in our board
#define R1 1 //used to call row 1 in our board
#define R2 2 //used to call row 2 in our board
#define C0 0 //used to call column 0 in our board
#define C1 1 //used to call column 1 in our board
#define C2 2 //used to call column 2 in our board

#define TOTAL_SECONDS 60
// The formula for computing the load value is based upon the formula from 4.1.1 (calculating timer intervals)
// in the Cortex-A9 MPCore Technical Reference Manual 4-2.
// Assuming that the prescaler = 0, the formula for computing the load value based upon the desired period is:
// load-value = (period * timer-clock) - 1
#define TIMER_PERIOD 50.0E-3 // You can change this value to a value that you select.
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)

static uint32_t isr_functionCallCount = 0;

int main() {

    minimax_board_t board6; //Declares new Board  Number 6
    board6.squares[R0][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board6.squares[R0][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board6.squares[R0][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board6.squares[R1][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board6.squares[R1][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board6.squares[R1][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board6.squares[R2][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board6.squares[R2][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board6.squares[R2][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square

    minimax_board_t board7; //Declares new Board  Number 7
    board7.squares[R0][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board7.squares[R0][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board7.squares[R0][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board7.squares[R1][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board7.squares[R1][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board7.squares[R1][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board7.squares[R2][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board7.squares[R2][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board7.squares[R2][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X

    minimax_board_t board8; //Declares new Board  Number 8
    board8.squares[R0][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board8.squares[R0][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board8.squares[R0][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board8.squares[R1][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board8.squares[R1][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board8.squares[R1][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board8.squares[R2][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board8.squares[R2][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board8.squares[R2][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O

    minimax_board_t board9; //Declares new Board  Number 9
    board9.squares[R0][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board9.squares[R0][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board9.squares[R0][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board9.squares[R1][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board9.squares[R1][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board9.squares[R1][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board9.squares[R2][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board9.squares[R2][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board9.squares[R2][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X

    minimax_board_t board10;     //Declares new Board  Number 10
    board10.squares[R0][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board10.squares[R0][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board10.squares[R0][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board10.squares[R1][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board10.squares[R1][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board10.squares[R1][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board10.squares[R2][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board10.squares[R2][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board10.squares[R2][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X

    minimax_board_t board11; //Declares new Board  Number 11
    board11.squares[R0][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board11.squares[R0][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board11.squares[R0][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board11.squares[R1][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board11.squares[R1][C1] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board11.squares[R1][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board11.squares[R2][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board11.squares[R2][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board11.squares[R2][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square

    minimax_board_t board12; //Declares new Board  Number 12
    board12.squares[R0][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board12.squares[R0][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board12.squares[R0][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board12.squares[R1][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board12.squares[R1][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board12.squares[R1][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board12.squares[R2][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board12.squares[R2][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board12.squares[R2][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O

    minimax_board_t board13; //Declares new Board  Number 13
    board13.squares[R0][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board13.squares[R0][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board13.squares[R0][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board13.squares[R1][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board13.squares[R1][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board13.squares[R1][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board13.squares[R2][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board13.squares[R2][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board13.squares[R2][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O

    minimax_board_t board14; //Declares new Board  Number 14
    board14.squares[R0][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board14.squares[R0][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board14.squares[R0][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board14.squares[R1][C0] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board14.squares[R1][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board14.squares[R1][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board14.squares[R2][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board14.squares[R2][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board14.squares[R2][C2] = MINIMAX_X_SQUARE;  //Set this position to be an X

    minimax_board_t board15; //Declares new Board  Number 15
    board15.squares[R0][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board15.squares[R0][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board15.squares[R0][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board15.squares[R1][C0] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board15.squares[R1][C1] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board15.squares[R1][C2] = MINIMAX_O_SQUARE;  //Set this position to be an O
    board15.squares[R2][C0] = MINIMAX_X_SQUARE;  //Set this position to be an X
    board15.squares[R2][C1] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square
    board15.squares[R2][C2] = MINIMAX_EMPTY_SQUARE;  //Set this position to be an empty Square

    uint8_t row, column;    //Declare Row and Column that will be passed to compute next move Function to be modified inside that Function


    minimax_computeNextMove(&board6, true, &row, &column);     // false means O is current player.
    printf("next move for board6: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 6
    minimax_computeNextMove(&board7, false, &row, &column);     // false means O is current player.
    printf("next move for board7: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 7
    minimax_computeNextMove(&board8, false, &row, &column);     // false means O is current player.
    printf("next move for board8: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 8
    minimax_computeNextMove(&board9, true, &row, &column);     // false means O is current player.
    printf("next move for board9: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 9
    minimax_computeNextMove(&board10, true, &row, &column);     // false means O is current player.
    printf("next move for board10: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 10
    minimax_computeNextMove(&board11, false, &row, &column);     // false means O is current player.
    printf("next move for board11: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 11
    minimax_computeNextMove(&board12, false, &row, &column);     // false means O is current player.
    printf("next move for board12: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 12
    minimax_computeNextMove(&board13, true, &row, &column);     // false means O is current player.
    printf("next move for board13: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 13
    minimax_computeNextMove(&board14, true, &row, &column);     // false means O is current player.
    printf("next move for board14: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 14
    minimax_computeNextMove(&board15, false, &row, &column);     // false means O is current player.
    printf("next move for board15: (%d, %d)\n\r", row, column);  //Prints the Next move for Board 15

    return 0;
}

// The clockControl_tick() function is now called directly by the timer interrupt service routine.
void isr_function()
{

}


