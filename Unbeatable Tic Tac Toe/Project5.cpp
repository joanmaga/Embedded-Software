#include <SDKDDKVer.h>
#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stdio.h>
#include "minimax.h"
#include <stdbool.h>

int main() {
	minimax_board_t board1;  // Board 1 is the main example in the web-tutorial that I use on the web-site.
	board1.squares[0][0] = MINIMAX_O_SQUARE;
	board1.squares[0][1] = MINIMAX_EMPTY_SQUARE;
	board1.squares[0][2] = MINIMAX_X_SQUARE;
	board1.squares[1][0] = MINIMAX_X_SQUARE;
	board1.squares[1][1] = MINIMAX_EMPTY_SQUARE;
	board1.squares[1][2] = MINIMAX_EMPTY_SQUARE;
	board1.squares[2][0] = MINIMAX_X_SQUARE;
	board1.squares[2][1] = MINIMAX_O_SQUARE;
	board1.squares[2][2] = MINIMAX_O_SQUARE;

	minimax_board_t board2;
	board2.squares[0][0] = MINIMAX_O_SQUARE;
	board2.squares[0][1] = MINIMAX_EMPTY_SQUARE;
	board2.squares[0][2] = MINIMAX_X_SQUARE;
	board2.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board2.squares[1][1] = MINIMAX_EMPTY_SQUARE;
	board2.squares[1][2] = MINIMAX_EMPTY_SQUARE;
	board2.squares[2][0] = MINIMAX_X_SQUARE;
	board2.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board2.squares[2][2] = MINIMAX_O_SQUARE;

	minimax_board_t board3;
	board3.squares[0][0] = MINIMAX_O_SQUARE;
	board3.squares[0][1] = MINIMAX_EMPTY_SQUARE;
	board3.squares[0][2] = MINIMAX_EMPTY_SQUARE;
	board3.squares[1][0] = MINIMAX_O_SQUARE;
	board3.squares[1][1] = MINIMAX_EMPTY_SQUARE;
	board3.squares[1][2] = MINIMAX_EMPTY_SQUARE;
	board3.squares[2][0] = MINIMAX_X_SQUARE;
	board3.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board3.squares[2][2] = MINIMAX_X_SQUARE;

	minimax_board_t board4;
	board4.squares[0][0] = MINIMAX_O_SQUARE;
	board4.squares[0][1] = MINIMAX_EMPTY_SQUARE;
	board4.squares[0][2] = MINIMAX_EMPTY_SQUARE;
	board4.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board4.squares[1][1] = MINIMAX_EMPTY_SQUARE;
	board4.squares[1][2] = MINIMAX_EMPTY_SQUARE;
	board4.squares[2][0] = MINIMAX_X_SQUARE;
	board4.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board4.squares[2][2] = MINIMAX_X_SQUARE;

	minimax_board_t board5;
	board5.squares[0][0] = MINIMAX_X_SQUARE;
	board5.squares[0][1] = MINIMAX_X_SQUARE;
	board5.squares[0][2] = MINIMAX_EMPTY_SQUARE;
	board5.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board5.squares[1][1] = MINIMAX_O_SQUARE;
	board5.squares[1][2] = MINIMAX_EMPTY_SQUARE;
	board5.squares[2][0] = MINIMAX_EMPTY_SQUARE;
	board5.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board5.squares[2][2] = MINIMAX_EMPTY_SQUARE;

	minimax_board_t board6;
	board6.squares[0][0] = MINIMAX_EMPTY_SQUARE;
	board6.squares[0][1] = MINIMAX_EMPTY_SQUARE;
	board6.squares[0][2] = MINIMAX_X_SQUARE;
	board6.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board6.squares[1][1] = MINIMAX_EMPTY_SQUARE;
	board6.squares[1][2] = MINIMAX_X_SQUARE;
	board6.squares[2][0] = MINIMAX_O_SQUARE;
	board6.squares[2][1] = MINIMAX_O_SQUARE;
	board6.squares[2][2] = MINIMAX_EMPTY_SQUARE;

	minimax_board_t board7;
	board7.squares[0][0] = MINIMAX_EMPTY_SQUARE;
	board7.squares[0][1] = MINIMAX_X_SQUARE;
	board7.squares[0][2] = MINIMAX_X_SQUARE;
	board7.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board7.squares[1][1] = MINIMAX_O_SQUARE;
	board7.squares[1][2] = MINIMAX_O_SQUARE;
	board7.squares[2][0] = MINIMAX_O_SQUARE;
	board7.squares[2][1] = MINIMAX_X_SQUARE;
	board7.squares[2][2] = MINIMAX_X_SQUARE;

	minimax_board_t board8;
	board8.squares[0][0] = MINIMAX_X_SQUARE;
	board8.squares[0][1] = MINIMAX_O_SQUARE;
	board8.squares[0][2] = MINIMAX_EMPTY_SQUARE;
	board8.squares[1][0] = MINIMAX_O_SQUARE;
	board8.squares[1][1] = MINIMAX_X_SQUARE;
	board8.squares[1][2] = MINIMAX_X_SQUARE;
	board8.squares[2][0] = MINIMAX_EMPTY_SQUARE;
	board8.squares[2][1] = MINIMAX_O_SQUARE;
	board8.squares[2][2] = MINIMAX_O_SQUARE;

	minimax_board_t board9;
	board9.squares[0][0] = MINIMAX_O_SQUARE;
	board9.squares[0][1] = MINIMAX_O_SQUARE;
	board9.squares[0][2] = MINIMAX_X_SQUARE;
	board9.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board9.squares[1][1] = MINIMAX_O_SQUARE;
	board9.squares[1][2] = MINIMAX_EMPTY_SQUARE;
	board9.squares[2][0] = MINIMAX_O_SQUARE;
	board9.squares[2][1] = MINIMAX_X_SQUARE;
	board9.squares[2][2] = MINIMAX_X_SQUARE;

	minimax_board_t board10;
	board10.squares[0][0] = MINIMAX_X_SQUARE;
	board10.squares[0][1] = MINIMAX_X_SQUARE;
	board10.squares[0][2] = MINIMAX_O_SQUARE;
	board10.squares[1][0] = MINIMAX_X_SQUARE;
	board10.squares[1][1] = MINIMAX_O_SQUARE;
	board10.squares[1][2] = MINIMAX_O_SQUARE;
	board10.squares[2][0] = MINIMAX_EMPTY_SQUARE;
	board10.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board10.squares[2][2] = MINIMAX_X_SQUARE;

	minimax_board_t board11;
	board11.squares[0][0] = MINIMAX_X_SQUARE;
	board11.squares[0][1] = MINIMAX_X_SQUARE;
	board11.squares[0][2] = MINIMAX_EMPTY_SQUARE;
	board11.squares[1][0] = MINIMAX_O_SQUARE;
	board11.squares[1][1] = MINIMAX_O_SQUARE;
	board11.squares[1][2] = MINIMAX_EMPTY_SQUARE;
	board11.squares[2][0] = MINIMAX_X_SQUARE;
	board11.squares[2][1] = MINIMAX_X_SQUARE;
	board11.squares[2][2] = MINIMAX_EMPTY_SQUARE;

	minimax_board_t board12;
	board12.squares[0][0] = MINIMAX_X_SQUARE;
	board12.squares[0][1] = MINIMAX_EMPTY_SQUARE;
	board12.squares[0][2] = MINIMAX_EMPTY_SQUARE;
	board12.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board12.squares[1][1] = MINIMAX_X_SQUARE;
	board12.squares[1][2] = MINIMAX_O_SQUARE;
	board12.squares[2][0] = MINIMAX_EMPTY_SQUARE;
	board12.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board12.squares[2][2] = MINIMAX_O_SQUARE;

	minimax_board_t board13;
	board13.squares[0][0] = MINIMAX_EMPTY_SQUARE;
	board13.squares[0][1] = MINIMAX_EMPTY_SQUARE;
	board13.squares[0][2] = MINIMAX_EMPTY_SQUARE;
	board13.squares[1][0] = MINIMAX_X_SQUARE;
	board13.squares[1][1] = MINIMAX_EMPTY_SQUARE;
	board13.squares[1][2] = MINIMAX_X_SQUARE;
	board13.squares[2][0] = MINIMAX_O_SQUARE;
	board13.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board13.squares[2][2] = MINIMAX_O_SQUARE;

	minimax_board_t board14;
	board14.squares[0][0] = MINIMAX_O_SQUARE;
	board14.squares[0][1] = MINIMAX_X_SQUARE;
	board14.squares[0][2] = MINIMAX_O_SQUARE;
	board14.squares[1][0] = MINIMAX_EMPTY_SQUARE;
	board14.squares[1][1] = MINIMAX_X_SQUARE;
	board14.squares[1][2] = MINIMAX_O_SQUARE;
	board14.squares[2][0] = MINIMAX_O_SQUARE;
	board14.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board14.squares[2][2] = MINIMAX_X_SQUARE;

	minimax_board_t board15;
	board15.squares[0][0] = MINIMAX_O_SQUARE;
	board15.squares[0][1] = MINIMAX_X_SQUARE;
	board15.squares[0][2] = MINIMAX_O_SQUARE;
	board15.squares[1][0] = MINIMAX_O_SQUARE;
	board15.squares[1][1] = MINIMAX_X_SQUARE;
	board15.squares[1][2] = MINIMAX_O_SQUARE;
	board15.squares[2][0] = MINIMAX_X_SQUARE;
	board15.squares[2][1] = MINIMAX_EMPTY_SQUARE;
	board15.squares[2][2] = MINIMAX_EMPTY_SQUARE;

	uint8_t row, column;

	minimax_computeNextMove(&board1, true, &row, &column);      // true means X is current player.
	printf("next move for board1: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board2, true, &row, &column);      // true means X is current player.
	printf("next move for board2: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board3, true, &row, &column);      // true means X is current player.
	printf("next move for board3: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board4, false, &row, &column);     // false means O is current player.
	printf("next move for board4: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board5, false, &row, &column);     // false means O is current player.
	printf("next move for board5: (%d, %d)\n\r", row, column);

	minimax_computeNextMove(&board6, true, &row, &column);     // false means O is current player.
	printf("next move for board6: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board7, false, &row, &column);     // false means O is current player.
	printf("next move for board7: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board8, false, &row, &column);     // false means O is current player.
	printf("next move for board8: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board9, true, &row, &column);     // false means O is current player.
	printf("next move for board9: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board10, true, &row, &column);     // false means O is current player.
	printf("next move for board10: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board11, false, &row, &column);     // false means O is current player.
	printf("next move for board11: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board12, false, &row, &column);     // false means O is current player.
	printf("next move for board12: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board13, true, &row, &column);     // false means O is current player.
	printf("next move for board13: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board14, true, &row, &column);     // false means O is current player.
	printf("next move for board14: (%d, %d)\n\r", row, column);
	minimax_computeNextMove(&board15, false, &row, &column);     // false means O is current player.
	printf("next move for board15: (%d, %d)\n\r", row, column);





	//board5.squares[row][column] = MINIMAX_X_SQUARE;
	//minimax_computeNextMove(&board5, true, &row, &column);     // false means O is current player.
	//printf("next move for board5: (%d, %d)\n\r", row, column);
	//board5.squares[row][column] = MINIMAX_O_SQUARE;

	//int r, c;
	//printf("Show your next move:\n\r");
	//scanf("%d", &r);
	//scanf("%d", &c);
	//board5.squares[r][c] = MINIMAX_O_SQUARE;

	//minimax_computeNextMove(&board5, true, &row, &column);      // true means X is current player.
	//printf("next move for board5: (%d, %d)\n\r", row, column);
	//board5.squares[row][column] = MINIMAX_X_SQUARE;
	//print_Board(&board5);

	//printf("Show your next move:\n\r");
	//scanf("%d", &r);
	//scanf("%d", &c);
	//board5.squares[r][c] = MINIMAX_O_SQUARE;
	//print_Board(&board5);

	//minimax_computeNextMove(&board5, true, &row, &column);      // true means X is current player.
	//printf("next move for board5: (%d, %d)\n\r", row, column);
	//board5.squares[row][column] = MINIMAX_X_SQUARE;
	//print_Board(&board5);

	//printf("Show your next move:\n\r");
	//scanf("%d", &r);
	//scanf("%d", &c);
	//board5.squares[r][c] = MINIMAX_O_SQUARE;
	//print_Board(&board5);

	//minimax_computeNextMove(&board5, true, &row, &column);      // true means X is current player.
	//printf("next move for board5: (%d, %d)\n\r", row, column);
	//board5.squares[row][column] = MINIMAX_X_SQUARE;
	//print_Board(&board5);

	//printf("Show your next move:\n\r");
	//scanf("%d", &r);
	//scanf("%d", &c);
	//board5.squares[r][c] = MINIMAX_O_SQUARE;
	//print_Board(&board5);

	//minimax_computeNextMove(&board5, true, &row, &column);     // false means O is current player.
	//printf("next move for board5: (%d, %d)\n\r", row, column);
	//board5.squares[row][column] = MINIMAX_X_SQUARE;
	//print_Board(&board5);

	//printf("Show your next move:\n\r");
	//scanf("%d", &r);
	//scanf("%d", &c);
	//board5.squares[r][c] = MINIMAX_O_SQUARE;
	//print_Board(&board5);

	//minimax_computeNextMove(&board5, true, &row, &column);     // false means O is current player.
	//printf("next move for board5: (%d, %d)\n\r", row, column);
	//board5.squares[row][column] = MINIMAX_X_SQUARE;
	//print_Board(&board5);
	return 0;
}