/*	EE2441 Serial Data Out Program
    Copyright (C) <2016>  <Greg Turner>

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
*/

// This app will shift out a byte of data to a connected shift register.
// The serial out bit (shift register input) is connected to MSP430 P1.0 (red LED)
// The clock for the shift register is connected to MSP430 P1.6 (green LED)
// This clock and data connection may be different from the required clock
// and data connections for the Spring 2016 Final Project

// This code example is functional however, this example is
// very poor as an example of "good code" with regard to formatting and
// general good coding efficiency.  We will go over these issues and
// demonstrate better code in class.


#include <msp430.h>

#define LS377CLOCK BIT6
#define MASKMSP430OUTS (BIT0 + BIT6)

// Uses P1.6 as the clock to the external D-Flip-Flop
 void ClockData(void)
 {
	P1OUT |= LS377CLOCK;							// Assert P1.6 (rising clock edge)
	__delay_cycles(1);								// Delay
	P1OUT &= ~LS377CLOCK;							// De-assert P1.6 (falling clock)
 }


 // Shift out the value of DataOut to the connected shift register
 void SerialDataOut(char DataOut)
{
	char i, lsb;							// i = for loop index
											// lsb = Value of the Least Significant Bit of DataOut
	for(i=0; i<=7; i++)
	{
		lsb = DataOut & 0x01;				// Set lsb to right most bit of DataOut

		// Place the LSB of DataOut on P1.0 and therefore to the input of the shift register
		if (lsb)
		{
			P1OUT |= 0x01;		// Assert P1.0 if the LSB of DataOut is 1
		}
		else
		{
			P1OUT &= ~0x01;		// De-assert P1.0 if the LSB of DataOut is 0
		}
		__delay_cycles(8);				// Delay

		// Clock the Data to the Shift Register
		ClockData();

		// Shift data left and repeat the for loop
		DataOut >>= 0x01;
	}
}


 /*
 * main.c
 */

int main(void) {
	char Data;						// Internal value that stores values
									// internal to the program and then
									// gets sent to shift register


    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

    P1DIR |= MASKMSP430OUTS;		// Set P1.0 and P1.6 as outputs



	while(1) {

	Data = 0x00;					// Clear Data

	SerialDataOut(Data);			// Shift Data out to LED's
	__delay_cycles(800000);				// Delay

	Data = 0x55;						// Shift Out the value 0x55
    SerialDataOut(Data);
	__delay_cycles(800000);

	Data = 0xaa;						// Shift Out the value 0xaa
    SerialDataOut(Data);
	__delay_cycles(800000);

	Data = 0x55;						// Shift Out the value 0x55
    SerialDataOut(Data);
	__delay_cycles(800000);



	}
}
