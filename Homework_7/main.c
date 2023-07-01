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

Homework 7 – Write a program for the MSP430G2 Launchpad that will use the two built in LED’s as a
two bit counter. The counter must increment by one every time that the S2 button is pressed.
 The program is expected to run on the MSP430G2 Launchpad
	o It could be made to run on any Launchpad that has two LED’s and one button
 The LSB of the counter must be LED2 (controlled by P1.6)
 The MSB of the counter must be LED1 (controlled by P1.0)
 The enable for the counter must be from the S2 button (P1.3)
 The program must provide an internal pull up resistor for the P1.3 input
	o On the G2 Launchpad the S2 button is connected from P1.3 to ground
	o And it may or may not have an external pull up resistor
 The counter should only increment once every time that the button is pressed
	o This is different from the demo in class where the counter continuously incremented as
	the button was held down.
 The program must poll the S2 button for its value (No interrupt service routine)


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

char readinput(void)
{
	char local = 0;
	if (!(P1IN & BIT3))
		//Knowing that P1IN holds logical values that are configured as inputs, this will return a nonzero value if the button is not
		//pushed (since P1IN is pulled up to a high/1 value the only time the if statement will execute is when P1.3/S2 button has an input of 1 i.e. it is pressed)
	{
		push_num ++; //the number of times the S2 button is pressed increments by one but does not continually do so since the if statement has a break in it
		local = 1;
		__delay_cycles(10000);//increasing the amount in the delay cycle makes a delayed reaction on the msp430 when the S2 button is pressed, anything lower than 10,000 yield an error

		while(!(P1IN & BIT3))	//while the S2 button is being pressed wait for it to be unpressed and then return the local value
				{
					//downside: this might not be a good idea considering that the s2 button could theoretically be pressed forever
					__delay_cycles(10000);
				} //this will stop the if statement from proceeding until the button is no longer being pressed

	}
	return local; //this function will return either a local value of 0 or 1 depending on whether the button is pushed
}


void countup(void)
{
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

int main(void) //lines 108-122 could be put into a separate init() function and then called back to in the main function
{
	char enable = 0; //stores the status of the S2 button









	WDTCTL = WDTPW | WDTHOLD;	// Stops watchdog timer, a default for starting new ccs projects
	P1DIR |= outputmasks; //will simultaneously set P1.0 and P1.6 as outputs by logically "OR"ing the macro with the P1 direction i.e. enabaling these macros by moving them to a value of 1, since the macro is defined as the addition of the two

	/*We can clear P1.3 by using: P1DIR &= ~(BIT3);
	whereby logically "NAND"ing ("clearing") P1 direction with BIT3
	will set P1.3 it to an input, but that isn't neccessary
	since P1.3 is already an input by default*/

	P1REN |= BIT3; /*Here the P1 RESITOR ENABLE is logically "OR"ed with P1.3 to enable it's internal resistor
	which may not be neccessary if P1.3 already has an external resistor on the MSP430 (the MSP that is being used does not but it won't affect an MSP430 that does)*/
	//Keep in mind BIT3 has a decimal mask value of 8 (in binary that is place 3(as in 2^(3) = 8)
	P1OUT |= BIT3; //This part of the code logically "OR"s the Port 1 Ouput with P1.3 in order to make the internal resistor pullup (to presumably make it a pull down we could use &= ~ between the two macros)
	cursor = 0; //the current state is initialized to 0
	push_num = 0; //the number of pushes is initialized to 0 at the start of the code
	P1OUT &= ~(outputmasks); //will clear the LED displays to 00 at the start of the code

	while(1)
	{
		enable = readinput();//i.e. this will depend on the value returned for variable "local" from the readinput function

		if(1)
		{
			switch(enable)
			{
			case 0:
				break;
			case 1: //if the S2 button is pressed run through the countup function which controls each LED
				countup();
				__delay_cycles(800000);
				break;
			}
		}//end of first if statement
//The next part of the code seems redundant since the second if loop will never run
		//if(0)
		//{
		//	if(enable)
		//		{
		//		countup();
		//		__delay_cycles(800000);
		//		}
		//}//end of second if statement
	}
	
}
