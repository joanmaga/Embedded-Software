#include <stdio.h>
#include "ticTacToeControl.h"
#include "supportFiles/display.h"
#include "ticTacToeDisplay.h"
#include "minimax.h"
#include "src/Lab2/buttons.h"

#define BUTTONS_BTN0_MASK 0x1   //Mask for Button 1
#define MAX_WAIT_USER 100   //Set Max wait for User to be 100
#define ADC_COUNTER_MAX_VALUE 1 //Max value to wait for ADC to settle is 1
#define X_VALUE 2    // player-square means X occupies the square.
#define O_VALUE 1  // opponent-square means O occupies the square.

static uint32_t wait = 0, adcCounter = 0;   //Variables used for Waiting purposes: Wait for ADC to Settle, and Wait for User to Go first
static bool comp_is_x, is_comp_turn;   //Boolean variables to check if it's X's Turn and/or Computer's Turn
static uint8_t row, column;
static uint16_t score;

static minimax_board_t board;

// States for the controller state machine.
enum tictacControl_st_t {
    init_st,                 // Start here, transition out of this state on the first tick.
    wait_display_screen,    //Waits for the Display Screen to Initialize
    wait_for_input, //Waits for First Move. Either Computer or User
    waiting_for_touch_st,    // waiting for touch, clock is enabled and running.
    adc_Counter_running_st,     // waiting for the touch-controller ADC to settle.
    comp_turn,  //State to take care of Computer's turn
    user_turn  //State to take care of User's turn
} currentState;

// This is a debug state print routine. It will print the names of the states each
// time tick() is called. It only prints states if they are different than the
// previous state.
void debugStatePrint() {
  static tictacControl_st_t previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state name over and over.
  if (previousState != currentState || firstPass) {
    firstPass = false;                // previousState will be defined, firstPass is false.
    previousState = currentState;     // keep track of the last state that you were in.
    switch(currentState) {            // This prints messages based upon the state that you were in.
      case init_st: //Enter Case init_st if that is the Current State
        printf("init_st\n\r");  //Print Current State
        break;
      case wait_display_screen:     //Enter Case wait_display_screen if that is the Current State
        printf("wait_display_screen\n\r");  //Print Current State
        break;
      case waiting_for_touch_st:     //Enter Case waiting_for_touch_st if that is the Current State
        printf("waiting_for_touch_st\n\r");  //Print Current State
        break;
      case adc_Counter_running_st:     //Enter Case adc_Counter_running_st if that is the Current State
        printf("ad_timer_running_st\n\r");  //Print Current State
        printf("adcCounter: %ld\n\r", adcCounter);    //Check Value of Counter adcCounter for debugging purposes
        break;
      case wait_for_input:     //Enter Case wait_for_input if that is the Current State
        printf("wait_for_input\n\r");  //Print Current State
        break;
      case comp_turn:     //Enter Case comp_turn if that is the Current State
        printf("comp_turn\n\r");  //Print Current State
        break;
      case user_turn:     //Enter Case user_turn if that is the Current State
        printf("user_turn\n\r");  //Print Current State
        break;
     }
  }
}

void tictacControl_init()
{
    currentState = init_st; //Initialize Current STate by setting it to the Init_st State
    comp_is_x = true, is_comp_turn = true;  //Initializes the Computer turn and Computer is X to be True. These values will change through out the running of the program
    wait = 0, adcCounter = 0;   //Initializes the Variables that will be Incremented when waiting for ADC to settle or For user to  Make the First Move
    score = 0;  //Initializes the Score to be Zero
    minimax_initBoard(&board);  //Initializes the Values of Each Square in the Board
}

//Checks to see if the Game is Over
bool isGameOver(bool is_comp_turn)
{
    score = 0;  //Set the score to be Zero
    if((comp_is_x && is_comp_turn)||(!comp_is_x && !is_comp_turn))  //Checks to see if X is playing
    {
        score = minimax_computeBoardScore(&board, true);    //Compute the Socre based on the Board in the Display
    }
    else
    {
        score = minimax_computeBoardScore(&board, false);  //Checks to see if O is playing
    }
    bool over = minimax_isGameOver(score);    //Compute the Socre based on the Board in the Display
    return over;    //Returns if Game is over or Not
}

// Call this before you call clockControl_tick().
void tictacControl_tick()
{
    debugStatePrint();  //Calls the Debug State Print Function for debugging purposes
    // Perform state update first.
    switch(currentState) {
        case init_st: //Enter Case init_st if that is the Current State
          currentState = wait_display_screen;   //Goes straight to Wait_Display Screen
          break;
        case wait_display_screen:     //Enter Case wait_display_screen if that is the Current State
            currentState = wait_for_input;  //Goes to Wait for Input
            break;
        case wait_for_input:     //Enter Case wait_for_input if that is the Current State
            if(display_isTouched()) //Display was touched. If statement should Execute the body
            {
                comp_is_x = false;  //If display is Touched than the Player went first.
                currentState = adc_Counter_running_st;    //Go to Waiting for Touch state after first Touch
            }
            else if(wait == MAX_WAIT_USER)  //Wait reached the Max Value. Computer Goes First
            {
                comp_is_x = true;   //User did not Touch the Board. Computer is X and playes first
                currentState = comp_turn;    //Goes to Computer Turn State
            }
            else    //No actions Occurred. Stay on this State
            {
                currentState = wait_for_input;  //Stay on this State
            }
            break;
        case waiting_for_touch_st:     //Enter Case waiting_for_touch_st if that is the Current State
              if((buttons_read() & BUTTONS_BTN0_MASK) == BUTTONS_BTN0_MASK) //Button 0 was pressed. Reset Board
              {
                  minimax_initBoard(&board);    //Reinitializes the Board
                  display_clearOldTouchData();    //Clear screen from old touches
                  currentState = wait_display_screen;   //Goes to Wait Display Screen State
              }
              else if(isGameOver(is_comp_turn)) //If game is over, then wait in this State until user presses Button 0
              {
                  currentState = waiting_for_touch_st; //Wait here until User Press BTN0
              }
              else if(display_isTouched())  //Display is Touched, user goes
              {
                  currentState = adc_Counter_running_st;    //Goes to state where it waits until the ADC Settles
              }
              else    //No actions Occurred. Stay on this State
              {
                  currentState = waiting_for_touch_st;  //Stay on this State
              }
            break;
        case adc_Counter_running_st:     //Enter Case adc_Counter_running_st if that is the Current State
            if (adcCounter == ADC_COUNTER_MAX_VALUE)    //Checks to see if ADC reached the Max Value
            {
                ticTacToeDisplay_touchScreenComputeBoardRowColumn(&row, &column);   //Calls function to Compute the Row and Column that were touched in the Screen
                if((board.squares[row][column] != X_VALUE)&&(board.squares[row][column] != O_VALUE))    //Checks to see if square is Empty
                {
                    currentState = user_turn;   //Goes to User Turn State
                }
                else    //Square is not Empty. Wait for User to Input in new place on Screen
                {
                    currentState = waiting_for_touch_st;    //Go to Waiting for Touch State
                }
            }
            else    //ADC Counter hasn't Settled. Stay here until it does
            {
                currentState = adc_Counter_running_st;  //Stay on ADC Counter State
            }
            break;
        case comp_turn:     //Enter Case comp_turn if that is the Current State
            currentState = waiting_for_touch_st;    //Go to Waiting for touch State
            break;
        case user_turn:     //Enter Case user_turn if that is the Current State
            currentState = comp_turn;   //Go to Computer Turn State
            break;
        default:
          break;
      }

      // Perform state action next.
      switch(currentState) {
          case init_st: //Enter Case init_st if that is the Current State
            break;
          case wait_display_screen:     //Enter Case wait_display_screen if that is the Current State
              ticTacToeDisplay_init();  //Initializes the Display
              break;
          case wait_for_input:     //Enter Case wait_for_input if that is the Current State
              wait++;   //Increments the Value of Wait until it reaches the Max Value
              display_clearOldTouchData();    //Clear screen from old touches
              break;
          case waiting_for_touch_st:     //Enter Case waiting_for_touch_st if that is the Current State
              adcCounter = 0;   //Sets ADC Counter Variable to Zero
              wait = 0; //Sets the Wait Variable to 0
              break;
          case adc_Counter_running_st:     //Enter Case adc_Counter_running_st if that is the Current State
              adcCounter++; //Increments the ADC Counter Until it reaches the Max Value
              break;
          case comp_turn:     //Enter Case comp_turn if that is the Current State
              minimax_computeNextMove(&board, comp_is_x, &row, &column);    //Computes the Move the Computer should play
              if(comp_is_x){    //Checks to see if Computer is X
                  board.squares[row][column] = X_VALUE; //Adds an X to a Square on the Board
                  ticTacToeDisplay_drawX(row, column, false);   //Draws the X on the Screen
              }
              else{ //Computer is O
                  board.squares[row][column] = O_VALUE; //Adds an O to a Square on the Board
                  ticTacToeDisplay_drawO(row, column, false);   //Draws the O on the Screen
              }
              is_comp_turn = !is_comp_turn; //Not Computer Turn anymore. Not this Variable
            break;
          case user_turn:     //Enter Case user_turn if that is the Current State
              if(comp_is_x)    //Checks to see if Computer is X
              {
                  board.squares[row][column] = O_VALUE;  //Adds an O to a Square on the Board
                  ticTacToeDisplay_drawO(row, column, false);   //Draws the O on the Screen
              }
              else //Computer is O
              {
                  board.squares[row][column] = X_VALUE; //Adds an X to a Square on the Board
                  ticTacToeDisplay_drawX(row, column, false);   //Draws the X on the Screen
              }
              is_comp_turn = !is_comp_turn; //Not User Turn anymore. Not this Variable
              break;
         default:
          break;
      }
}


