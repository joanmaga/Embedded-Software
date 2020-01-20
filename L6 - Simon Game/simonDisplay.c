#include "SimonDisplay.h"
#include <stdio.h>
#include "supportFiles/utils.h"
#include "supportFiles/display.h"

#define LEFT_BUTTON_REGION (DISPLAY_WIDTH/4) - (SIMON_DISPLAY_BUTTON_WIDTH/2)	//x0 Position of Buttons Red and Blue
#define RIGHT_BUTTON_REGION ((DISPLAY_WIDTH*3)/4) - (SIMON_DISPLAY_BUTTON_WIDTH/2)	//x0 Position of Buttons Yellow and Green
#define UPPER_BUTTON_REGION (DISPLAY_HEIGHT/4) - (SIMON_DISPLAY_BUTTON_HEIGHT/2)	//y0 Position of Buttons Red and Yellow
#define LOWER_BUTTON_REGION ((DISPLAY_HEIGHT*3)/4) - (SIMON_DISPLAY_BUTTON_HEIGHT/2)	//y0 Position of Buttons Blue and Green

#define LEFT_SQUARE_REGION 0					//x0 Position of Squares Red and Blue
#define RIGHT_SQUARE_REGION (DISPLAY_WIDTH/2)	//x0 Position of Squares Yellow and Green
#define UPPER_SQUARE_REGION 0					//y0 Position of Squares Red and Yellow
#define LOWER_SQUARE_REGION (DISPLAY_HEIGHT/2)	//y0 Position of Squares Blue and Green

#define TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS 60 // in ms
#define MAX_STR 255
#define TEXT_SIZE 2
#define TEXT_VERTICAL_POSITION 0
#define TEXT_HORIZONTAL_POSITION (DISPLAY_HEIGHT/2)
#define INSTRUCTION_LINE_1 "Touch and release to start the Simon demo."
#define INSTRUCTION_LINE_2 "Demo will terminate after %d touches."
#define DEMO_OVER_MESSAGE_LINE_1 "Simon demo terminated"
#define DEMO_OVER_MESSAGE_LINE_2 "after %d touches."
#define TEXT_VERTICAL_POSITION 0 // Start at the far left.
#define ERASE_THE_SQUARE true  // drawSquare() erases if this is passed in.
#define DRAW_THE_SQUARE false  // drawSquare() draws the square if this is passed in.
#define ERASE_THE_BUTTON true  // drawSquare() erases if this is passed in.
#define DRAW_THE_BUTTON false  // drawSquare() draws the square if this is passed in.

int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y)
{
	int8_t region = 0;

	if (x < SIMON_DISPLAY_SQUARE_WIDTH) {	//Check if Touch was in left side of Screen
		if (y < SIMON_DISPLAY_SQUARE_HEIGHT)	//Check if Touch was in upper location of Screen
			region = SIMON_DISPLAY_REGION_0;	//Region is Red
		else									//Touch was in the lower location of Screen
			region = SIMON_DISPLAY_REGION_2;	//Region is Red
	}
	else {	//Touch was in the Right side of Screen
		if (y < SIMON_DISPLAY_SQUARE_HEIGHT)	//Check if Touch was in upper location of Screen
			region = SIMON_DISPLAY_REGION_1;	//Region is Yellow
		else									//Touch was in the Right side of Screen
			region = SIMON_DISPLAY_REGION_3;	//Region is Green
	}

	return region;
}

void simonDisplay_drawButton(uint8_t regionNumber, bool erase)
{
	uint16_t reg_color = 0;
	if (erase)	//Check if Button needs to be erased
		reg_color = DISPLAY_BLACK;	//If erase flag is up, draw Black square over the Button
	else {	//Button does not need to be erased
		switch (regionNumber) {	//Chooses the Color of the Button based on the Region Number
			case SIMON_DISPLAY_REGION_0:	//Checks if Region Number is 0
				reg_color = DISPLAY_RED;	//Choose color to be Red
				break;
			case SIMON_DISPLAY_REGION_1:	//Checks if Region Number is 1
				reg_color = DISPLAY_YELLOW;	//Choose color to be Yellow
				break;
			case SIMON_DISPLAY_REGION_2:	//Checks if Region Number is 2
				reg_color = DISPLAY_BLUE;	//Choose color to be Blue
				break;
			case SIMON_DISPLAY_REGION_3:	//Checks if Region Number is 3
				reg_color = DISPLAY_GREEN;	//Choose color to be Green
				break;
			default:						//If Switch Statement gets here, then no Valid Region Number was passed to Function
				printf("Region Number is invalid\n\r");	//Print Statement for User to know that the Region number is not Valid
				break;
		}
	}

	switch (regionNumber) {		//Chooses which color and which region will have a button drawn at
		case SIMON_DISPLAY_REGION_0:	//Checks if Region Number is 0 (Red)
			display_fillRect(LEFT_BUTTON_REGION, UPPER_BUTTON_REGION, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Button 0
			break;
		case SIMON_DISPLAY_REGION_1:	//Checks if Region Number is 1 (Yellow)
			display_fillRect(RIGHT_BUTTON_REGION, UPPER_BUTTON_REGION, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Button 1
			break;
		case SIMON_DISPLAY_REGION_2:	//Checks if Region Number is 2 (Blue)
			display_fillRect(LEFT_BUTTON_REGION, LOWER_BUTTON_REGION, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Button 2
			break;
		case SIMON_DISPLAY_REGION_3:	//Checks if Region Number is 3 (Green)
			display_fillRect(RIGHT_BUTTON_REGION, LOWER_BUTTON_REGION, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Button 3
			break;
		default:						//If Switch Statement gets here, then no Valid Region Number was passed to Function
			printf("Region Number is invalid\n\r");	//Print Statement for User to know that the Region number is not Valid
			break;
	}

}

void simonDisplay_drawAllButtons()
{
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_0, DRAW_THE_BUTTON);	//Draws a Red Rectangle in the Region of Button 0
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_1, DRAW_THE_BUTTON);	//Draws a Yellow Rectangle in the Region of Button 1
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_2, DRAW_THE_BUTTON);	//Draws a Blue Rectangle in the Region of Button 2
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_3, DRAW_THE_BUTTON);	//Draws a Green Rectangle in the Region of Button 3
}

void simonDisplay_eraseAllButtons()
{
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_0, ERASE_THE_BUTTON);	//Draws a Black Rectangle in the Region of Button 0
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_1, ERASE_THE_BUTTON);	//Draws a Black Rectangle in the Region of Button 1
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_2, ERASE_THE_BUTTON);	//Draws a Black Rectangle in the Region of Button 2
	simonDisplay_drawButton(SIMON_DISPLAY_REGION_3, ERASE_THE_BUTTON);	//Draws a Black Rectangle in the Region of Button 3
}

void simonDisplay_drawSquare(uint8_t regionNo, bool erase)
{
	uint16_t reg_color = 0;
	if (erase)	//Check if Square needs to be erased
		reg_color = DISPLAY_BLACK;	//If erase flag is up, draw Black square over the Square
	else {	//Square does not need to be erased
		switch (regionNo) {	//Chooses the Color of the Square based on the Region Number
		case SIMON_DISPLAY_REGION_0:	//Checks if Region Number is 0
			reg_color = DISPLAY_RED;	//Choose color to be Red
			break;
		case SIMON_DISPLAY_REGION_1:	//Checks if Region Number is 1
			reg_color = DISPLAY_YELLOW;	//Choose color to be Yellow
			break;
		case SIMON_DISPLAY_REGION_2:	//Checks if Region Number is 2
			reg_color = DISPLAY_BLUE;	//Choose color to be Blue
			break;
		case SIMON_DISPLAY_REGION_3:	//Checks if Region Number is 3
			reg_color = DISPLAY_GREEN;	//Choose color to be Green
			break;
		default:						//If Switch Statement gets here, then no Valid Region Number was passed to Function
			printf("Region Number is invalid\n\r");	//Print Statement for User to know that the Region number is not Valid
			break;
		}
	}

	switch (regionNo) {		//Chooses which color and which region will have a Square drawn at
	case SIMON_DISPLAY_REGION_0:	//Checks if Region Number is 0 (Red)
		display_fillRect(LEFT_SQUARE_REGION, UPPER_SQUARE_REGION, SIMON_DISPLAY_SQUARE_WIDTH, SIMON_DISPLAY_SQUARE_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Square 0
		break;
	case SIMON_DISPLAY_REGION_1:	//Checks if Region Number is 1 (Yellow)
		display_fillRect(RIGHT_SQUARE_REGION, UPPER_SQUARE_REGION, SIMON_DISPLAY_SQUARE_WIDTH, SIMON_DISPLAY_SQUARE_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Square 1
		break;
	case SIMON_DISPLAY_REGION_2:	//Checks if Region Number is 2 (Blue)
		display_fillRect(LEFT_SQUARE_REGION, LOWER_SQUARE_REGION, SIMON_DISPLAY_SQUARE_WIDTH, SIMON_DISPLAY_SQUARE_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Square 2
		break;
	case SIMON_DISPLAY_REGION_3:	//Checks if Region Number is 3 (Green)
		display_fillRect(RIGHT_SQUARE_REGION, LOWER_SQUARE_REGION, SIMON_DISPLAY_SQUARE_WIDTH, SIMON_DISPLAY_SQUARE_HEIGHT, reg_color);	//Draws a Rectangle in the Region of Square 3
		break;
	default:						//If Switch Statement gets here, then no Valid Region Number was passed to Function
		printf("Region Number is invalid\n\r");	//Print Statement for User to know that the Region number is not Valid
		break;
	}
}

// Runs a brief demonstration of how buttons can be pressed and squares lit up to implement the user
// interface of the Simon game. The routine will continue to run until the touchCount has been reached, e.g.,
// the user has touched the pad touchCount times.

// I used a busy-wait delay (utils_msDelay) that uses a for-loop and just blocks until the time has passed.
// When you implement the game, you CANNOT use this function as we discussed in class. Implement the delay
// using the non-blocking state-machine approach discussed in class.
void simonDisplay_runTest(uint16_t touchCount)
{
	display_init();         // Always initialize the display.
	char str[MAX_STR];      // Enough for some simple printing.
	uint8_t regionNumber = 0;   // Convenience variable.
	uint16_t touches = 0;   // Terminate when you receive so many touches.
							// Write an informational message and wait for the user to touch the LCD.
	display_fillScreen(DISPLAY_BLACK);              // clear the screen.
	display_setCursor(TEXT_VERTICAL_POSITION, TEXT_HORIZONTAL_POSITION); // move to the middle of the screen.
	display_setTextSize(TEXT_SIZE);                 // Set the text size for the instructions.
	display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);   // Reasonable text color.
	sprintf(str, INSTRUCTION_LINE_1);                   // Copy the line to a buffer.
	display_println(str);                               // Print to the LCD.
	display_println();                                  // new-line.
	sprintf(str, INSTRUCTION_LINE_2, touchCount);       // Copy the line to a buffer.
	display_println(str);                               // Print to the LCD.
	while (!display_isTouched());       // Wait here until the screen is touched.
	while (display_isTouched());        // Now wait until the touch is released.
	display_fillScreen(DISPLAY_BLACK);  // Clear the screen.
	simonDisplay_drawAllButtons();      // Draw all of the buttons.
	bool touched = false;  		// Keep track of when the pad is touched.
	int16_t x, y;  					// Use these to keep track of coordinates.
	uint8_t z;      					// This is the relative touch pressure.
	while (touches < touchCount) {  // Run the loop according to the number of touches passed in.
		if (!display_isTouched() && touched) {          // user has stopped touching the pad.
			simonDisplay_drawSquare(regionNumber, ERASE_THE_SQUARE);  // Erase the square.
			simonDisplay_drawButton(regionNumber, DRAW_THE_BUTTON);  //  Draw the button.
			touched = false;					// Released the touch, set touched to false.
		}
		else if (display_isTouched() && !touched) {	// User started touching the pad.
			touched = true;                             // Just touched the pad, set touched = true.
			touches++;                                  // Keep track of the number of touches.
			display_clearOldTouchData();                // Get rid of data from previous touches.
														// Must wait this many milliseconds for the chip to do analog processing.
			utils_msDelay(TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS);
			display_getTouchedPoint(&x, &y, &z);        // After the wait, get the touched point.
			regionNumber = simonDisplay_computeRegionNumber(x, y);// Compute the region number, see above.
			simonDisplay_drawSquare(regionNumber, DRAW_THE_SQUARE);  // Draw the square (erase = false).
		}
	}
	// Done with the demo, write an informational message to the user.
	display_fillScreen(DISPLAY_BLACK);        // clear the screen.
											  // Place the cursor in the middle of the screen.
	display_setCursor(TEXT_VERTICAL_POSITION, TEXT_HORIZONTAL_POSITION);
	display_setTextSize(TEXT_SIZE); // Make it readable.
	display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);  // red is foreground color, black is background color.
	sprintf(str, DEMO_OVER_MESSAGE_LINE_1);    // Format a string using sprintf.
	display_println(str);                     // Print it to the LCD.
	sprintf(str, DEMO_OVER_MESSAGE_LINE_2, touchCount);  // Format the rest of the string.
	display_println(str);  // Print it to the LCD.
}



