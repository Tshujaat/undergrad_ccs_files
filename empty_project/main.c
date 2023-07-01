/*
 * -------------------------------------------
 *    MSP432 DriverLib - v2_20_00_08 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>



/*  MSP432 Homework 4 Part 2
    Taha Shujaat (1001046728)

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

This code alternates LED 2 on the MSP432 P401R between colors red, green, yellow,
blue, pink, sky blue, and white

This program demonstrates a seven state counter that uses LED2, buttons S1 and S2 using driverlib functions


*/





// Global variable used to hold the current state
char currentState;

// Global variable used to hold the number of pushes
long int pushes;


/*
 * Initialization function runs once before the state machine while(1) loop.
 */
void init(void)
{
	WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
	GPIO_setAsOutputPin(GPIO_PORT_1, GPIO_PIN0); // Set P1.0 to output direction
    //P1DIR |= (BIT0);                      // Set P1.0 to output direction
    //P2DIR |= (BIT2 + BIT1 + BIT0);        // Set P2, bits 2, 1, 0 to output by logically ORing it with the port 2 directory (P2.0, P2.1, P2.2)
	GPIO_setAsOutputPin(GPIO_PORT_2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2));

	// P1 is set to input by default in the P1DIR reg by logically NANDing it with the port 1 directory
    //P1DIR &= ~(BIT1 + BIT4);                // So clearing it here is redundant
                                            // Set Buttons to input direction
    GPIO_setAsInputPin(GPIO_PORT_1, (GPIO_PIN1 + GPIO_PIN4));
                                            // Setting the internal resistor to be a pullup
                                            // may be redundant if button has an external pullup
    //P1REN |= (BIT1 + BIT4);                 // enable internal resistor for pins P1.1 and P1.4
    //P1OUT |= (BIT1 + BIT4);                 // make internal resistor pullup

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_1, (GPIO_PIN1 + GPIO_PIN4));

    currentState = 0;                       // Global variable init
    pushes = 0;                             // Global variable init

    //P1OUT &= ~(BIT0);                 // Set display initially to zero
    //P2OUT &= ~(BIT2 + BIT1 + BIT0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2));
}

// Check S1 Button (counter reverse order)
char readS1Input(void)
{
    char local = 0;

    // The P1IN register holds the logical values of the pins that are
    // configured as inputs

    if (/*!(P1IN & BIT1) becomes*/MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0) {           // Check for S1 input
        pushes = pushes + 1;       // Code that executes when S1 button is pushed
        local = 1;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0/*!(P1IN & BIT1)*/)   {
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

    if (MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0/*!(P1IN & BIT4)*/) {           // Check for S2 input
        pushes = pushes + 1;       // Code that executes when S2 button is pushed
        local = 2;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0/*!(P1IN & BIT4)*/)   {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
        }
        __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
    }
    return local;
}

//keep in mind that P2.0 pertains to the red LED, P2.1 to green, and P2.2 to blue
void countUp(void)
{
    switch (currentState){
    case 0 :
        currentState = 1;
        currentState = 1;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);//this LED is turned off to indicate the count up button is being used (P1.0)
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);//red
        //P1OUT &= ~(BIT0); //this LED is turned off to indicate the count up button is being used (P1.0)
        //P2OUT &= ~(BIT2 + BIT1);
        //P2OUT |= (BIT0); //red
        break;
    case 1 :
        currentState = 2;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
        //currentState = 2;
        //P1OUT &= ~(BIT0);
        //P2OUT &= ~(BIT2 + BIT0);
        //P2OUT |= (BIT1); //green
        break;
    case 2 :
    	currentState = 3;
    	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
    	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1));
        //currentState = 3;
        //P1OUT &= ~(BIT0);
        //P2OUT &= ~(BIT2);
        //P2OUT |= (BIT0 + BIT1); //yellow is a combo of red and green
        break;
    case 3 :
        currentState = 4;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
        //currentState = 4;
        //P1OUT &= ~(BIT0);
        //P2OUT &= ~(BIT1 + BIT0);
        //P2OUT |= (BIT2); //blue
        break;
    case 4 :
        currentState = 5;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2));
        //currentState = 5;

//        P1OUT &= ~(BIT0);
//
  //      P2OUT &= ~(BIT1);
    //    P2OUT |= (BIT0 + BIT2); // pink/violet is a combination of blue and red
        break;
    case 5:
        currentState = 6;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2));
    	//currentState = 6;
    	//P1OUT &= ~(BIT0);

    	//P2OUT &= ~(BIT0);
    	//P2OUT |= (BIT1 + BIT2); //skyblue is a combination of green and blue
    	break;
    case 6:
        currentState = 7;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2));
    	//currentState = 7;
    	//P1OUT &= ~(BIT0);
    	//P2OUT |= (BIT0 + BIT1 + BIT2);//white is a combination of RG and B
    	break;
    case 7 :
        currentState = 0;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2));
        //currentState = 0;
        //P1OUT &= ~(BIT0);
        //P2OUT &= ~(BIT1 + BIT0+ BIT2);

        break;
    }
}

//basically the opposite of count up
void countDown(void)
{
    switch (currentState){
    case 2 :
        currentState = 1;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
        //currentState = 1;
        //P1OUT |= (BIT0); //this LED is turned on to indicate the count down button is being used
        //P2OUT &= ~(BIT2 + BIT1);
        //P2OUT |= (BIT0); //red
        break;
    case 3 :
        currentState = 2;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
        //currentState = 2;
        //P1OUT |= (BIT0);
        //P2OUT &= ~(BIT2 + BIT0);
        //P2OUT |= (BIT1); //green
        break;
    case 4 :
        currentState = 3;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1));
    	//currentState = 3;
    	//P1OUT |= (BIT0);
    	//P2OUT &= ~(BIT2);
    	//P2OUT |= (BIT0 + BIT1); //yellow is a combo of red and green
    	break;
    case 5 :
        currentState = 4;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
       // currentState = 4;
       // P1OUT |= (BIT0);
       // P2OUT &= ~(BIT1 + BIT0);
       // P2OUT |= (BIT2); //blue
        break;
    case 6:
        currentState = 5;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2));
    	//currentState = 5;
    	//P1OUT |= (BIT0);
    	//P2OUT &= ~(BIT1);
    	//P2OUT |= (BIT0 + BIT2); // pink/violet is a combination of blue and red
    	break;
    case 7:
        currentState = 6;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2));
    	//currentState = 6;
    	//P1OUT |= (BIT0);
    	//P2OUT &= ~(BIT0);
    	//P2OUT |= (BIT1 + BIT2); //skyblue is a combination of green and blue
    	break;

    case 0 :
        currentState = 7;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2));
    	//currentState = 7;
    	//P1OUT |= (BIT0);
    	//P2OUT |= (BIT0 + BIT1 + BIT2);//white is a combination of RG and B
    	break;
    case 1 :
        currentState = 0;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2));
    	 //currentState = 0;
    	 //P1OUT |= (BIT0);
    	 //P2OUT &= ~(BIT1 + BIT0+ BIT2); //resets LED display to nothing
    	 break;
    }
}



/**
 * main.c
 */
void main(void)
{
	   /* Stop Watchdog  */
	   // MAP_WDT_A_holdTimer();



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
            // If S2 button is pressed then allow the counter to increment once
            countUp();
            break;
        case 2 :
            // If S1 button is pressed then allow the counter to increment once
            countDown();
            break;
        }

    }
}
