#include "minimax.h"
#include <stdio.h>

#define R0 0 //used to call row 0 in our board 
#define R1 1 //used to call row 1 in our board
#define R2 2 //used to call row 2 in our board
#define C0 0 //used to call column 0 in our board
#define C1 1 //used to call column 1 in our board
#define C2 2 //used to call column 2 in our board
#define INIT_SC -2
#define MAX_TABLE 9
#define ROW_SIZE 3
#define COL_SIZE 3
#define INIT_LOW_SCORE -1
#define INIT_HIGH_SCORE -1

static minimax_move_t choice;

void print_Board(minimax_board_t* board)
{

	printf("%d %d %d\n\r", board->squares[R0][C0], board->squares[R0][C1], board->squares[R0][C2]);
	printf("%d %d %d\n\r", board->squares[R1][C0], board->squares[R1][C1], board->squares[R1][C2]);
	printf("%d %d %d\n\r", board->squares[R2][C0], board->squares[R2][C1], board->squares[R2][C2]);
}

// current_player_is_x == true means X, current_player_is_x == false means O
int minimax(minimax_board_t* board, bool current_player_is_x, int16_t depth) {
	//evaluates the current score of the game by checking the previous player's move (hence the "!" before current_player_is_x)

	int16_t score = minimax_computeBoardScore(board, !current_player_is_x);
	bool game_is_over = minimax_isGameOver(score);
	if (game_is_over) //if the game is over
		return score;//return the current score of the game
		// Otherwise, you need to recurse.
		// This loop will generate all possible boards.
	int16_t score_table[MAX_TABLE] = { INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC };
	minimax_move_t moves_table[MAX_TABLE];
	for (int8_t mm_row = 0; mm_row < COL_SIZE; mm_row++) {//magic//for all rows
		for (int8_t mm_col = 0; mm_col < ROW_SIZE; mm_col++) {//magic//for all columns
			if (board->squares[mm_row][mm_col] == MINIMAX_EMPTY_SQUARE) {
				// Assign the square to the player or opponent.
				if (current_player_is_x) {
					board->squares[mm_row][mm_col] = MINIMAX_X_SQUARE;
				}
				else{
					board->squares[mm_row][mm_col] = MINIMAX_O_SQUARE;
				}
				/*print_Board(board);
				printf("Depth: %d\n", depth);
				printf("\n");
				printf("\n");*/
				
				score = minimax(board, !current_player_is_x, depth + 1);
				/*if (depth == 0) {
					printf("thisisatest\n\r");
				}*/
				//this is one possible method, may be able to find a better one
				for (uint16_t table_index  = 0; table_index < MAX_TABLE; table_index++){//magic
					if(score_table[table_index] == INIT_SC){//magic
						/*if ((depth <= 1) && !current_player_is_x && mm_row == 2 && mm_col == 1)
							printf("TEST");*/
						score_table[table_index] = score;
						moves_table[table_index].row = mm_row;
						moves_table[table_index].column = mm_col;
						/*if (depth == 0) {
							printf("thisisatest\n\r");
						}*/
						//doesn't mattwer what it says, just use it at as a breakpoint
						//table_index = 9;//magic
						break;
					}
				}
				// Undo the change to the board (return the square to empty) prior to next iteration of for-loops.
				board->squares[mm_row][mm_col] = MINIMAX_EMPTY_SQUARE;
			}
		}
	}
	if (current_player_is_x) {
		/*printf("Current Player: %d, Depth: %d\n", current_player_is_x, depth);*/
		int16_t cur_high_score = INIT_HIGH_SCORE;
		int16_t best_index = 0;
		for (uint16_t table_index = 0; table_index < MAX_TABLE; table_index++){//magic
			if(score_table[table_index] > cur_high_score){//magic
				best_index = table_index;
				choice.row = moves_table[table_index].row;
				choice.column = moves_table[table_index].column;
				cur_high_score = score_table[table_index];
				score = score_table[table_index];
				/*printf("Current Player: %d, score: %d\n", current_player_is_x, score);
				printf("   Move:(%d,%d) Depth: %d \n", choice.row, choice.column, depth);*/
				//table_index = 9;
				//break;
				if (score_table[table_index] > MINIMAX_DRAW_SCORE)
					return score;
			}
		}
		choice.row = moves_table[best_index].row;
		choice.column = moves_table[best_index].column;
		return score_table[best_index];
	}
	else {
		/*printf("Current Player: %d, Depth: %d\n", current_player_is_x, depth);*/
		int16_t cur_low_score = INIT_LOW_SCORE;
		int16_t best_index = 0;
		for (uint16_t table_index = 0; table_index < MAX_TABLE; table_index++){//magic
			if((score_table[table_index] < cur_low_score) && (score_table[table_index] != INIT_SC)){//magic
				best_index = table_index;
				choice.row = moves_table[table_index].row;
				choice.column = moves_table[table_index].column;
				cur_low_score = score_table[table_index];
				score = score_table[table_index];
				/*printf("Current Player: %d, score: %d\n", current_player_is_x, score);
				printf("   Move:(%d,%d) Depth: %d \n", choice.row, choice.column, depth);*/

				//table_index = 9;
				//break;
				if (score_table[table_index] < MINIMAX_DRAW_SCORE)
					return score;
			}
		}
		choice.row = moves_table[best_index].row;
		choice.column = moves_table[best_index].column;
		return score_table[best_index];
	}
	/*printf("Score: %d, Depth: %d\n", score, depth);*/
	return score;
}

// This routine is not recursive but will invoke the recursive minimax function. 
// You will call this function from the controlling state machine that you will implement in a later milestone.
// It computes the row and column of the next move based upon:
// the current board,
// the player. true means the computer is X. false means the computer is O.
// When called from the controlling state machine, you will call this function as follows:
// 1. If the computer is playing as X, you will call this function with current_player_is_x = true.
// 2. If the computer is playing as O, you will call this function with current_player_is_x = false.
// minimax_computeNextMove directly passes the current_player_is_x argument into the minimax() function.
void minimax_computeNextMove(minimax_board_t* board, bool current_player_is_x, uint8_t* row, uint8_t* column){
	uint16_t score = minimax(board, current_player_is_x, 0);
    *row = choice.row;
	*column = choice.column;
	print_Board(board);
}

// Determine that the game is over by looking at the score.
bool minimax_isGameOver(minimax_score_t score){
	if (score == MINIMAX_NOT_ENDGAME){
		return false;
	}
	else 
		return true;
}

// Returns the score of the board, based upon the player and the board.
// This returns one of 4 values: MINIMAX_X_WINNING_SCORE, 
// MINIMAX_O_WINNING_SCORE, MINIMAX_DRAW_SCORE, MINIMAX_NOT_ENDGAME
// Note: the player argument makes it possible to speed up this function.
// Assumptions: 
// (1) if current_player_is_x == true, the last thing played was an 'X'.
// (2) if current_player_is_x == false, the last thing played was an 'O'.
// Hint: If you know the game was not over when an 'X' was played,
// you don't need to look for 'O's, and vice-versa.
minimax_score_t minimax_computeBoardScore(minimax_board_t* board, bool player_is_x){
	int16_t cur_Win_Type;
	int16_t cur_Win_Score;
	if(player_is_x){
		cur_Win_Type = MINIMAX_X_SQUARE;
		cur_Win_Score = MINIMAX_X_WINNING_SCORE;
	}
	else{
		cur_Win_Type = MINIMAX_O_SQUARE;
		cur_Win_Score = MINIMAX_O_WINNING_SCORE;
	}
	if ((board->squares[R0][C0]== cur_Win_Type && board->squares[R0][C1]== cur_Win_Type && board->squares[R0][C2]==cur_Win_Type) || //123 //Row 0
		(board->squares[R1][C0]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R1][C2]==cur_Win_Type) || //456 //Row 1
		(board->squares[R2][C0]== cur_Win_Type && board->squares[R2][C1]== cur_Win_Type && board->squares[R2][C2]==cur_Win_Type) || //789 //Row2
		(board->squares[R0][C0]== cur_Win_Type && board->squares[R1][C0]== cur_Win_Type && board->squares[R2][C0]==cur_Win_Type) || //147 //Col 0
		(board->squares[R0][C1]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R2][C1]==cur_Win_Type) || //258 //Col 1
		(board->squares[R0][C2]== cur_Win_Type && board->squares[R1][C2]== cur_Win_Type && board->squares[R2][C2]==cur_Win_Type) || //369 //Col 2
		(board->squares[R0][C0]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R2][C2]==cur_Win_Type) || //159 //Diag 1-9
		(board->squares[R0][C2]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R2][C0]==cur_Win_Type)){  //357 //Diag 3-7
		return cur_Win_Score;
	}
	for(uint16_t i = 0; i < COL_SIZE; i++){
		for(uint16_t j = 0; j < ROW_SIZE; j++){
			if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE){
				return MINIMAX_NOT_ENDGAME;
			}
		}
	}
	return MINIMAX_DRAW_SCORE;
}

// Init the board to all empty squares.
void minimax_initBoard(minimax_board_t* board){
	for (int16_t i = 0; i < COL_SIZE; i++){//magic
		for (int16_t j = 0; j < ROW_SIZE; j++){//magic
			board->squares[i][j] = 0;//magic
		}		
	}
}

