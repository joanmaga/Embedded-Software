/*
 * clockDisplay.c
 *
 *  Created on: Sep 29, 2017
 *      Author: joanmaga
 */

#include "supportFiles/display.h"   //Use the Display Functions
#include "supportFiles/utils.h"     //Use it to add delay to some functions
#include <stdio.h>


#define TEXT_SIZE 6     //Modifiable Text Size to change text Size of the Display
#define SIZE_THREE 3    //Text Size 3
#define SIZE_FOUR 4     //Text Size 4
#define SIZE_FIVE 5     //Text Size 5
#define SIZE_SIX 6      //Text Size 6

#define OFFSET_HOUR_THREE 96    //Position of Hour with Text size 3
#define OFFSET_HOUR_FOUR 78     //Position of Hour with Text size 4
#define OFFSET_HOUR_FIVE 66     //Position of Hour with Text size 5
#define OFFSET_HOUR_SIX 48      //Position of Hour with Text size 6

#define COLON_ONE_THREE 128     //Position of First Colon with Text size 3
#define COLON_ONE_FOUR 119      //Position of First Colon with Text size 4
#define COLON_ONE_FIVE 113      //Position of First Colon with Text size 5
#define COLON_ONE_SIX 104       //Position of First Colon with Text size 6

#define OFFSET_MIN_THREE 142    //Position of Minute with Text size 3
#define OFFSET_MIN_FOUR 136     //Position of Minute with Text size 4
#define OFFSET_MIN_FIVE 130     //Position of Minute with Text size 5
#define OFFSET_MIN_SIX 124      //Position of Minute with Text size 6

#define COLON_TWO_THREE 174     //Position of Second Colon with Text size 3
#define COLON_TWO_FOUR 177      //Position of Second Colon with Text size 4
#define COLON_TWO_FIVE 177      //Position of Second Colon with Text size 5
#define COLON_TWO_SIX 180       //Position of Second Colon with Text size 6

#define OFFSET_SEC_THREE 188    //Position of Second with Text size 3
#define OFFSET_SEC_FOUR 194     //Position of Second with Text size 4
#define OFFSET_SEC_FIVE 194     //Position of Second with Text size 5
#define OFFSET_SEC_SIX 200      //Position of Second with Text size 6

#define OFFSET_HEIGHT_THREE 108 //Height of Text with Text size 3
#define OFFSET_HEIGHT_FOUR 104  //Height of Text with Text size 4
#define OFFSET_HEIGHT_FIVE 100  //Height of Text with Text size 5
#define OFFSET_HEIGHT_SIX 96    //Height of Text with Text size 6

#define OFFSET_CHAR_THREE 18    //Offest to get to the position of the Second Digit of Hour, Minute, or Second for Text Size 3
#define OFFSET_CHAR_FOUR 24     //Offest to get to the position of the Second Digit of Hour, Minute, or Second for Text Size 4
#define OFFSET_CHAR_FIVE 30     //Offest to get to the position of the Second Digit of Hour, Minute, or Second for Text Size 5
#define OFFSET_CHAR_SIX 36      //Offest to get to the position of the Second Digit of Hour, Minute, or Second for Text Size 6

//Position of Each Vertices of each Triangle that will perform Increment and Decrement
//Starts from left to right for the x-axis, and from top to bottom for the y-axis
#define X1_SIX 58   //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X2_SIX 83   //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X3_SIX 108  //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X4_SIX 134  //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X5_SIX 159  //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X6_SIX 184  //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X7_SIX 210  //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X8_SIX 235  //Horizontal Position of one of the Vertices of the triangles for text size 6
#define X9_SIX 260  //Horizontal Position of one of the Vertices of the triangles for text size 6
#define Y1_SIX 51   //Vertical Position of one of the Vertices of the triangles for text size 6
#define Y2_SIX 81   //Vertical Position of one of the Vertices of the triangles for text size 6
#define Y3_SIX 154  //Vertical Position of one of the Vertices of the triangles for text size 6
#define Y4_SIX 184  //Vertical Position of one of the Vertices of the triangles for text size 6

#define X1_FIVE 74  //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X2_FIVE 94  //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X3_FIVE 114 //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X4_FIVE 138 //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X5_FIVE 158 //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X6_FIVE 178 //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X7_FIVE 202 //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X8_FIVE 222 //Horizontal Position of one of the Vertices of the triangles for text size 5
#define X9_FIVE 242 //Horizontal Position of one of the Vertices of the triangles for text size 5
#define Y1_FIVE 61  //Vertical Position of one of the Vertices of the triangles for text size 5
#define Y2_FIVE 88  //Vertical Position of one of the Vertices of the triangles for text size 5
#define Y3_FIVE 150 //Vertical Position of one of the Vertices of the triangles for text size 5
#define Y4_FIVE 177 //Vertical Position of one of the Vertices of the triangles for text size 5

#define X1_FOUR 84  //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X2_FOUR 99  //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X3_FOUR 114 //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X4_FOUR 142 //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X5_FOUR 157 //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X6_FOUR 172 //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X7_FOUR 200 //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X8_FOUR 215 //Horizontal Position of one of the Vertices of the triangles for text size 4
#define X9_FOUR 230 //Horizontal Position of one of the Vertices of the triangles for text size 4
#define Y1_FOUR 71  //Vertical Position of one of the Vertices of the triangles for text size 4
#define Y2_FOUR 94  //Vertical Position of one of the Vertices of the triangles for text size 4
#define Y3_FOUR 145 //Vertical Position of one of the Vertices of the triangles for text size 4
#define Y4_FOUR 168 //Vertical Position of one of the Vertices of the triangles for text size 4

#define X1_THREE 100    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X2_THREE 112    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X3_THREE 124    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X4_THREE 146    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X5_THREE 158    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X6_THREE 170    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X7_THREE 192    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X8_THREE 204    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define X9_THREE 216    //Horizontal Position of one of the Vertices of the triangles for text size 3
#define Y1_THREE 80     //Vertical Position of one of the Vertices of the triangles for text size 3
#define Y2_THREE 100    //Vertical Position of one of the Vertices of the triangles for text size 3
#define Y3_THREE 137    //Vertical Position of one of the Vertices of the triangles for text size 3
#define Y4_THREE 157    //Vertical Position of one of the Vertices of the triangles for text size 3

#define MAX_HOUR 12 //Max value of Hour
#define PREV_MAX_HOUR 11    //Value previous to max hour, used for conditional statements
#define MAX_MIN 59  //Max value of minute
#define PREV_MAX_MIN 58     //Value previous to max minute, used for conditional statements
#define MAX_SEC 59  //Max Value of Second
#define PREV_MAX_SEC 58 //Value previous to max second, used for conditional statements
#define ROLL_HOUR_DOUBLE 9 //Rolls the hour to double digits
#define ROLL_HOUR_SINGLE 10 //Rolls the hour to Single digits

#define MIN_HOUR 1  //Minimum Value of Hour
#define MIN_MIN 0   //Minimum Value of Minute
#define MIN_MIN_NEXT 1 //Next Minute after minimum Minute
#define MIN_SEC 0   //Minimum Value of Second
#define MIN_SEC_NEXT 1 //Next Second after minimum Second

#define HOUR_DOUBLE_DIGITS 10 //Helper Constant to check if there are two Digits in Hour
#define MIN_DOUBLE_DIGITS 10  //Helper Constant to check if there are two Digits in Minute
#define SEC_DOUBLE_DIGITS 10  //Helper Constant to check if there are two Digits in Second

#define Y_MIDDLE 120    //Vertical value for the Half size of the screen

#define FALSE false //Bool used in Update TIme Display FUnction

#define CHAR_MAX 4  //Maximum Value for Char used when incrementing or decrementing the Time

#define DELAY_1 100 //First Delay used in Run Test
#define DELAY_2 500 //Second Delay used in Run Test
#define TEST_SEC 50 //Used to define the Initial value of Seconds

int16_t x, y; //Parameters of the Touch Screen
uint8_t z;  //Pressure of Touch Screen

int8_t textSize = 0;    //Initialize Text Size Variable


int32_t hourCursor = 0, minCursor = 0, secCursor = 0;   //Initialize Time Cursors
int32_t colonHourMinCursor = 0, colonMinSecCursor = 0;  //Initialize Colon Cursors
int32_t xOne = 0, xTwo = 0, xThree = 0, xFour = 0, xFive = 0, xSix = 0, xSev = 0, xEight = 0, xNine = 0;    //Initialize Horizontal Cursors for Triangles
int32_t yOne = 0, yTwo = 0, yThree = 0, yFour = 0;  //Initialize Vertical Cursors for Triangles

int32_t offset_height = 0;  //Offset of Height of Text
int32_t char_size = 0;  //Char Sise used to increment and decrement the Time

int32_t hourCur = MAX_HOUR, minCur = PREV_MAX_MIN, secCur = TEST_SEC; //Current Value of time
int32_t hourPrev = MAX_HOUR, minPrev = PREV_MAX_MIN, secPrev = TEST_SEC; //Previous Value of Time


void formatCall(int8_t textSize)    //Format Text and Triangles based on TEXT_SIZE Constant
{
    if(textSize == SIZE_THREE)  //For Size 3
    {
        hourCursor = OFFSET_HOUR_THREE, minCursor = OFFSET_MIN_THREE, secCursor = OFFSET_SEC_THREE; //Set Hour, minute, and second Offsets
        colonHourMinCursor = COLON_ONE_THREE, colonMinSecCursor = COLON_TWO_THREE;  //Set Colons Offsets
        xOne = X1_THREE, xTwo = X2_THREE, xThree = X3_THREE;    //Set Triangles horizontal Offsets
        xFour = X4_THREE, xFive = X5_THREE, xSix = X6_THREE;    //Set Triangles horizontal Offsets
        xSev = X7_THREE, xEight = X8_THREE, xNine = X9_THREE;    //Set Triangles horizontal Offsets
        yOne = Y1_THREE, yTwo = Y2_THREE, yThree = Y3_THREE, yFour = Y4_THREE;  //Set Triangles Vertical Offsets
        offset_height = OFFSET_HEIGHT_THREE;    //Set Height of TExt
        char_size = OFFSET_CHAR_THREE;  //Set offset for each char that is being skipped
    }
    else if(textSize == SIZE_FOUR) //For Size 4
    {
        hourCursor = OFFSET_HOUR_FOUR, minCursor = OFFSET_MIN_FOUR, secCursor = OFFSET_SEC_FOUR; //Set Hour, minute, and second Offsets
        colonHourMinCursor = COLON_ONE_FOUR, colonMinSecCursor = COLON_TWO_FOUR;  //Set Colons Offsets
        xOne = X1_FOUR, xTwo = X2_FOUR, xThree = X3_FOUR;    //Set Triangles horizontal Offsets
        xFour = X4_FOUR, xFive = X5_FOUR, xSix = X6_FOUR;    //Set Triangles horizontal Offsets
        xSev = X7_FOUR, xEight = X8_FOUR, xNine = X9_FOUR;    //Set Triangles horizontal Offsets
        yOne = Y1_FOUR, yTwo = Y2_FOUR, yThree = Y3_FOUR, yFour = Y4_FOUR;  //Set Triangles Vertical Offsets
        offset_height = OFFSET_HEIGHT_FOUR;    //Set Height of TExt
        char_size = OFFSET_CHAR_FOUR;  //Set offset for each char that is being skipped
    }
    else if(textSize == SIZE_FIVE) //For Size 5
    {
        hourCursor = OFFSET_HOUR_FIVE, minCursor = OFFSET_MIN_FIVE, secCursor = OFFSET_SEC_FIVE; //Set Hour, minute, and second Offsets
        colonHourMinCursor = COLON_ONE_FIVE, colonMinSecCursor = COLON_TWO_FIVE;  //Set Colons Offsets
        xOne = X1_FIVE, xTwo = X2_FIVE, xThree = X3_FIVE;    //Set Triangles horizontal Offsets
        xFour = X4_FIVE, xFive = X5_FIVE, xSix = X6_FIVE;    //Set Triangles horizontal Offsets
        xSev = X7_FIVE, xEight = X8_FIVE, xNine = X9_FIVE;    //Set Triangles horizontal Offsets
        yOne = Y1_FIVE, yTwo = Y2_FIVE, yThree = Y3_FIVE, yFour = Y4_FIVE;  //Set Triangles Vertical Offsets
        offset_height = OFFSET_HEIGHT_FIVE;    //Set Height of TExt
        char_size = OFFSET_CHAR_FIVE;  //Set offset for each char that is being skipped
    }
    else if(textSize == SIZE_SIX)   //For Size 6
    {
        hourCursor = OFFSET_HOUR_SIX, minCursor = OFFSET_MIN_SIX, secCursor = OFFSET_SEC_SIX; //Set Hour, minute, and second Offsets
        colonHourMinCursor = COLON_ONE_SIX, colonMinSecCursor = COLON_TWO_SIX;  //Set Colons Offsets
        xOne = X1_SIX, xTwo = X2_SIX, xThree = X3_SIX;    //Set Triangles horizontal Offsets
        xFour = X4_SIX, xFive = X5_SIX, xSix = X6_SIX;    //Set Triangles horizontal Offsets
        xSev = X7_SIX, xEight = X8_SIX, xNine = X9_SIX;    //Set Triangles horizontal Offsets
        yOne = Y1_SIX, yTwo = Y2_SIX, yThree = Y3_SIX, yFour = Y4_SIX;  //Set Triangles Vertical Offsets
        offset_height = OFFSET_HEIGHT_SIX;    //Set Height of TExt
        char_size = OFFSET_CHAR_SIX;  //Set offset for each char that is being skipped
    }

}

// Called only once - performs any necessary inits.
// This is a good place to draw the triangles and any other
// parts of the clock display that will never change.
void clockDisplay_init()
{
    display_init();  // Must init all of the software and underlying hardware for LCD.
    display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
    display_setTextColor(DISPLAY_GREEN);    //Declare Text Color
    formatCall(TEXT_SIZE);      // Define Text Size
    display_fillTriangle(xOne,yTwo,xTwo,yOne,xThree,yTwo,DISPLAY_GREEN);    //Display Hour Increment Triangle
    display_fillTriangle(xOne,yThree,xTwo,yFour,xThree,yThree,DISPLAY_GREEN);   //Display Hour Decrement Triangle

    display_fillTriangle(xFour,yTwo,xFive,yOne,xSix,yTwo,DISPLAY_GREEN);    //Display Minute Increment Triangle
    display_fillTriangle(xFour,yThree,xFive,yFour,xSix,yThree,DISPLAY_GREEN);   //Diaply Minute Decrement Triangle

    display_fillTriangle(xSev,yTwo,xEight,yOne,xNine,yTwo,DISPLAY_GREEN);   //DIsplay Second Increment TRiangle
    display_fillTriangle(xSev,yThree,xEight,yFour,xNine,yThree,DISPLAY_GREEN);  //Display Second Decrement Triangle

    display_setCursor(colonHourMinCursor, offset_height);   //Set Cursor for First Colon
    display_setTextColor(DISPLAY_GREEN);    //Declare Text Color
    display_setTextSize(TEXT_SIZE);                // Define Text Size
    display_println(":");   //Display First Colon

    display_setCursor(colonMinSecCursor, offset_height);    //Set Cursor for Second Colon
    display_setTextColor(DISPLAY_GREEN);    //Declare Text Color
    display_println(":");   //Display Second COlon

    //INITIALIZE HOUR MIN AND SEC
    display_setCursor(hourCursor, offset_height);
    display_println(hourCur);   //Print Initial Hour
    display_setCursor(minCursor, offset_height);
    display_println(minCur);    //Print Initial Minutes
    display_setCursor(secCursor, offset_height);
    display_println(secCur);    //Print Initial Seconds
}

void incHour()  //Function to Increment Hour
{
    if(hourCur == MAX_HOUR) //Rollover If-Statement
    {
        hourCur = MIN_HOUR; //Hour is set to minimum value
    }
    else
    {
        hourCur++;  //Increment Hour
    }
}

void decHour()  //Function to decrement Hour
{
    if(hourCur <= MIN_HOUR) //Rollover If-Statement
    {
        hourCur = MAX_HOUR; //Hour is set to Maximum Value
    }
    else
    {
        hourCur--;  //Decrement Hour
    }
}

void incMin()   //Function to increment minutes
{
    if(minCur == MAX_MIN)   //Rollover If-Statement
    {
        minCur = MIN_MIN;   //Minute is set to minimum value
    }
    else
    {
        minCur++;   //Increment Minute
    }
}

void decMin()   //Function to decrement Minutes
{
    if(minCur == MIN_MIN)   //Rollover If-Statement
    {
        minCur = MAX_MIN;   //Minute is set to Maximum Value
    }
    else
    {
         minCur--;  //Decrement Minute
    }
}

void incSec()   //Function to Increment Seconds
{
    if(secCur == MAX_SEC)   //Rollover If-Statement
    {
        secCur = MIN_SEC;   //Second is set to minimum value
    }
    else
    {
        secCur++;   //Increment Second
    }
}

void decSec()   //Function to Decrement Seconds
{
    if(secCur == MIN_SEC)   //Rollover If-Statement
    {
        secCur = MAX_SEC;   //Second is set to Maximum Value
    }
    else
    {
        secCur--;   //Decrement Second
    }
}

// Updates the time display with latest time, making sure to update only those digits that
// have changed since the last update.
// if forceUpdateAll is true, update all digits.
void clockDisplay_updateTimeDisplay(bool forceUpdateAll)
{
    formatCall(TEXT_SIZE);  //Call Function to use the Right Text Format
    if(hourCur != hourPrev)
    {
        if((hourCur + MIN_HOUR) == hourPrev || hourCur == MAX_HOUR){  //Dec State

            if(hourCur == ROLL_HOUR_DOUBLE || hourCur == MAX_HOUR) //update both digits
            {
                char currentHour[CHAR_MAX]; //Create char array for CurrentHour
                char previousHour[CHAR_MAX];    //Create char array for PreviousHour
                sprintf(currentHour, "%2hd", (int8_t)hourCur);  //Save Current TIme to CurrentHour Variable
                sprintf(previousHour, "%2hd", (int8_t)hourPrev);    //Save Previous TIme to previousHour Variable

                display_setCursor(hourCursor, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(previousHour);     //Clear old number by printing a black number over it
                display_setCursor(hourCursor, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(currentHour); //Display Current Time
                hourPrev = hourCur; //Set previous Time to Current Time
            }
            else if(hourCur == PREV_MAX_HOUR || hourCur == ROLL_HOUR_SINGLE) {  //Update Second Digit
               display_setCursor(hourCursor + char_size, offset_height);    //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_BLACK); //Color of Text is Black
               display_print(hourPrev - HOUR_DOUBLE_DIGITS); //Clear old number by printing a black number over it
               display_setCursor(hourCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
               display_print(hourCur - HOUR_DOUBLE_DIGITS); //Display Current Time
               hourPrev = hourCur; //Set previous Time to Current Time
            }
            else //Any other type of Update
            {
                display_setCursor(hourCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(hourPrev); //Clear old number by printing a black number over it
                display_setCursor(hourCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(hourCur); //Display Current Time
                hourPrev = hourCur; //Set previous Time to Current Time
            }
        }
        else
        {
            if(hourPrev == ROLL_HOUR_DOUBLE || hourPrev == MAX_HOUR) //update both digits
            {
                char currentHour[CHAR_MAX];  //Create char array for CurrentHour
                char previousHour[CHAR_MAX];//Create char array for PreviousHour
                sprintf(currentHour, "%2hd", (int8_t)hourCur);//Save Current TIme to CurrentHour Variable
                sprintf(previousHour, "%2hd", (int8_t)hourPrev);    //Save Previous TIme to previousHour Variable

                display_setCursor(hourCursor, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(previousHour); //Clear old number by printing a black number over it
                display_setCursor(hourCursor, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(currentHour); //Display Current Time
                hourPrev = hourCur; //Set previous Time to Current Time
            }
            else if(hourCur == PREV_MAX_HOUR || hourCur == ROLL_HOUR_SINGLE) {
               display_setCursor(hourCursor + char_size, offset_height);    //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_BLACK); //Color of Text is Black
               display_print(hourPrev - HOUR_DOUBLE_DIGITS); //Clear old number by printing a black number over it
               display_setCursor(hourCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
               display_print(hourCur - HOUR_DOUBLE_DIGITS); //Display Current Time
               hourPrev = hourCur; //Set previous Time to Current Time
            }
            else //update
            {
                display_setCursor(hourCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(hourPrev); //Clear old number by printing a black number over it
                display_setCursor(hourCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(hourCur); //Display Current Time
                hourPrev = hourCur; //Set previous Time to Current Time
            }
        }
    }

    else if(minCur != minPrev)
    {

        if((minCur + MIN_MIN_NEXT) == minPrev || minCur == MAX_MIN){  //Dec State



            if(!(minPrev % MIN_DOUBLE_DIGITS)) //update both digits
            {
                char currentMin[CHAR_MAX];  //Create char array for CurrentMin
                char previousMin[CHAR_MAX]; //Create char array for PReviousMin
                sprintf(currentMin, "%02hd", (int16_t)minCur);//Save Current TIme to CurrentMin Variable
                sprintf(previousMin, "%02hd", (int16_t)minPrev);    //Save Previous TIme to previousMin Variable

                display_setCursor(minCursor, offset_height);    //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(previousMin); //Clear old number by printing a black number over it
                display_setCursor(minCursor, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(currentMin); //Display Current Time
                minPrev = minCur; //Set previous Time to Current Time
            }
            else //update
            {
                display_setCursor(minCursor + char_size, offset_height);    //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(minPrev % MIN_DOUBLE_DIGITS); //Clear old number by printing a black number over it
                display_setCursor(minCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(minCur % MIN_DOUBLE_DIGITS); //Display Current Time
                minPrev = minCur; //Set previous Time to Current Time
            }
        }
        else
        {
            if(!(minCur % MIN_DOUBLE_DIGITS)) //update both digits
            {
                char currentMin[CHAR_MAX];  //Create char array for CurrentMin
                char previousMin[CHAR_MAX]; //Create char array for PreviousMin
                sprintf(currentMin, "%02hd", (int16_t)minCur);//Save Current TIme to CurrentMin Variable
                sprintf(previousMin, "%02hd", (int16_t)minPrev);    //Save Previous TIme to previousMin Variable

                display_setCursor(minCursor, offset_height);    //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(previousMin); //Clear old number by printing a black number over it
                display_setCursor(minCursor, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(currentMin); //Display Current Time
                minPrev = minCur; //Set previous Time to Current Time
            }
            else //update
            {
                display_setCursor(minCursor + char_size, offset_height);    //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_BLACK);    //Color of Text is Black
                display_print(minPrev % MIN_DOUBLE_DIGITS); //Clear old number by printing a black number over it
                display_setCursor(minCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
                display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
                display_print(minCur % MIN_DOUBLE_DIGITS); //Display Current Time
                minPrev = minCur; //Set previous Time to Current Time
            }
        }
    }

    else if(secCur != secPrev)
    {
        if((secCur + MIN_SEC_NEXT) == secPrev || secCur == MAX_SEC){  //Dec State

           if(!(secPrev % SEC_DOUBLE_DIGITS)) //update both digits
           {
               printf("secCur %ld \n", secCur);
               char currentSec[CHAR_MAX];   //Create char array for CurrentSec
               char previousSec[CHAR_MAX];  //Create char array for PReviousMin
               sprintf(currentSec, "%02hd", (int16_t)secCur);//Save Current TIme to CurrentSec Variable
               sprintf(previousSec, "%02hd", (int16_t)secPrev); //Save Previous TIme to previousSec Variable

               display_setCursor(secCursor, offset_height); //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_BLACK); //Color of Text is Black
               display_print(previousSec); //Clear old number by printing a black number over it
               display_setCursor(secCursor, offset_height);   //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
               display_print(currentSec); //Display Current Time
               secPrev = secCur; //Set previous Time to Current Time
           }
           else //update
           {
               display_setCursor(secCursor + char_size, offset_height); //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_BLACK); //Color of Text is Black
               display_print(secPrev % SEC_DOUBLE_DIGITS); //Clear old number by printing a black number over it
               display_setCursor(secCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
               display_print(secCur % SEC_DOUBLE_DIGITS); //Display Current Time
               secPrev = secCur; //Set previous Time to Current Time
           }
       }
       else
       {
           if(!(secCur % SEC_DOUBLE_DIGITS)) //update both digits
           {
               char currentSec[CHAR_MAX];
               char previousSec[CHAR_MAX];
               sprintf(currentSec, "%02hd", (int16_t)secCur);   //Save Current TIme to CurrentSec Variable
               sprintf(previousSec, "%02hd", (int16_t)secPrev); //Save Previous TIme to previousSec Variable

               display_setCursor(secCursor, offset_height); //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_BLACK); //Color of Text is Black
               display_print(previousSec); //Clear old number by printing a black number over it
               display_setCursor(secCursor, offset_height);   //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
               display_print(currentSec); //Display Current Time
               secPrev = secCur; //Set previous Time to Current Time
           }
           else //update
           {
               display_setCursor(secCursor + char_size, offset_height); //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_BLACK); //Color of Text is Black
               display_print(secPrev % SEC_DOUBLE_DIGITS); //Clear old number by printing a black number over it
               display_setCursor(secCursor + char_size, offset_height);   //Set Cursor at position of number that will be changed
               display_setTextColor(DISPLAY_GREEN);    //Color of Text is Green
               display_print(secCur % SEC_DOUBLE_DIGITS); //Display Current Time
               secPrev = secCur; //Set previous Time to Current Time
           }
       }
    }
}

// Reads the touched coordinates and performs the increment or decrement,
// depending upon the touched region.
void clockDisplay_performIncDec()
{
    formatCall(TEXT_SIZE);  //Call Function to use the Right Text Format

        display_getTouchedPoint(&x,&y,&z);  //Get Position of Touch
        if(x < colonHourMinCursor)  //First horizontal third of Screen was touched
        {
            if(y < Y_MIDDLE)    //Upper Area of Screen was touched
            {
                incHour();  //Increment Hour
                clockDisplay_updateTimeDisplay(FALSE);  //Display The incremented Value
            }
            else    //Lower Area of Screen was touched
            {
                decHour();  //Decrement Hour
                clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
            }

        }
        else if(x < colonMinSecCursor && x > colonHourMinCursor) //Second horizontal third of Screen was touched
        {
            if(y < Y_MIDDLE)    //Upper Area of Screen was touched
            {
                incMin();   //Increment Minute
                clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
            }
            else    //Lower Area of Screen was touched
            {
                decMin();   //Decrement Minute
                clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
            }

        }
        else    //Third horizontal third of Screen was touched
        {
            if(y < Y_MIDDLE)    //Upper Area of Screen was touched
            {
                incSec();   //Increment Second
                clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
            }
            else    //Lower Area of Screen was touched
            {
                decSec();   //Decrement Second
                clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
            }

        }
}

// Advances the time forward by 1 second and update the display.
void clockDisplay_advanceTimeOneSecond()
{
    incSec();   //Increment One Sec
    clockDisplay_updateTimeDisplay(FALSE);  //Display The incremented Second
    if(secCur == MIN_SEC){  //If seconds is rolled over
        incMin();   //Increment Minutes
        clockDisplay_updateTimeDisplay(FALSE);  //Display The incremented Minute
        if(minCur == MIN_MIN){    //If minutes is rolled over
            incHour();  //Increment Hours
            clockDisplay_updateTimeDisplay(FALSE);  //Display The incremented Hour
        }
    }
}


// Run a test of clock-display functions.
void clockDisplay_runTest()
{
    clockDisplay_init();    //Initialize the Clock Display


        incSec();   //Increment One Sec
        clockDisplay_updateTimeDisplay(FALSE);  //Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        incSec();   //Increment One Sec
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        incSec();   //Increment One Sec
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay

        decSec();   //Decrement One Sec
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        decSec(); //Decrement One Sec
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        decSec(); //Decrement One Sec
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay


        incMin();   //Increment One Min
       clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
       utils_msDelay(DELAY_2); //Add Delay
       incMin();   //Increment One Min
       clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
       utils_msDelay(DELAY_2); //Add Delay
       incMin();   //Increment One Min
       clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
       utils_msDelay(DELAY_2); //Add Delay

       decMin(); //Decrement One Min
       clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
       utils_msDelay(DELAY_2); //Add Delay
       decMin(); //Decrement One Min
       clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
       utils_msDelay(DELAY_2); //Add Delay
       decMin(); //Decrement One Min
       clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
       utils_msDelay(DELAY_2); //Add Delay


       incHour();   //Increment One Hour
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        incHour();   //Increment One Hour
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        incHour();   //Increment One Hour
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay

        decHour(); //Decrement One Hour
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        decHour(); //Decrement One Hour
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay
        decHour(); //Decrement One Hour
        clockDisplay_updateTimeDisplay(FALSE);//Display The incremented Value
        utils_msDelay(DELAY_2); //Add Delay

        for(int32_t i = 0; i < DELAY_1; i++) {  //For loop to Increment 10 Seconds per second
            clockDisplay_advanceTimeOneSecond();    //Display The incremented Value
            utils_msDelay(DELAY_1); //Add Delay
        }
}


