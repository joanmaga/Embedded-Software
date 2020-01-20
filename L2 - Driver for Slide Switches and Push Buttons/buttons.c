/*
 * buttons.c
 *
 *  Created on: Sep 13, 2017
 *      Author: joanmaga
 */

//Support files necessary to get the program running
#include "buttons.h"
#include "xparameters.h"
#include "supportFiles/leds.h"
#include "xil_io.h"
#include "supportFiles/display.h"

#define printFailedStatusFlag true
#define GPIO_TRI_OFFSET 0x04
#define GPIO_TRI_INPUT 0x0F
#define GPIO_DATA_OFFSET 0x00
#define ALL_ON 0x0F //Value for all the buttons on
#define ALL_OFF 0x00 //Value for all buttons off

#define DISPLAY_OFFSET 20 //Helps with the positioning of text and shapes when displaying the buttons graphs in the display
#define DISPLAY_SIZE_ONE 40 //Helps with positioning of outputs on display
#define DISPLAY_SIZE_TWO 120 //Helps with positioning of outputs on display
#define DISPLAY_SIZE_THREE 160 //Helps with positioning of outputs on display
#define DISPLAY_SIZE_FOUR 240 //Helps with positioning of outputs on display
#define DISPLAY_MANIP 2   //Helps with display customization and avoid using a magic number
#define TEXT_SIZE 2            //Use to set Text Size

int32_t buttons_readGpioRegister(int32_t offset) //Read a Register, the Offset
{
    return Xil_In32(XPAR_PUSH_BUTTONS_BASEADDR  + offset);
}
void buttons_writeGpioRegister(int32_t offset, int32_t value) //Write to a Register, and pass which value I want to write to it
{
    Xil_Out32(XPAR_PUSH_BUTTONS_BASEADDR  + offset, value);
}

// Initializes the button driver software and hardware. Returns one of the defined status values (above).
int32_t buttons_init()
{
    display_init();  // Must init all of the software and underlying hardware for LCD.
    display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
    buttons_writeGpioRegister(GPIO_TRI_OFFSET, GPIO_TRI_INPUT); //Writes to the GPIO_TRI, and make the switches input
    if(buttons_readGpioRegister(GPIO_TRI_OFFSET) == GPIO_TRI_INPUT) //Check if Values are being passed correctly
    {
        return BUTTONS_INIT_STATUS_OK;
    }
    else
    {
        return BUTTONS_INIT_STATUS_FAIL;
    }
}

// Returns the current value of all 4 buttons as the lower 4 bits of the returned value.
// bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
int32_t buttons_read()
{
    return buttons_readGpioRegister(GPIO_DATA_OFFSET);
}

// Runs a test of the buttons. As you push the buttons, graphics and messages will be written to the LCD
// panel. The test will until all 4 pushbuttons are simultaneously pressed.
void buttons_runTest()
{
    buttons_init(); //Initializes the button driver software and hardware

    //Bools to avoid rerunning an if statement if button is already pressed
    bool btn0_off = true;
    bool btn1_off = true;
    bool btn2_off = true;
    bool btn3_off = true;

    while(buttons_read() != ALL_ON) //Run until all buttons are pressed at the same time
    {

        if(btn0_off && (buttons_read() & BUTTONS_BTN0_MASK) == BUTTONS_BTN0_MASK)
        {
            display_fillRect( DISPLAY_SIZE_FOUR,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_BLUE);
            display_setCursor(DISPLAY_SIZE_FOUR + DISPLAY_OFFSET, DISPLAY_SIZE_ONE);            // The upper left of the LCD screen.
            display_setTextColor(DISPLAY_BLACK);   // Make the text black.
            display_setTextSize(TEXT_SIZE);                // Define Text Size
            display_println("BTN0\n\r");           //Print this current button on Display
            btn0_off = false;
        }
        else if((buttons_read() & BUTTONS_BTN0_MASK) != BUTTONS_BTN0_MASK) //Turn button Display square black if button is not pressed
        {
            display_fillRect( DISPLAY_SIZE_FOUR,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_BLACK);
            btn0_off = true;
        }

        if(btn3_off && (buttons_read() & BUTTONS_BTN3_MASK) == BUTTONS_BTN3_MASK)
        {
            display_fillRect( 0,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_YELLOW);
            display_setCursor(DISPLAY_OFFSET, DISPLAY_SIZE_ONE);            // The upper left of the LCD screen.
            display_setTextColor(DISPLAY_BLACK);  // Make the text black.
            display_setTextSize(TEXT_SIZE);     // Define Text Size
            display_println("BTN3\n\r");        //Print this current button on Display
            btn3_off = false;
        }
        else if((buttons_read() & BUTTONS_BTN3_MASK) != BUTTONS_BTN3_MASK)
        {
            display_fillRect( 0,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_BLACK);
            btn3_off = true;
        }

        if(btn1_off && (buttons_read() & BUTTONS_BTN1_MASK) == BUTTONS_BTN1_MASK)
        {
            display_fillRect( DISPLAY_SIZE_THREE,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_RED);
            display_setCursor(DISPLAY_SIZE_THREE+DISPLAY_OFFSET, DISPLAY_SIZE_ONE);            // The upper left of the LCD screen.
            display_setTextColor(DISPLAY_BLACK);  // Make the text black.
            display_setTextSize(TEXT_SIZE);     // Define Text Size
            display_println("BTN1\n\r");        //Print this current button on Display
            btn1_off = false;
        }
        else if((buttons_read() & BUTTONS_BTN1_MASK) != BUTTONS_BTN1_MASK)
        {
            display_fillRect( DISPLAY_SIZE_THREE,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_BLACK);
            btn1_off = true;
        }

        if(btn2_off && (buttons_read() & BUTTONS_BTN2_MASK) == BUTTONS_BTN2_MASK)
        {
            display_fillRect( DISPLAY_SIZE_ONE*DISPLAY_MANIP,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_GREEN);
            display_setCursor(DISPLAY_SIZE_ONE*DISPLAY_MANIP + DISPLAY_OFFSET, DISPLAY_SIZE_ONE);            // The upper left of the LCD screen.
            display_setTextColor(DISPLAY_BLACK);  // Make the text black.
            display_setTextSize(TEXT_SIZE);     // Define Text Size
            display_println("BTN2\n\r");        //Print this current button on Display
            btn2_off = false;
        }
        else if((buttons_read() & BUTTONS_BTN2_MASK) != BUTTONS_BTN2_MASK)
        {
            display_fillRect( DISPLAY_SIZE_ONE*DISPLAY_MANIP,  0,  DISPLAY_SIZE_ONE*DISPLAY_MANIP,  DISPLAY_SIZE_TWO,  DISPLAY_BLACK);
            btn2_off = true;
        }

    }

    display_fillScreen(DISPLAY_BLACK); //If all buttons

}
