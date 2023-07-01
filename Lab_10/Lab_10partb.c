// Introduction to Electrical Engineering at The University of Texas at Arlington

#include <msp430g2553.h>

unsigned int adcval = 0;						// Variable to store ADC value

void ConfigureADC(void)
{
	ADC10CTL1 = INCH_5 + ADC10SSEL_3;			// Set ADC to channel 5, select the SMCLK as the ADC clock
	ADC10CTL0 = ADC10ON + SREF_0 + ADC10SHT_2;	// Turn the ADC on, set VCC and VSS as reference, use 16 SMCLK clock cycles to sample the analog input
	ADC10AE0 |= BIT5;							// Set the ADC to read from P1.5
}


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;					// Stop watchdog timer

	DCOCTL = CALDCO_1MHZ;						// DCO = 1MHZ

    P1DIR = 0x07;								// Set P1.0, P1.1 and P1.2 as outputs
    P1SEL |= 0x20;								// Set P1.5 as ADC input (the PxSEL commands select peripheral functions for the pin)

    ConfigureADC();								// Call ConfigureADC subroutine

    while(1)									// Infinite loop
    {
    	__delay_cycles(10000);					// Delay 10000 clock cycle to allow ADC to settle
    	ADC10CTL0 |= ENC + ADC10SC;				// Enable conversion and start conversion
    	adcval = ADC10MEM;						// Store ADC sampled value

    	if (adcval < 380)						// Based on sensor input to ADC, determine which pin to turn on. These values were determined
    	{										// by calibrating the sensor prior to the lab. P1.0 = force < 1/2 lb, P1.1 = 1/2 lb. < force < 1 lb.
    		if (adcval < 205)					// P1.2 = force > 1 lb.
    		{
    		P1OUT = 0x04;
    		}
    		else
    		{
    			P1OUT = 0x02;
    		}
    	}
    	else
    	{
    		P1OUT = 0x01;
    	}

    }

}
