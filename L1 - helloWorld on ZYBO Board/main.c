
// Print out "hello world" on both the console and the LCD screen.

#include <stdio.h>
#include "supportFiles/display.h"
int main() {
	display_init();  // Must init all of the software and underlying hardware for LCD.
	display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
	display_setCursor(0, 0);            // The upper left of the LCD screen.
	display_setTextColor(DISPLAY_RED);  // Make the text red.
	display_setTextSize(2);             // Make the text a little larger.
	//display_println("hello world (on the LCD)!");    // This actually prints the string to the LCD.
	//DRAW
	display_drawLine(0, 0, 320, 240, DISPLAY_GREEN);
	display_drawLine(320, 0, 0, 240, DISPLAY_GREEN);
	display_drawCircle( 80,  120,  30,  DISPLAY_RED);
	display_fillCircle( 240,  120,  30,  DISPLAY_RED);
	display_fillTriangle( 160,  80,  125,  20,
	   195,  20,  DISPLAY_YELLOW);
	display_drawTriangle( 160,  160,  125,  220,
	   195,  220,  DISPLAY_YELLOW);


	printf("hello world! ");  // This prints on the console.
}

// This function must be defined but can be left empty for now.
// You will use this function in a later lab.
// It is called in the timer interrupt service routine (see interrupts.c in supportFiles).
void isr_function() {
    // Empty for now.
}
