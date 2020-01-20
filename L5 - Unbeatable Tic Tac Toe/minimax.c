/*
 * minimax.c
 *
 *  Created on: Oct 19, 2017
 *      Author: joanmaga
 */

#include "minimax.h"    //Minimax Header file provided by Instructor
#include <stdio.h>      //Standard Input Output Library

#define R0 0 //used to call row 0 in our board
#define R1 1 //used to call row 1 in our board
#define R2 2 //used to call row 2 in our board
#define C0 0 //used to call column 0 in our board
#define C1 1 //used to call column 1 in our board
#define C2 2 //used to call column 2 in our board
#define INIT_SC -2  //Value used to initialize the Score Table
#define MAX_TABLE 9 //Maximum number of elements allowed in Board
#define SIZE 3  //Size of Board
#define INIT_LOW_SCORE 1 //Initial Low Score for O
#define INIT_HIGH_SCORE -1 //Initial High Score for X
#define INIT_BEST_INDEX 0 //Used to avoid Magic Numbers when initializing best Index
#define EMPTY_SQUARE 0  //Initializer for when Square in Board is Empty

static minimax_move_t choice;   //Variable used for finding the Best choice computed

void print_Board(minimax_board_t* board)    //Print Board for debugging purposes
{

    printf("%d %d %d\n\r", board->squares[R0][C0], board->squares[R0][C1], board->squares[R0][C2]); //Print Statement 1
    printf("%d %d %d\n\r", board->squares[R1][C0], board->squares[R1][C1], board->squares[R1][C2]); //Print Statement 2
    printf("%d %d %d\n\r", board->squares[R2][C0], board->squares[R2][C1], board->squares[R2][C2]); //Print Statement 3
}

// current_player_is_x == true means X, current_player_is_x == false means O
int minimax(minimax_board_t* board, bool current_player_is_x, int16_t depth) {
    //evaluates the current score of the game by checking the previous player's move (hence the "!" before current_player_is_x)
    int16_t score = minimax_computeBoardScore(board, !current_player_is_x);
    bool game_is_over = minimax_isGameOver(score);  //Bool to check if Game is over
    if (game_is_over){ //if the game is over
        return score;//return the current score of the game
    }
    // Otherwise, you need to recurse.
    // This loop will generate all possible boards.
    int16_t score_table[MAX_TABLE] = { INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC, INIT_SC };   //Initialize the Score Table
    minimax_move_t moves_table[MAX_TABLE];  //Create and define a Size for the Moves Table
    for (int8_t mm_row = 0; mm_row < SIZE; mm_row++) { //For loop to go through all the rows
        for (int8_t mm_col = 0; mm_col < SIZE; mm_col++) { //For loop to go through all the columns
            if (board->squares[mm_row][mm_col] == MINIMAX_EMPTY_SQUARE) {   //Only Evaluates square if it's empty
                // Assign the square to the player or opponent.
                if (current_player_is_x) {  //Checks if Current player is X
                    board->squares[mm_row][mm_col] = MINIMAX_X_SQUARE;  //Draw an X in Board
                }
                else{   //Current player is O
                    board->squares[mm_row][mm_col] = MINIMAX_O_SQUARE;  //Draw an O in Board
                }

                score = minimax(board, !current_player_is_x, depth + 1);    //Recursively goes through Minimax again to find another score
                for (uint16_t table_index  = 0; table_index < MAX_TABLE; table_index++){    //For loop to update the Score and Move Table
                    if(score_table[table_index] == INIT_SC){    //If Score Table index isn't modified, i.e. if Score Table index has the initial value, update it
                        score_table[table_index] = score;   //Add Score to Score table Index
                        moves_table[table_index].row = mm_row;  //Add Move Row to Move Table Index
                        moves_table[table_index].column = mm_col; //Add Move Column to Move Table Index
                        table_index = MAX_TABLE;    //We found a better score, exit the For loop
                    }
                }
                // Undo the change to the board (return the square to empty) prior to next iteration of for-loops.
                board->squares[mm_row][mm_col] = MINIMAX_EMPTY_SQUARE;
            }
        }
    }
    if (current_player_is_x) {  //Check is Current player is X
        int16_t cur_high_score = INIT_HIGH_SCORE;   //Initialize the High Score of X to be -1
        int16_t best_index = INIT_BEST_INDEX;   //Initializes the Best Index of the Score Table. It will change if we find a better score in the Table
        for (uint16_t table_index = 0; table_index < MAX_TABLE; table_index++){ //For Loop to go through all the possible positions in the Score table
            if(score_table[table_index] > cur_high_score){ //If we find a better Score than the High Score, access the If Statement
                best_index = table_index;   //We found a better Index, Update it
                choice.row = moves_table[table_index].row;  //Update the Choice Row based on the Moves table at the depicted Index
                choice.column = moves_table[table_index].column;  //Update the Choice Column based on the Moves table at the depicted Index
                cur_high_score = score_table[table_index];  //Update the Current High Score
                score = score_table[table_index];   //Set the Variable score that is supposed to be Returned
                if (score_table[table_index] > MINIMAX_DRAW_SCORE){ //If we find a better value than DRAW_SCORE i.e. We found the highest score possible, return the Score
                    return score;   //Returns the score and recursively gets out of this minimax instance
                }
            }
        }
        //Updates Choice Row and Column based on the best index at the moves table
        choice.row = moves_table[best_index].row;
        choice.column = moves_table[best_index].column;
        return score_table[best_index]; //Returns the score at the Best Index
    }
    else {  //Current player is O
        int16_t cur_low_score = INIT_LOW_SCORE; //Initialize the Low Score of X to be 1
        int16_t best_index = INIT_BEST_INDEX;   //Initializes the Best Index of the Score Table. It will change if we find a better score in the Table
        for (uint16_t table_index = 0; table_index < MAX_TABLE; table_index++){ //For Loop to go through all the possible positions in the Score table
            if(score_table[table_index] < cur_low_score && score_table[table_index] != INIT_SC){    //If we find a lower Score than the Low Score, access the If Statement
                best_index = table_index;   //We found a better Index, Update it
                choice.row = moves_table[table_index].row;  //Update the Choice Row based on the Moves table at the depicted Index
                choice.column = moves_table[table_index].column;  //Update the Choice Column based on the Moves table at the depicted Index
                cur_low_score = score_table[table_index];  //Update the Current Low Score
                score = score_table[table_index];   //Set the Variable score that is supposed to be Returned
                if (score_table[table_index] < MINIMAX_DRAW_SCORE){ //If we find a lower value than DRAW_SCORE i.e. We found the highest score possible, return the Score
                    return score;   //Returns the score and recursively gets out of this minimax instance
                }
            }
        }
        //Updates Choice Row and Column based on the best index at the moves table
        choice.row = moves_table[best_index].row;
        choice.column = moves_table[best_index].column;
        return score_table[best_index]; //Returns the score at the Best Index
    }
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
    minimax(board, current_player_is_x, 0); //Calls minimax that is going to choose the best row and column to be played
    *row = choice.row;  //Updates the Best Row
    *column = choice.column;    //Updates the Best Column
    print_Board(board); //Prints the Board for Debugging purposes
}

// Determine that the game is over by looking at the score.
bool minimax_isGameOver(minimax_score_t score){
    if (score == MINIMAX_NOT_ENDGAME){  //If Score equals -1 then the Game keeps going
        return false;   //Return that the Game is not over
    }
    else    //Game is over
        return true;    //Return that the Game is over
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
    int16_t cur_Win_Type;   //Variable to check if the winning Player is X or O
    int16_t cur_Win_Score;  //Variable to check if the winning Score is -10 or 10 based on which player is playing
    if(player_is_x){    //Checks if Current player is X
        cur_Win_Type = MINIMAX_X_SQUARE;    //Update the Win Type to be of Type X
        cur_Win_Score = MINIMAX_X_WINNING_SCORE;    //Updates the Winning Score to be 10
    }
    else{   //Current Player is O
        cur_Win_Type = MINIMAX_O_SQUARE;    //Update the Win Type to be of Type O
        cur_Win_Score = MINIMAX_O_WINNING_SCORE;    //Updates the Winning Score to be -10
    }
    //If Statement that goes through all the possible wins
    if ((board->squares[R0][C0]== cur_Win_Type && board->squares[R0][C1]== cur_Win_Type && board->squares[R0][C2]==cur_Win_Type) || //123 //Row 0
        (board->squares[R1][C0]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R1][C2]==cur_Win_Type) || //456 //Row 1
        (board->squares[R2][C0]== cur_Win_Type && board->squares[R2][C1]== cur_Win_Type && board->squares[R2][C2]==cur_Win_Type) || //789 //Row2
        (board->squares[R0][C0]== cur_Win_Type && board->squares[R1][C0]== cur_Win_Type && board->squares[R2][C0]==cur_Win_Type) || //147 //Col 0
        (board->squares[R0][C1]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R2][C1]==cur_Win_Type) || //258 //Col 1
        (board->squares[R0][C2]== cur_Win_Type && board->squares[R1][C2]== cur_Win_Type && board->squares[R2][C2]==cur_Win_Type) || //369 //Col 2
        (board->squares[R0][C0]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R2][C2]==cur_Win_Type) || //159 //Diag 1-9
        (board->squares[R0][C2]== cur_Win_Type && board->squares[R1][C1]== cur_Win_Type && board->squares[R2][C0]==cur_Win_Type)){  //357 //Diag 3-7
        return cur_Win_Score;   //A Win was obtained. Return Current Winning Score
    }
    for(uint16_t i = 0; i < SIZE; i++){ //For loop to go through all the Rows
        for(uint16_t j = 0; j < SIZE; j++){ //For loop to go through all the Columns
            if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE){  //If any of the Squares are empty and no Wins occurred, then Game is not Over
                return MINIMAX_NOT_ENDGAME; //Return -1 representing that the Game is not over
            }
        }
    }
    return MINIMAX_DRAW_SCORE;  //If all the If statements were checked, the base case is that the Game was Drawn. Return 0
}

// Init the board to all empty squares.
void minimax_initBoard(minimax_board_t* board){
    for (int16_t i = 0; i < SIZE; i++){ //Goes through all the Rows to initialize the board
        for (int16_t j = 0; j < SIZE; j++){ //Goes through all the Columns to initialize the board
            board->squares[i][j] = EMPTY_SQUARE;    //Initialize each Board Square to have a Value of Zero
        }
    }
}




