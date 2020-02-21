/**********************
*Name: Lucas Buerkel and Jake Christy
*Course: EGR226-905 - Microcontroller Programming and Applications
*Project: Lab 06 - Interfacing Keypad with MSP432
*File: Lab 6 Part 1
*Description: This program will read in inputs from a keypad connected to 7 pins
*             in the MSP432. When pressing the buttons on the keypad, whatever is
*             pressed will be printed.
******************************************************************************/

#include "msp.h"
#include <stdio.h>
#include <stdlib.h>

void initialize();  //GPIO initialization
uint8_t Read_Keypad(); //keypad scan subroutine
void Print_Keys();   //print key pressed
void delayMs(int millis);

uint8_t num, pressed;


void main(void)
{     //beginning main loop
WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
initialize();

while(1){  //beginning while loop
  int pressed;
  pressed = Read_Keypad(); //reading function
   if(pressed)
       Print_Keys();
   delayMs(10); //10ms delay*/
} //ending while loop
} //ending main loop

/***| initialize() |************************************//*
*Brief:    initialization of keypad pins
*Params:   NA
*Returns:  NA
********************************************************/
void initialize(){
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F; //configure port 4 as GPIO for keypad
    P4->DIR &= ~0x7F; //make port 4 input
    P4->REN |= 0x7F;  //enable internal resistor on port 4
    P4->OUT |= 0x7F; //port 4 resistor is pull-up
}

/***| Read_Keypad() |************************************//*
*Brief:    function creates debounce for keypad buttons and
*          reads keys as rows and columns
*Params:   NA
*Returns:  1
********************************************************/

uint8_t Read_Keypad(){
    uint8_t col, row;
    for ( col = 0; col < 3; col++ )
    {
    P4->DIR = 0x00; // Set Columns to inputs
    P4->DIR |= BIT ( 4+col );// Set column 3 to output
    P4->OUT &=~ BIT ( 4+col );// Set column 3 to LOW

    delayMs(10); // Delay the while loop

    row = P4->IN & 0x0F; // read all rows

    while ( !(P4IN & BIT0) | !(P4IN & BIT1) | !( P4IN & BIT2) | !( P4IN & BIT3) );

    if (row != 0x0F) break; // if one of the input is low, some key is pressed.
    }

    P2->DIR = 0x00; // Set Columns to inputs
    if ( col == 3)
    return 0;
    if (row == 0x0E) num = col + 1;// key in row 0
    if (row == 0x0D) num = 3 + col + 1;// key in row 1
    if (row == 0x0B) num = 6 + col + 1;// key in row 2
    if (row == 0x07) num = 9 + col + 1;// key in row 3
    return 1;
}

/***| Print_Keys() |************************************//*
*Brief:    if statements that print the values of the
*          buttons as they're seen on the keypad
*Params:   NA
*Returns:  NA
********************************************************/
void Print_Keys(){
    if(num == 10)
        printf("*\n");
    else if(num == 11)
        printf("0\n");
    else if(num == 12)
        printf("#\n");
    else
        printf("%d\n", num);
}

/***| delayMs() |************************************//*
*Brief:    SysTick delay function
*Params:   int millis
*Returns:  NA
********************************************************/
void delayMs(int millis){
    int i; // Loop variable
    SysTick->LOAD = 3000-1; // Set load value to count down from
    SysTick->VAL = 0;       // Clear current register value
    SysTick->CTRL = 0x5;    // Enable the timer

    for(i = 0; i<millis; i++){  // Halt program for input milliseconds using SysTick
        while((SysTick->CTRL & 0x10000)==0); // Wait until COUNTFLAG is set
    }
    SysTick->CTRL = 0; // Stop the timer
}
