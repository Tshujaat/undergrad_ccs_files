
//******************************************************************************
//  MSP432P401 Demo - ADC14, Using an External Reference
//
//  Description: This example shows how to use an external positive reference for
//  the ADC14.The external reference is applied to the VeREF+ pin. AVss is used
//  for the negative reference. A single conversion is performed on channel A0.
//  The conversion results are stored in ADC14MEM0. Test by applying a voltage
//  to channel A0, then setting and running to a break point at the "_NOP()"
//  instruction. To view the conversion results, open an SFR window in debugger
//  and view the contents of ADC14MEM0 or from the variable ADCvar.
//  NOTE: VeREF+ range: 1.4V (min) to AVCC (max)
//        VeREF- range: 0V (min) to 1.2V (max)
//        Differential ref voltage range: 1.4V(min) to AVCC(max)
//          (see datasheet for device specific information)
//
//                MSP432P401
//             -------------------
//         /|\|                   |
//          | |                   |
//          --|RST                |
//            |                   |
//     Vin -->|P5.5/A0            |
//            |                   |
//     REF -->|P5.6/VREF+/VeREF+  |
//            |                   |
//
//   Wei Zhao
//   Texas Instruments Inc.
//   June 2014
//   Built with Code Composer Studio V6.0
//******************************************************************************
#include "msp.h"
#include <stdint.h>

int ADCvarR1;
double fADCvarR1;

int ADCvarR2;
double fADCvarR2;

int ADCvarR3;
double fADCvarR3;

int ADCvarR4;
double fADCvarR4;

int ADCvarR5;
double fADCvarR5;

int ADCvarR6;
double fADCvarR6;


int ADCvarC1;
double fADCvarC1;

int ADCvarC2;
double fADCvarC2;

int ADCvarC3;
double fADCvarC3;

int ADCvarC4;
double fADCvarC4;

int ADCvarC5;
double fADCvarC5;

int ADCvarC6;
double fADCvarC6;

char strR1C1[100];
char strR2C1[100];
char strR3C1[100];
char strR4C1[100];
char strR5C1[100];
char strR6C1[100];
char strR1C2[100];
char strR2C2[100];
char strR3C2[100];
char strR4C2[100];
char strR5C2[100];
char strR6C2[100];
char strR1C3[100];
char strR2C3[100];
char strR3C3[100];
char strR4C3[100];
char strR5C3[100];
char strR6C3[100];
char strR1C4[100];
char strR2C4[100];
char strR3C4[100];
char strR4C4[100];
char strR5C4[100];
char strR6C4[100];
char strR1C5[100];
char strR2C5[100];
char strR3C5[100];
char strR4C5[100];
char strR5C5[100];
char strR6C5[100];
char strR1C6[100];
char strR2C6[100];
char strR3C6[100];
char strR4C6[100];
char strR5C6[100];
char strR6C6[100];

int main(void)

{
    WDTCTL = WDTPW | WDTHOLD;                   // Stop WDT

    // Configure GPIO
    P4SEL1 |= BIT0 | BIT2 | BIT4 | BIT5 | BIT7 | BIT1;  // Enable A/D channel P4.0, P4.2, P4.4, P4.5, P4.7 and ref pin P4.1
    P4SEL0 |= BIT0 | BIT2 | BIT4 | BIT5 | BIT7 | BIT1;

    P5SEL1 |= BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6; // Enable A/D channel P5.3, ref pin P5.6
    P5SEL0 |= BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6;

    P6SEL1 |= BIT1 | BIT4; // Enable A/D channel P6.1, ref pin P6.4
    P6SEL0 |= BIT1 | BIT4;


    // Configure ADC14
    ADC14->CTL0 = ADC14_CTL0_ON | ADC14_CTL0_MSC | ADC14_CTL0_SHP| ADC14_CTL0_SHT0__192 | ADC14_CTL0_CONSEQ_3;// Turn on ADC14, set sampling time
    ADC14->CTL1 = ADC14_CTL1_RES_2 ;                     // Use sampling timer, 12-bit conversion results

    ADC14->MCTL[7] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_7 ;   //A7/P4.6/C1
    ADC14->MCTL[1] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_1 ;   //A1/P5.4/C2
    ADC14->MCTL[0] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_0 ;   //A0/P5.5/C3
    ADC14->MCTL[2] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_2 ;   //A2/P5.1/C4
    ADC14->MCTL[5] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_5 ;    //A5/P5.0/C5
    ADC14->MCTL[3] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_3 ;   //A3/P5.2/C6

    ADC14->MCTL[14] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_14 ;   //A14/P6.1/R1
    ADC14->MCTL[13] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_13 ;   //A13/P4.0/R2
    ADC14->MCTL[11] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_11 ;   //A11/P4.2/R3
    ADC14->MCTL[9]  = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_9 ;    //A9/P4.4/R4
    ADC14->MCTL[8]  = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_8 ;    //A8/P4.5/R5
    ADC14->MCTL[6]  = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_6 ;    //A6/P4.7/R6

    ADC14->CTL0 |= ADC14_CTL0_ENC;                      // Enable conversions
    while (1)
    {
      ADC14->CTL0 |= ADC14_CTL0_SC;                     // Start conversion-software trigger


      //ADC Row values and conversions for 2^(12) = 4095
      ADCvarR1 = ADC14->MEM[14];                       // Read conversion result from row 1 aka A14/P6.1
      fADCvarR1 = (3.3*ADCvarR1)/(4095.0);

      ADCvarR2 = ADC14->MEM[13];                       // Read conversion result from row 2 aka A13/P4.0
      fADCvarR2 = (3.3*ADCvarR2)/(4095.0);

      ADCvarR3 = ADC14->MEM[11];                       // Read conversion result from row 3 aka A11/P4.2
      fADCvarR3 = (3.3*ADCvarR3)/(4095.0);

      ADCvarR4 = ADC14->MEM[9];                        // Read conversion result from row 4 aka A9/P4.4
      fADCvarR4 = (3.3*ADCvarR4)/(4095.0);

      ADCvarR5 = ADC14->MEM[8];                       // Read conversion result from row 5 aka A8/P4.5
      fADCvarR5 = (3.3*ADCvarR5)/(4095.0);

      ADCvarR6 = ADC14->MEM[6];                       // Read conversion result from row 6 aka A6/P4.7
      fADCvarR6 = (3.3*ADCvarR6)/(4095.0);


      //ADC Column values and conversions for 2^(12) = 4095
      ADCvarC1 = ADC14->MEM[7];                       // Read conversion result from column 1 aka A7/P4.6
      fADCvarC1 = (3.3*ADCvarC1)/(4095.0);

      ADCvarC2 = ADC14->MEM[1];                       // Read conversion result from column 2 aka A1/P5.4
      fADCvarC2 = (3.3*ADCvarC2)/(4095.0);

      ADCvarC3 = ADC14->MEM[0];                       // Read conversion result from column 2 aka A0/P5.5
      fADCvarC3 = (3.3*ADCvarC3)/(4095.0);

      ADCvarC4 = ADC14->MEM[2];                       // Read conversion result from column 2 aka A2/P5.1
      fADCvarC4 = (3.3*ADCvarC4)/(4095.0);

      ADCvarC5 = ADC14->MEM[5];                       // Read conversion result from column 2 aka A5/P5.0
      fADCvarC5 = (3.3*ADCvarC5)/(4095.0);

      ADCvarC6 = ADC14->MEM[3];                       // Read conversion result from column 2 aka A3/P5.2
      fADCvarC6 = (3.3*ADCvarC6)/(4095.0);

      sprintf(strR1C1, "%f", fADCvarC1 - fADCvarR1);
      sprintf(strR1C2, "%f", fADCvarC2 - fADCvarR1);
      sprintf(strR1C3, "%f", fADCvarC3 - fADCvarR1);
      sprintf(strR1C4, "%f", fADCvarC4 - fADCvarR1);
      sprintf(strR1C5, "%f", fADCvarC5 - fADCvarR1);
      sprintf(strR1C6, "%f", fADCvarC6 - fADCvarR1);

      sprintf(strR2C1, "%f", fADCvarC1 - fADCvarR2);
      sprintf(strR2C2, "%f", fADCvarC2 - fADCvarR2);
      sprintf(strR2C3, "%f", fADCvarC3 - fADCvarR2);
      sprintf(strR2C4, "%f", fADCvarC4 - fADCvarR2);
      sprintf(strR2C5, "%f", fADCvarC5 - fADCvarR2);
      sprintf(strR2C6, "%f", fADCvarC6 - fADCvarR2);

      sprintf(strR3C1, "%f", fADCvarC1 - fADCvarR3);
      sprintf(strR3C2, "%f", fADCvarC2 - fADCvarR3);
      sprintf(strR3C3, "%f", fADCvarC3 - fADCvarR3);
      sprintf(strR3C4, "%f", fADCvarC4 - fADCvarR3);
      sprintf(strR3C5, "%f", fADCvarC5 - fADCvarR3);
      sprintf(strR3C6, "%f", fADCvarC6 - fADCvarR3);

      sprintf(strR4C1, "%f", fADCvarC1 - fADCvarR4);
      sprintf(strR4C2, "%f", fADCvarC2 - fADCvarR4);
      sprintf(strR4C3, "%f", fADCvarC3 - fADCvarR4);
      sprintf(strR4C4, "%f", fADCvarC4 - fADCvarR4);
      sprintf(strR4C5, "%f", fADCvarC5 - fADCvarR4);
      sprintf(strR4C6, "%f", fADCvarC6 - fADCvarR4);

      sprintf(strR5C1, "%f", fADCvarC1 - fADCvarR5);
      sprintf(strR5C2, "%f", fADCvarC2 - fADCvarR5);
      sprintf(strR5C3, "%f", fADCvarC3 - fADCvarR5);
      sprintf(strR5C4, "%f", fADCvarC4 - fADCvarR5);
      sprintf(strR5C5, "%f", fADCvarC5 - fADCvarR5);
      sprintf(strR5C6, "%f", fADCvarC6 - fADCvarR5);

      sprintf(strR6C1, "%f", fADCvarC1 - fADCvarR6);
      sprintf(strR6C2, "%f", fADCvarC2 - fADCvarR6);
      sprintf(strR6C3, "%f", fADCvarC3 - fADCvarR6);
      sprintf(strR6C4, "%f", fADCvarC4 - fADCvarR6);
      sprintf(strR6C5, "%f", fADCvarC5 - fADCvarR6);
      sprintf(strR6C6, "%f", fADCvarC6 - fADCvarR6);

      printf("\n|%s	%s	%s	%s	%s	%s|\n", strR1C1, strR1C2, strR1C3, strR1C4, strR1C5, strR1C6);
      printf("\n|%s	%s	%s	%s	%s	%s|\n", strR2C1, strR2C2, strR2C3, strR2C4, strR2C5, strR2C6);
      printf("\n|%s	%s	%s	%s	%s	%s|\n", strR3C1, strR3C2, strR3C3, strR3C4, strR3C5, strR3C6);
      printf("\n|%s	%s	%s	%s	%s	%s|\n", strR4C1, strR4C2, strR4C3, strR4C4, strR4C5, strR4C6);
      printf("\n|%s	%s	%s	%s	%s	%s|\n", strR5C1, strR5C2, strR5C3, strR5C4, strR5C5, strR5C6);
      printf("\n|%s	%s	%s	%s	%s	%s|\n", strR6C1, strR6C2, strR6C3, strR6C4, strR6C5, strR6C6);

    }

}
