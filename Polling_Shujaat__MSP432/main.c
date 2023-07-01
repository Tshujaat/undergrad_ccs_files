/*  MSP432 Binary Counter
    Copyright (C) <2017>  <Greg Turner>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

This code produces a counter using serial polling to detect
button pushes.  The code uses hard-coded wait states for switch debouncing

This program demonstrates a six state counter that uses LED1 and LED2


*/




#include "msp.h"

// Global variable used to hold the current state
char currentState;

// Global variable used to hold the number of pushes
long int pushes;


/*
 * Initialization function runs once before the state machine while(1) loop.
 */
void init(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;                 // Stop watchdog timer
    P1DIR |= (BIT0);                      // Set P1.0 to output direction
    P2DIR |= (BIT2 + BIT1 + BIT0);        // Set P2, bits 2, 1, 0 to output

                                            // P1 is set to input by default in the P1DIR reg
    P1DIR &= ~(BIT1 + BIT4);                // So clearing it here is redundant
                                            // Set Buttons to input direction

                                            // Setting the internal resistor to be a pullup
                                            // may be redundant if button has an external pullup
    P1REN |= (BIT1 + BIT4);                 // enable internal resistor
    P1OUT |= (BIT1 + BIT4);                 // make internal resistor pullup

    currentState = 0;                       // Global variable init
    pushes = 0;                             // Global variable init

    P1OUT &= ~(BIT0);                 // Set display initially to zero
    P2OUT &= ~(BIT2 + BIT1 + BIT0);
}

// Check S1 Button (counter reverse order)
char readS1Input(void)
{
    char local = 0;

    // The P1IN register holds the logical values of the pins that are
    // configured as inputs

    if (!(P1IN & BIT1)) {           // Check for S1 input
        pushes = pushes + 1;       // Code that executes when S1 button is pushed
        local = 1;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(!(P1IN & BIT1))   {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
        }
        __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
    }
    return local;
}


// Check S2 Button (counter forward order)
char readS2Input(void)
{
    char local = 0;

    // The P1IN registers hold the logical values of the pins that are
    // configured as inputs

    if (!(P1IN & BIT4)) {           // Check for S2 input
        pushes = pushes + 1;       // Code that executes when S2 button is pushed
        local = 2;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(!(P1IN & BIT4))   {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
        }
        __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
    }
    return local;
}


void countUp(void)
{
    switch (currentState){
    case 0 :
        currentState = 1;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT2 + BIT1);
        P2OUT |= (BIT0);
        break;
    case 1 :
        currentState = 2;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT2 + BIT0);
        P2OUT |= (BIT1);
        break;
    case 2 :
        currentState = 3;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT1 + BIT0);
        P2OUT |= (BIT2);
        break;
    case 3 :
        currentState = 4;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT2 + BIT1);
        P2OUT |= (BIT0);
        break;
    case 4 :
        currentState = 5;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT2 + BIT0);
        P2OUT |= (BIT1);
        break;
    case 5 :
        currentState = 0;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT1 + BIT0);
        P2OUT |= (BIT2);
        break;
    }
}

void countDown(void)
{
    switch (currentState){
    case 2 :
        currentState = 1;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT2 + BIT1);
        P2OUT |= (BIT0);
        break;
    case 3 :
        currentState = 2;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT2 + BIT0);
        P2OUT |= (BIT1);
        break;
    case 4 :
        currentState = 3;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT1 + BIT0);
        P2OUT |= (BIT2);
        break;
    case 5 :
        currentState = 4;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT2 + BIT1);
        P2OUT |= (BIT0);
        break;
    case 0 :
        currentState = 5;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT2 + BIT0);
        P2OUT |= (BIT1);
        break;
    case 1 :
        currentState = 0;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT1 + BIT0);
        P2OUT |= (BIT2);
        break;
    }
}



/**
 * main.c
 */
void main(void)
{
	//WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    char enable = 0;            // Holds status of the button
    init();                     // One time initialization function

    /*
     * The main while loop of the state machine will constantly scan the inputs
     * and update the system based on those inputs
     */
    while(1) {
        enable = readS1Input();   // Check the S1 button
        if(!enable)                   // If S1 button is not pressed then check S2
            enable = readS2Input();   // Check the S2 button

        switch(enable) {
        case 0 :
            // Do nothing if neither button is pressed
            break;
        case 1 :
            // If S1 button is pressed then allow the counter to increment once
            countDown();
            break;
        case 2 :
            // If S2 button is pressed then allow the counter to increment once
            countUp();
            break;
        }

    }
}
