#include <msp430g2553.h>

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

	BCSCTL1 = CALBC1_1MHZ;									// Set ACLK divider to 1
	DCOCTL = CALDCO_1MHZ;									// SMCLK = DCO/1 = 1MHZ

	P1DIR |= 0x40;											// Set P1.6 as output
	P1OUT |= 0x00;											// Set P1.6 to low
	P1SEL |= 0x40;											// Select P1.6 for PWM output

	TACCR0 = 5000;											// PWM period
	TACCR1 = 3500;											// PWM duty cycle
	TACCTL1 = OUTMOD_7;										// PWM output mode: 7 - PWM reset/set, high below PWM duty cycle, low above
	TACCTL0 = CCIE;											// Enable counter interrupt
	TACTL = TASSEL_2 + ID_1 + MC_1;							// Use SMCLK for timer clock (1 MHz), set internal clock divider to 2, set continuous count up to TACCR0
	_BIS_SR(LPM0_bits + GIE);								// Enter low power mode, enable all interrupts

}

#pragma vector = TIMER0_A0_VECTOR							// Counter interrupt
__interrupt void Timer0_A0 (void)
{
	if(TACCR1 == 4500)										// Slowly increase PWM duty cycle, reset to 10% duty cycle at 80% duty cycle
	{
		TACCR1 = 500;
	}
	else
	{
		//TACCR1 = TACCR1 + 10;
	}

}
