/*	EE2441 CountS2Button
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


This program is an implementation of the two bit counter example from
section 7.3 of the Wakerley text (State Machine Analysis).  In this analysis
example, it is discovered that the circuit is a two bit counter with an
enable input.

In this program the two bit output is connected to the P1.0 and P1.6
output of the MSP430 Launchpad.  So the lsb is on the green led of the
launchpad (P1.6) and the msb is on the red led (P1.0).

The enable input will come from the S2 button on the launchpad.  It is connected
to P1.3.  This button may be connected to a pull up resistor external to the
MSP430 (if it is then the internal pullup is optional.  The internal resistor
is explicitly set to be a pullup.

This version of the counter will count once when the enable button is pushed.
It will wait until the button is released before updating the LED's.
If the button is pressed and released too quickly it may not catch the push.
Missing the button push is a result of the poor software debouncing that is
implemented here.  A better way of debouncing the input would be to use interrupts.

An Additional feature of this code is that it counts the number of times that
you press the enable button and holds that value in a global variable.  This global
variable can be accessed during debug.

*/



#include <msp430.h>

#define OUTPUTMASK (BIT6 + BIT0)

// Global variable used to hold the current state
char currentState;

// Global variable used to hold the number of pushes
char pushes;

/*
 * Initialization function runs once before the state machine while(1) loop.
 */
void init(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	P1DIR |= OUTPUTMASK;                      // Set P1.0 and P1.6 to output direction


											// P1.3 is set to input by default in the P1DIR reg
	P1DIR &= ~(BIT3);						// So clearing it here is redundant


											// Setting the internal resistor to be a pullup
											// may be redundant if P1.3 has an external pullup
	P1REN |= BIT3;                            // P1.3 enable internal resistor
	P1OUT |= BIT3;                             // P1.3 make internal resistor pullup
	currentState = 0;						// Global variable init
	pushes = 0;                             // Global variable init
	P1OUT &= ~(OUTPUTMASK);					// Set display initially to zero
	                                        // Using a mask so that only the desired bits are affected
}


/*
 * readInput - This function reads the status of the enable button
 * 				and returns its value
 *              (S2 on the Launchpad connected to Port 1 Bit 3)
 *
 * 				returns 0 if the button is not pressed
 * 				returns 1 if the button is pressed
 *
 * 				It also waits for the button to be released so that only one
 * 				button push is registered
 */
char readInput(void)
{
	char local = 0;

	// The P1IN registers holds the logical values of the pins that are
	// configured as inputs
	if (!(P1IN & BIT3)) {			// Check for S2 input
		pushes = pushes + 1;								// Code that executes when S2 button is pushed
		local = 1;
	    __delay_cycles(10000);			// Wait for debounce to end (bad code)
		while(!(P1IN & BIT3))	{
			// Wait for Up button to return to "normal"
			// Must re-read the input port
			// Not good code because loop could run forever
			__delay_cycles(10000);		// Again wait for debounce to end (again bad code)
		}
	}

	return local;
}


/*
 * countUp - This function changes the state of the counter based on the current
 * 				state and then updates the value of currentState to the next state
 */
void countUp(void)
{
	switch (currentState){
	case 0 :
		currentState = 1;
		P1OUT |= BIT6;
		P1OUT &= ~(BIT0);
		break;
	case 1 :
		currentState = 2;
		P1OUT &= ~(BIT6);
		P1OUT |= BIT0;
		break;
	case 2 :
		currentState = 3;
		P1OUT |= BIT6;
		P1OUT |= BIT0;
		break;
	case 3 :
		currentState = 0;
		P1OUT &= ~(BIT6);
		P1OUT &= ~(BIT0);
		break;
	}
}

/*
 * main
 */
int main(void)
{

	char enable = 0;			// Holds status of the S2 button
	init();						// One time initialization function

	/*
	 * The main while loop of the state machine will constantly scan the inputs
	 * and update the system based on those inputs
	 */
	while(1) {
		enable = readInput();	// Check the S2 button

		/*
		 * The decision making in the state machine can be made with
		 * a switch statement or nested if statements
		 * Both implementations are given below.  Only one should be enabled at a time
		 * Simply make one and only one of the #if statements #if 1.
		 */
#if 1
		switch(enable) {
		case 0 :
			// Do nothing if the button is not pressed
			break;
		case 1 :
			// If the button is pressed then allow the counter to increment once
			countUp();
			__delay_cycles(800000);
			break;
		}
#endif

#if 0
		if(enable){
			countUp();
			__delay_cycles(800000);

		}
#endif

	}  // end while loop
}  // end main
