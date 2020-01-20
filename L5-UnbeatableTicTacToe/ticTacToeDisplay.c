/*
 * ticTacToeDisplay.c
 *
 *  Created on: Oct 19, 2017
 *      Author: joanmaga
 */

#include "ticTacToeDisplay.h"
#include "supportFiles/display.h"
#include "minimax.h"
#include "supportFiles/utils.h"     //Use it to add delay to some functions
#include "src/Lab2/buttons.h"
#include "src/Lab2/switches.h"


#define R0 0 //used to call row 0 in our board
#define R1 1 //used to call row 1 in our board
#define R2 2 //used to call row 2 in our board
#define C0 0 //used to call column 0 in our board
#define C1 1 //used to call column 1 in our board
#define C2 2 //used to call column 2 in our board
#define VERTIC_1 DISPLAY_WIDTH/3
#define VERTIC_2 2*VERTIC_1
#define HORIZON_1 DISPLAY_HEIGHT/3
#define HORIZON_2 2*HORIZON_1
#define HEIGHT_TOP 0
#define WIDTH_LEFT 0
#define ROW_0 DISPLAY_HEIGHT/6  //Represents position of Row 0 for O Display
#define ROW_1 DISPLAY_HEIGHT/2  //Represents position of Row 1 for O Display
#define ROW_2 2*DISPLAY_HEIGHT/3 + DISPLAY_HEIGHT/6  //Represents position of Row 2 for O Display
#define COL_0 DISPLAY_WIDTH/6  //Represents position of Column 0 for O Display
#define COL_1 DISPLAY_WIDTH/2  //Represents position of Column 1 for O Display
#define COL_2 2*DISPLAY_WIDTH/3 + DISPLAY_WIDTH/6  //Represents position of Column 2 for O Display
#define CIRCLE_RADIUS DISPLAY_HEIGHT/6  //Radius of O
#define DELAY 50    //Delay used in the Run Test Function

#define BUTTONS_BTN0_MASK 0x1
#define BUTTONS_BTN1_MASK 0x2
#define SWITCHES_SW0_MASK 0x1

#define TEXT_SIZE 2 //Defines the Text Size

int16_t x, y; //Parameters of the Touch Screen
uint8_t z;  //Pressure of Touch Screen

// Inits the tic-tac-toe display, draws the lines that form the board.
void ticTacToeDisplay_init()
{
    display_init();  // Must init all of the software and underlying hardware for LCD.
    display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
    display_drawLine(VERTIC_1, HEIGHT_TOP, VERTIC_1, DISPLAY_HEIGHT, DISPLAY_WHITE);    //Initializes Line 1
    display_drawLine(VERTIC_2, HEIGHT_TOP, VERTIC_2, DISPLAY_HEIGHT, DISPLAY_WHITE);    //Initializes Line 2
    display_drawLine(WIDTH_LEFT, HORIZON_1, DISPLAY_WIDTH, HORIZON_1, DISPLAY_WHITE);    //Initializes Line 3
    display_drawLine(WIDTH_LEFT, HORIZON_2, DISPLAY_WIDTH, HORIZON_2, DISPLAY_WHITE);    //Initializes Line 4
}

// Draws an X at the specified row and column.
// erase == true means to erase the X by redrawing it as background. erase == false, draw the X as foreground.
void ticTacToeDisplay_drawX(uint8_t row, uint8_t column, bool erase)
{
    //To Draw an X we will work FOUR Positions: Row Position 1 and 2, and Column Position 1 and 2
    int16_t row_position_1 = 0; //Initializes Row Position 1
    int16_t row_position_2 = 0; //Initializes Row Position 2
    int16_t col_position_1 = 0; //Initializes Column Position 1
    int16_t col_position_2 = 0; //Initializes Column Position 1

    switch (row) {  //Switch Statement to Choose the Positions of the Rows of Each Line to Draw an X
        case R0:    //Row 0 was Passed
            row_position_1 = HEIGHT_TOP;    //Updates Row Position of Line 1
            row_position_2 = HORIZON_1;    //Updates Row Position of Line 2
            break;
        case R1:   //Row 1 was Passed
            row_position_1 = HORIZON_1;    //Updates Row Position of Line 1
            row_position_2 = HORIZON_2;    //Updates Row Position of Line 2
            break;
        case R2:   //Row 2 was Passed
            row_position_1 = HORIZON_2;    //Updates Row Position of Line 1
            row_position_2 = DISPLAY_HEIGHT;    //Updates Row Position of Line 2
            break;
        default:
            break;
    }

    switch (column) {  //Switch Statement to Choose the Positions of the Column of Each Line to Draw an X
        case C0:   //Column 0 was Passed
            col_position_1 = WIDTH_LEFT;    //Updates Column Position of Line 1
            col_position_2 = VERTIC_1;    //Updates Column Position of Line 2
            break;
        case C1:   //Column 1 was Passed
            col_position_1 = VERTIC_1;    //Updates Column Position of Line 1
            col_position_2 = VERTIC_2;    //Updates Column Position of Line 2
            break;
        case C2:   //Column 2 was Passed
            col_position_1 = VERTIC_2;    //Updates Column Position of Line 1
            col_position_2 = DISPLAY_WIDTH;    //Updates Column Position of Line 2
            break;
        default:
            break;
        }

    display_drawLine(col_position_1, row_position_1, col_position_2, row_position_2, DISPLAY_WHITE);    //Draws Line 1 from X on Screen
    display_drawLine(col_position_1, row_position_2, col_position_2, row_position_1, DISPLAY_WHITE);    //Draws Line 2 from X on Screen
}

// Draws an O at the specified row and column.
// erase == true means to erase the X by redrawing it as background. erase == false, draw the X as foreground.
void ticTacToeDisplay_drawO(uint8_t row, uint8_t column, bool erase)
{
    //To Draw an O we will work TWO Positions: Row Position and Column Position
    int16_t row_position = 0; //Initializes Row Position
    int16_t col_position = 0; //Initializes Column Position

    switch (row) {  //Switch Statement to Choose the Positions of the Row of the Center of O
        case R0:    //Row 0 was Passed
            row_position = ROW_0;
            break;
        case R1:    //Row 1 was Passed
            row_position = ROW_1;
            break;
        case R2:    //Row 2 was Passed
            row_position = ROW_2;
            break;
        default:
            break;
    }

    switch (column) {  //Switch Statement to Choose the Positions of the Column of the Center of O
        case C0:    //Column 0 was Passed
            col_position = COL_0;
            break;
        case C1:    //Column 1 was Passed
            col_position = COL_1;
            break;
        case C2:    //Column 2 was Passed
            col_position = COL_2;
            break;
        default:
            //Print Statement
            break;
        }

        display_drawCircle(col_position, row_position, CIRCLE_RADIUS, DISPLAY_WHITE);    //Draws an O in the Screen
}

// After a touch has been detected and after the proper delay, this sets the row and column arguments
// according to where the user touched the board.
void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column)
{
    //Clears Old Touch Data before going through the Function
    display_clearOldTouchData();
    display_getTouchedPoint(&x, &y, &z);    //Get the Coordinates of the Touches

    if(x < VERTIC_1)    //Checks if x is in the Leftmost Column
        *column = C0;   //Declare Column Variable to be Column 0
    else if(x > VERTIC_1 && x < VERTIC_2)    //Checks if x is in the Middle Column
        *column = C1;   //Declare Column Variable to be Column 1
    else if(x > VERTIC_2)    //Checks if x is in the Rightmost Column
        *column = C2;   //Declare Column Variable to be Column 2

    if(y < HORIZON_1)    //Checks if y is in the Top Row
        *row = R0;   //Declare Row Variable to be Row 0
    else if(y > HORIZON_1 && y < HORIZON_2)    //Checks if y is in the Middle Row
        *row = R1;   //Declare Row Variable to be Row 1
    else if(y > HORIZON_2)      //Checks if y is in the Bottom Row
        *row = R2;   //Declare Row Variable to be Row 2

}

// Runs a test of the display. Does the following.
// Draws the board. Each time you touch one of the screen areas, the screen will paint
// an X or an O, depending on whether switch 0 (SW0) is slid up (O) or down (X).
// When BTN0 is pushed, the screen is cleared. The test terminates when BTN1 is pushed.
void ticTacToeDisplay_runTest()
{
    ticTacToeDisplay_init();    //Initializes the Board by Drawing the Four Board Lines

    while((buttons_read() & BUTTONS_BTN1_MASK) != BUTTONS_BTN1_MASK)    //Runs the Test until Button 1 is not PRessed
    {
        if((buttons_read() & BUTTONS_BTN0_MASK) == BUTTONS_BTN0_MASK)   //If Button 0 is pressed then Reset the Board Lines
        {
            ticTacToeDisplay_drawBoardLines();  //Function call to Reset the Board Lines
        }

        if(display_isTouched()){
            utils_msDelay(DELAY);   //Sets the Delay for Response
            uint8_t row, column;    //Declares Row and Column that are going to be updated inside the Compute Board Row Column Function
            ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);   //Compute Board Row Column and pass the Row and Column by Reference to transfer those values to Draw the X or O in the Correct Position

            if((switches_read() & SWITCHES_SW0_MASK) == SWITCHES_SW0_MASK)  //If Switch 0 is Up, Draw an O
            {
                ticTacToeDisplay_drawO(row, column, false); //Draws an O based on the Row and Column Values
            }
            else    //Only X's will be drawn
            {
                ticTacToeDisplay_drawX(row, column, false); //Draws an X based on the Row and Column Values
            }



        }
    }
    display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
    display_setCursor(WIDTH_LEFT, HEIGHT_TOP);            // The upper left of the LCD screen.
    display_setTextColor(DISPLAY_WHITE);   // Make the text black.
    display_setTextSize(TEXT_SIZE);                // Define Text Size
    display_println("Terminating Test\n\r");    //Print Statement to alert the User that the Test is Done
}

// This will draw the four board lines.
void ticTacToeDisplay_drawBoardLines()
{
    display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
    display_drawLine(VERTIC_1, HEIGHT_TOP, VERTIC_1, DISPLAY_HEIGHT, DISPLAY_WHITE);    //Initializes Line 1
    display_drawLine(VERTIC_2, HEIGHT_TOP, VERTIC_2, DISPLAY_HEIGHT, DISPLAY_WHITE);    //Initializes Line 2
    display_drawLine(WIDTH_LEFT, HORIZON_1, DISPLAY_WIDTH, HORIZON_1, DISPLAY_WHITE);    //Initializes Line 3
    display_drawLine(WIDTH_LEFT, HORIZON_2, DISPLAY_WIDTH, HORIZON_2, DISPLAY_WHITE);    //Initializes Line 4
}


