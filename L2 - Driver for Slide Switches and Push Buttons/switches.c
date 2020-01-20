#include "switches.h"
#include "xparameters.h"
#include "supportFiles/leds.h"
#include "xil_io.h"

//Support files necessary to get the program running
#define printFailedStatusFlag true
#define GPIO_TRI_OFFSET 0x04
#define GPIO_TRI_INPUT 0x0F
#define GPIO_DATA_OFFSET 0x00
#define ALL_ON 0x0F //Value for all the switches on
#define ALL_OFF 0x00 //Value for all switches off

int32_t switches_readGpioRegister(int32_t offset) //Read a Register, the Offset
{
    return Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR + offset);
}
void switches_writeGpioRegister(int32_t offset, int32_t value) //Write to a Register, and pass which value I want to write to it
{
    Xil_Out32(XPAR_SLIDE_SWITCHES_BASEADDR + offset, value);
}

// Initializes the SWITCHES driver software and hardware. Returns one of the STATUS values defined above.
int32_t switches_init()
{
    leds_init(printFailedStatusFlag); //Initialize LEDS
    switches_writeGpioRegister(GPIO_TRI_OFFSET, GPIO_TRI_INPUT); //Writes to the GPIO_TRI, and make the switches input
    if(switches_readGpioRegister(GPIO_TRI_OFFSET) == GPIO_TRI_INPUT) //Check if Values are being passed correctly
    {
        return SWITCHES_INIT_STATUS_OK;     //The buttons were initalized correctly
    }
    else
    {
        return SWITCHES_INIT_STATUS_FAIL;   //The buttons were not initialized correctly
    }
}

// Returns the current value of all 4 switches as the lower 4 bits of the returned value.
// bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
int32_t switches_read()
{
    return switches_readGpioRegister(GPIO_DATA_OFFSET);
}

// Runs a test of the switches. As you slide the switches, LEDs directly above the switches will illuminate.
// The test will run until all switches are slid upwards. When all 4 slide switches are slid upward,
// this function will return.
void switches_runTest()
{
    switches_init();    // Initializes the SWITCHES driver software and hardware
    while(switches_read() != ALL_ON) //Run until all switches are on at the same time
    {
        leds_write(switches_read());    //Read Switch values and write to the LEDS
    }
    leds_write(ALL_OFF); //Turn all LEDS off
}
