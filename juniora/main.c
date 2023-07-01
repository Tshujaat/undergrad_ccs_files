/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
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

int ADCvarC1;
double fADCvarC1;

int ADCvarC2;
double fADCvarC2;


char strR1C1[100];
char strR2C1[100];
char strR1C2[100];
char strR2C2[100];

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // Stop WDT

    // Configure GPIO
    P5SEL1 |= BIT0 | BIT1 | BIT4 | BIT5 | BIT6;  // Enable A/D channel P5.0, P5.1, P5.4, P5.5, P5.6
    P5SEL0 |= BIT0 | BIT1 | BIT4 | BIT5 | BIT6;

    // Configure ADC14
    ADC14->CTL0 = ADC14_CTL0_ON | ADC14_CTL0_MSC | ADC14_CTL0_SHP| ADC14_CTL0_SHT0__192 | ADC14_CTL0_CONSEQ_3;// Turn on ADC14, set sampling time
    ADC14->CTL1 = ADC14_CTL1_RES_2 ;                     // Use sampling timer, 12-bit conversion results
    ADC14->MCTL[0] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_0 ;   //A0/P5.5
    ADC14->MCTL[1] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_1 ;   //A1/P5.4
    ADC14->MCTL[4] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_4 ;   //A4/P5.1
    ADC14->MCTL[5] = ADC14_MCTLN_VRSEL_0  | ADC14_MCTLN_INCH_5 ;   //A5/P5.0
    ADC14->CTL0 |= ADC14_CTL0_ENC;                      // Enable conversions
    while (1)
    {
      ADC14->CTL0 |= ADC14_CTL0_SC;                     // Start conversion-software trigger
      //while (!(ADC14->IFGR0 & BIT0));


      ADCvarR1 = ADC14->MEM[1];                       // Read conversion result from row 1 aka A1/P5.4
      fADCvarR1 = (3.3*ADCvarR1)/(4095.0);

      ADCvarR2 = ADC14->MEM[0];                       // Read conversion result from row 2 aka A0/P5.5
      fADCvarR2 = (3.3*ADCvarR2)/(4095.0);
      //sprintf(strR2, "%f", fADCvarR2);
      //printf("%s\n", strR2);

      ADCvarC1 = ADC14->MEM[4];                       // Read conversion result from column 1 aka A4/P5.1
      fADCvarC1 = (3.3*ADCvarC1)/(4095.0);

      ADCvarC2 = ADC14->MEM[5];                       // Read conversion result from column 2 aka A5/P5.0
      fADCvarC2 = (3.3*ADCvarC2)/(4095.0);

      sprintf(strR1C1, "%f", fADCvarC1 - fADCvarR1);
      sprintf(strR2C1, "%f", fADCvarC1 - fADCvarR2);
      sprintf(strR1C2, "%f", fADCvarC2 - fADCvarR1);
      sprintf(strR2C2, "%f", fADCvarC2 - fADCvarR2);

      printf("\n|%s	%s|\n|%s	%s|\n", strR1C1, strR1C2, strR2C1, strR2C2);
    }

}


