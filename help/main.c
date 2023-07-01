/*  MSP432 Homework 3 part b
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

This program demonstrates a seven state counter that uses LED2, buttons S1 and S2 using interrupts


*/
#include "msp.h"


/**
 * main.c
 */

// Global variable used to hold the current state
char currentState;

short Mode = 0;

void init(void)
{
//	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;

    P1DIR |= (BIT0);                                // Set the P1.0 pin to output direction
    P2DIR |= (BIT0 + BIT1 + BIT2);                  // Set the last three bits of P2 as outputs

    P1DIR &= ~(BIT1 + BIT4);                        // Set the P1.1 and P1.4 buttons to input direction

    P1REN |= (BIT1 + BIT4);                         // Enable internal resistors for P1.1 and P1.4 buttons
    P1OUT |= (BIT1 + BIT4);                         // Set internal resistors as pull up resistors

    P1IES |= (BIT1 + BIT4);                         // Enables  Falling Edge detection for P1.1 and P1.4
    P1IFG &= ~(BIT1 + BIT4);                        // Enables Interrupt flags for P1.1 and P1.4
    P1IE |= (BIT1 +BIT4);                           // Enables the interrupt detection for P1.1 and P1.4

    currentState = 0;
    Mode = 0;
    P1OUT &= ~(BIT0);
    P2OUT &= ~(BIT0 + BIT1 + BIT2);
}

void countUp(void)
{
    switch (currentState){
    case 0 :
        currentState = 1;
        P1OUT &= ~(BIT0); //this LED is turned off to indicate the count up button is being used (P1.0)
        P2OUT &= ~(BIT2 + BIT1);
        P2OUT |= (BIT0); //red
        break;
    case 1 :
        currentState = 2;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT2 + BIT0);
        P2OUT |= (BIT1); //green
        break;
    case 2 :
        currentState = 3;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT2);
        P2OUT |= (BIT0 + BIT1); //yellow is a combo of red and green
        break;
    case 3 :
        currentState = 4;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT1 + BIT0);
        P2OUT |= (BIT2); //blue
        break;
    case 4 :
        currentState = 5;

        P1OUT &= ~(BIT0);

        P2OUT &= ~(BIT1);
        P2OUT |= (BIT0 + BIT2); // pink/violet is a combination of blue and red
        break;
    case 5:
    	currentState = 6;
    	P1OUT &= ~(BIT0);

    	P2OUT &= ~(BIT0);
    	P2OUT |= (BIT1 + BIT2); //skyblue is a combination of green and blue
    	break;
    case 6:
    	currentState = 7;
    	P1OUT &= ~(BIT0);
    	P2OUT |= (BIT0 + BIT1 + BIT2);//white is a combination of RG and B
    	break;
    case 7 :
        currentState = 0;
        P1OUT &= ~(BIT0);
        P2OUT &= ~(BIT1 + BIT0+ BIT2);

        break;
    }
}

//basically the opposite of count up
void countDown(void)
{
    switch (currentState){
    case 2 :
        currentState = 1;
        P1OUT |= (BIT0); //this LED is turned on to indicate the count down button is being used
        P2OUT &= ~(BIT2 + BIT1);
        P2OUT |= (BIT0); //red
        break;
    case 3 :
        currentState = 2;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT2 + BIT0);
        P2OUT |= (BIT1); //green
        break;
    case 4 :
    	currentState = 3;
    	P1OUT |= (BIT0);
    	P2OUT &= ~(BIT2);
    	P2OUT |= (BIT0 + BIT1); //yellow is a combo of red and green
    	break;
    case 5 :
        currentState = 4;
        P1OUT |= (BIT0);
        P2OUT &= ~(BIT1 + BIT0);
        P2OUT |= (BIT2); //blue
        break;
    case 6:
    	currentState = 5;
    	P1OUT |= (BIT0);
    	P2OUT &= ~(BIT1);
    	P2OUT |= (BIT0 + BIT2); // pink/violet is a combination of blue and red
    	break;
    case 7:
    	currentState = 6;
    	P1OUT |= (BIT0);
    	P2OUT &= ~(BIT0);
    	P2OUT |= (BIT1 + BIT2); //skyblue is a combination of green and blue
    	break;

    case 0 :
    	currentState = 7;
    	P1OUT |= (BIT0);
    	P2OUT |= (BIT0 + BIT1 + BIT2);//white is a combination of RG and B
    	break;
    case 1 :
    	 currentState = 0;
    	 P1OUT |= (BIT0);
    	 P2OUT &= ~(BIT1 + BIT0+ BIT2); //resets LED display to nothing
    	 break;
    }
}

void PORT1_IRQHandler (void) //Keep in mind the user will have to go into the msp432_startup_ccs.c file underneath this folder and change the port 1 ISR from defaultISR to the function name

{


    P1IE &= ~(BIT1 + BIT4);
    __delay_cycles(10000);


    if(P1IFG & BIT4)
    {
        P1IES ^= BIT4;

        if (Mode == 0) //this first state is where we have both buttons look for a falling edge
        {
        	countDown();
        	Mode = 1;

        }
        else if (Mode == 1) // this second state is where the S1 button looks for the falling edge while S2 looks  for rising edge
        {

            Mode = 0;

        }
        else if (Mode == 2) //this thirdstate is the vice versa of the previous comment
        {
        	countDown(); //Counts down at the falling edge (i.e. if S2 is pressed or goes through ground, it will execute this code)
        	Mode = 3;
        }
        else
        {                  // State 3 the final state is where we have S1 and S2 looking for a rising edge

            Mode = 2;
        }
    }


    if (P1IFG & BIT1){
        P1IES ^= BIT1;

        if (Mode == 0) {
        	countUp(); //Counts up at the falling edge (i.e. if S1 is pressed or goes through ground, it will execute this code)
        	Mode = 2;
        }
        else if (Mode == 1) {
        	countUp();
        	Mode = 3;
        }
        else if (Mode == 2) {

            Mode = 0;

        }
        else {                  // State 3

            Mode = 1;

        }
    }



    P1IFG &= ~(BIT1 + BIT4);        // Clears the flags for interrupts so that it can
    P1IE |= (BIT1 + BIT4);          // reenable the S1 and S2 buttons for the interrupts
}


 void main(void)
 {
    //WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD; // for whatever reason CCS v6 uses this notation for stopping the watchdog timer instead of the one used above
    init();

    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);     // Enable Port 1 Interrupt Service Routine; can also be logically ORed with 0x

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;          // MSP is put to "sleep" until an event occurs

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;          // puts the MSP to "sleep" unntil the next interrupt is activated

    __DSB();                                      // wait for explanation during lecture

    __sleep();                                    // wait for explanation during lecture

}

