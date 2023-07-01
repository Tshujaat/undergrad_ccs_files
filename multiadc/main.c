 /*******************************************************************************
 *
 *                       MSP432 CODE EXAMPLE DISCLAIMER
 *
 * MSP432 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see http://www.ti.com/tool/mspdriverlib for an API functional
 * library & https://dev.ti.com/pinmux/ for a GUI approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP432P401 Demo - ADC14, Repeated Sequence of Conversions
//
//  Description: This example shows how to perform a repeated sequence of
//  conversions using "repeat sequence-of-channels" mode.  AVcc is used for the
//  reference and repeated sequence of conversions is performed on Channels A0,
//  A1, A2, and A3. Each conversion result is stored in ADC14MEM0, ADC14MEM1,
//  ADC14MEM2, and ADC14MEM3 respectively. After each sequence, the 4 conversion
//  results are moved to A0results[], A1results[], A2results[], and A3results[].
//  Test by applying voltages to channels A0 - A3. Open a watch window in
//  debugger and view the results. Set Breakpoint1 in the index increment line
//  to see the array values change sequentially and Breakpoint2 to see the entire
//  array of conversion results in A0results[], A1results[], A2results[], and
//  A3results[]for the specified Num_of_Results.
//
//  Note that a sequence has no restrictions on which channels are converted.
//  For example, a valid sequence could be A0, A3, A2, A4, A2, A1, A0, and A7.
//  See the User's Guide for instructions on using the ADC14.
//
//               MSP432P401
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//    Vin0 -->|P5.5/A0          |
//    Vin1 -->|P5.4/A1          |
//    Vin2 -->|P5.1/A4          |
//    Vin3 -->|P5.0/A5          |
//            |                 |
//
//   Wei Zhao
//   Texas Instruments Inc.
//   June 2014
//   Built with Code Composer Studio V6.0
//******************************************************************************
#include "msp.h"
#include <stdint.h>

//#define   Num_of_Results   8

volatile uint16_t A0results;
volatile uint16_t A1results;
volatile uint16_t A4results;
volatile uint16_t A5results;

float a;
float b;
float c;
float d;

float r1c1;
float r2c1;
float r1c2;
float r2c2;

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  // Configure GPIO
  P5SEL1 |= BIT5 | BIT4 | BIT1 |BIT0;       // Enable A/D channel A0, A1, A4, A5
  P5SEL0 |= BIT5 | BIT4 | BIT1 |BIT0;

  __enable_interrupt();
  NVIC_ISER0 = 1 << ((INT_ADC14 - 16) & 31);// Enable ADC interrupt in NVIC module

  ADC14CTL0 = ADC14ON | ADC14MSC | ADC14SHT0__192 | ADC14SHP | ADC14CONSEQ_3; // Turn on ADC14, extend sampling time
                                                                              // to avoid overflow of results

  ADC14MCTL0 = ADC14INCH_0;                 // ref+=AVcc, channel = A0
  ADC14MCTL1 = ADC14INCH_1;                 // ref+=AVcc, channel = A1
  ADC14MCTL2 = ADC14INCH_4;                 // ref+=AVcc, channel = A4
  ADC14MCTL3 = ADC14INCH_5 + ADC14EOS;        // ref+=AVcc, channel = A5, end seq.
  ADC14IER0 = ADC14IE5;                     // Enable ADC14IFG.5

  SCB_SCR &= ~SCB_SCR_SLEEPONEXIT;          // Wake up on exit from ISR

  while(1)
  {
    ADC14CTL0 |= ADC14ENC | ADC14SC;        // Start conversion-software trigger
    printf("\n%f			%f\n%f			%f\n\n", r1c1, r1c2, r2c1, r2c2);
    __sleep();
    //__no_operation();                       // For debugger
  }

}

// ADC14 interrupt service routine
void ADC14IsrHandler(void)
{
    if (ADC14IFGR0 & ADC14IFG5)
    {
        A0results = ADC14MEM0;       // Move A0 results, IFG is cleared
        A1results = ADC14MEM1;       // Move A1 results, IFG is cleared
        A4results = ADC14MEM4;       // Move A4 results, IFG is cleared
        A5results = ADC14MEM5;       // Move A5 results, IFG is cleared

        a = (A1results)*(3.3/4095.0); //P5.4
        b = (A0results)*(3.3/4095.0); //P5.5
        c = (A4results)*(3.3/4095.0); //P5.1
        d = (A5results)*(3.3/4095.0); //P5.0

        r1c1 = c - a; //P5.1 - P5.4
        r2c1 = c - b; //P5.1 - P5.5
        r1c2 = d - a; //P5.0 - P5.4
        r2c2 = d - b; //P5.0 - P5.5
        //index = (index + 1) & 0x7;          // Increment results index, modulo
        //__no_operation();                   //Set Breakpoint1 here
    }

}
