#include "msp430.h"

#define BUTTON BIT3

char cursor; //global variable store where the current location of the lit up LED is
char push_num; //global variable will counts the number of times the S2 button is pressed on the MSP430

/* Function Prototype */
void InitializeButton(void);

/* Main function */
void main(void)
{
 WDTCTL = WDTPW + WDTHOLD;          // Stop WDT

 P1DIR |= (BIT0+BIT6);              // P1.0 and P1.6 to outputs
 P1OUT |= (BIT0+BIT6);              // Set the LEDs P1.0 and P1.6 to on

 InitializeButton();

 _BIS_SR(LPM0_bits+GIE);        // Enter LPM0 with interrupts enabled
}

/* This function configures the button so it will trigger interrupts
 * when pressed.  Those interrupts will be handled by PORT1_ISR() */
void InitializeButton(void)
{
    P1DIR &= ~BUTTON;   // Set button pin as an input pin
    P1OUT |= BUTTON;    // Set pull up resistor on for button
    P1REN |= BUTTON;    // Enable pull up resistor for button to keep pin high until pressed
    P1IES |= BUTTON;    // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
    P1IFG &= ~BUTTON;   // Clear the interrupt flag for the button
    P1IE |= BUTTON;     // Enable interrupts on port 1 for the button
}

/* Port 1 interrupt to service the button press */
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{

  P1IFG &= ~BUTTON;                 // Clear the interrupt flag for the button
  P1IE &= ~BUTTON;                  // Disable Button interrupt
  WDTCTL = WDT_MDLY_32;             // Start and set watchdog timer (WDT) to trigger every 32ms
  IFG1 &= ~WDTIFG;                  // Clear the interrupt flag for the WDT
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

// WDT Interrupt Service Routine used to de-bounce button press
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    IE1 &= ~WDTIE;                   // disable Watchdog timer (WDT) interrupt
    IFG1 &= ~WDTIFG;                 // clear WDT interrupt flag
    WDTCTL = WDTPW + WDTHOLD;        // put WDT back in hold state
    P1IE |= BUTTON;                  // Reenable interrupts for the button
}
