/* Copyright (C) <2017> <Taha Shujaat> <taha.shujaatt@mavs.uta.edu> <tahashujaat1997@gmail.com>

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License the terms of the General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied waranty of
   MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have recieved a copy of the GNU General Public License
   along with this program. If not, see <http:/www.gnu.org/licenses/>.
   */

/*

Homework 8 – Rework Homework 7, but use an interrupt service routine to detect the pressing of the
S2 button. See the following material for an explanation of the ISR.
 Resource Explorer project - Software Port Interrupt Service on P1.4 from LPM4
	o Use caution with this project because it does not de-bounce the switch
 http://www.ganssle.com/debouncing.htm
	o Good explanation of bouncing.
 https://bennthomsen.wordpress.com/engineering-toolbox/ti-msp430-launchpad/using-theswitches/
	o This is a good explanation of how to debounce the input using software in an ISR

*/

#include <msp430.h> //line 46 and 47 do not require a semicolon
#define outputmasks (BIT6 + BIT0) /*expanding the macros we see BIT0
has a decimal mask value of 1 (in binary that is place 0 as in 2^(0) = 1) and
BIT6 has a decimal mask value of 64 (in binary that is place 6(as in 2^(6) = 64)
*/
/* we must define the output as BIT6 before BIT0 so that the left most LED on the chip
serves as the most significant bit and the right most bit is the least significant bit*/
char cursor; //global variable store where the current location of the lit up LED is
char push_num; //global variable will counts the number of times the S2 button is pressed on the MSP430

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    IE1 &= ~WDTIE;                   // disable Watchdog timer (WDT) interrupt
    IFG1 &= ~WDTIFG;                 // clear WDT interrupt flag
    WDTCTL = WDTPW + WDTHOLD;        // put WDT back in hold state
    P1IE |= BIT3;                  // Reenable interrupts for the button
}

#pragma vector=PORT1_VECTOR //PORT1_VECTOR macro is defined as 0xFFE4. with port 1 having 8 different pins (P1.0-P1.7) we could configure each pin to have a button that can cause an interrupt service routine
__interrupt void PORT1_ISR(void) //normally we dont pass anything in or out of a service routine so it has void on each side of the function declaration
{

  P1IFG &= ~BIT3;                 // Clear the interrupt flag for the button, if we don't the interrupt flag is still set, thereby triggering another interrupt
  P1IES &= ~BIT3;                  // Disable Button interrupt
  WDTCTL = WDT_MDLY_8;             // Start and set watchdog timer (WDT) to trigger every 32ms
  IFG1 &= ~WDTIFG;                  // Clear the interrupt flag for the WDT (watchdog timer)
  IE1 |= WDTIE;                 // enable WDT interrupt
	switch(cursor) //keeping in mind that BIT6 and BIT0 start off as 00
	{
	case 0:
		cursor=1;//increments the cursor by one to be available in the next case
		P1OUT &= ~(BIT0); //brings this bit to a zero or turns it off
		P1OUT |= BIT6;//turns the least significant bit on to get a value of 0b01
		break;
	case 1:
		cursor=2;
		P1OUT |= BIT0;//sets msb to 1
		P1OUT &= ~(BIT6); //sets lsb to 0 to get a binary value of 0b10
		break;
	case 2:
		cursor=3;
		P1OUT |= BIT0;//sets msb to 1
		P1OUT |= BIT6;//sets lsb to 1 to get a binary value of 0b11
		break;
	case 3:
		cursor=0;//resets the state so it loops back up to the first case in this instance is case 0
		P1OUT &= ~(BIT0); //turns off both LEDS for an initialized value of 0b00
		P1OUT &= ~(BIT6);
		break;
	}
}

int main(void) //lines 85-96 could be put into a separate init() function and then called back to in the main function
{

	 WDTCTL = WDTPW + WDTHOLD;          // Stop WDT (WatchDogTimer)

	 P1DIR |= (BIT0+BIT6);              // P1.0 and P1.6 to outputs
	 P1OUT &= ~(BIT0+BIT6);              // Set the LEDs P1.0 and P1.6 to off
	 cursor = 0;
	 P1DIR &= ~BIT3;   // Set button pin as an input pin
	 P1OUT |= BIT3;    // Set pull up resistor on for button
	 P1REN |= BIT3;    // Enable pull up resistor for button to keep pin high until pressed
	 P1IE |= BIT3;    // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
	 //P1IFG is an 8 bit register, so when the button is pressed, the P1.3 voltage goes from vcc to ground, the P!IFG regiter at BIT3 will go to a 1, therby waking up the cpu and it reads the event that just happened at port 1
	 P1IFG &= ~BIT3;   // Clear the interrupt flag for the button i.e. keeps the code on standby until an event (in this case the pressing of the button) does happen, and if it does, process the code
	 P1IE |= BIT3;     // Enable interrupts on port 1 for the button
//this is the part of code in hw 7 that implements the while(1) loop:
	 _BIS_SR(LPM0_bits+GIE);        // Enter LPM0 with interrupts enabled i.e. it falls "asleep" while the button is not being pressed, therby consuming less power (LPM standing for Low Power Mode). It is a Macro NOT a function call, so the main function never actually returns (never leaves the while loop when the button is pressed)
	

}
