//   June 2014
//   Built with Code Composer Studio V6.0
//******************************************************************************
#include "msp.h"
#include <stdint.h>

volatile uint16_t ADCvar;
float  a;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // Stop WDT

    // Configure GPIO
    P5SEL1 |= BIT1 | BIT6;                             // Enable A/D channel A0
    P5SEL0 |= BIT5 | BIT6;

    // Configure ADC14
    ADC14CTL0 = ADC14ON | ADC14SHP| ADC14SHT0_2;// Turn on ADC14, set sampling time
    ADC14CTL1 = ADC14RES_2;                     // Use sampling timer, 12-bit conversion results
    ADC14MCTL0 = ADC14VRSEL_14 | ADC14INCH_0;   // Vr+ = VeREF+ (ext) and Vr-=AVss, A0
    ADC14CTL0 |= ADC14ENC;                      // Enable conversions

    while (1)
    {
      ADC14CTL0 |= ADC14SC;                     // Start conversion-software trigger
      while (!(ADC14IFGR0 & BIT0));
      ADCvar = ADC14MEM0;                       // Read conversion result
      a = (ADCvar)*(3.3/4095.0); //P5.5
      printf("%d\n",ADCvar);
      printf("%f\n\n", a);

      //__no_operation();                         // SET BREAKPOINT HERE
    }
