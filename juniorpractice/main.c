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

volatile uint16_t ADCvar1;
volatile uint16_t ADCvar0;
volatile uint16_t ADCvar4;
volatile uint16_t ADCvar5;

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

    WDTCTL = WDTPW | WDTHOLD;                   // Stop WDT

    // Configure GPIO


    // Configure ADC14
    ADC14CTL0 = ADC14ON | ADC14SHP| ADC14SHT0_2;// Turn on ADC14, set sampling time
    ADC14CTL1 = ADC14RES_2;                     // Use sampling timer, 12-bit conversion results
    ADC14MCTL1 = ADC14VRSEL_14 | ADC14INCH_1;   // Vr+ = VeREF+ (ext) and Vr-=AVss, A1, memory control (32)
    ADC14MCTL0 = ADC14VRSEL_14 | ADC14INCH_0;
    ADC14MCTL4 = ADC14VRSEL_14 | ADC14INCH_4;
    ADC14MCTL5 = ADC14VRSEL_14 | ADC14INCH_5;
   // ADC14CTL0 |= ADC14ENC;                      // Enable conversions
    //ADC14CTL1 |= ADC14ENC;


    while (1)
    {
      ADC14CTL0 |= ADC14SC;                     // Start conversion-software trigger
      while ((ADC14IFGR0 & BIT1));
      P5SEL1 |= BIT4 | BIT6;                             // Enable A/D channel A1
      P5SEL0 |= BIT4 | BIT6;
      ADCvar1 = ADC14MEM1;
      a = (ADCvar1)*(3.3/4095.0); //P5.4


      while ((ADC14IFGR0 & BIT0));
      P5SEL1 |= BIT5 | BIT6;                             // Enable A/D channel A0
      P5SEL0 |= BIT5 | BIT6;
      ADCvar0 = ADC14MEM0;
      b = (ADCvar0)*(3.3/4095.0); //P5.5


      while ((ADC14IFGR0 & BIT4));
      P5SEL1 |= BIT1 | BIT6;                             // Enable A/D channel A4
      P5SEL0 |= BIT1 | BIT6;
      ADCvar4 = ADC14MEM4;
      c = ADCvar4*(3.3/4095.0);//P5.1


      while ((ADC14IFGR0 & BIT5));
      P5SEL1 |= BIT0 | BIT6;                             // Enable A/D channel A5
      P5SEL0 |= BIT0 | BIT6;
      ADCvar5 = ADC14MEM5;
      d = ADCvar5*(3.3/4095.0);//P5.0

      //P5SEL1 &= ~(BIT2 | BIT6);                             // Disable A/D channel A3
      //P5SEL0 &= ~(BIT2 | BIT6);

      r1c1 = c - a;	//P5.1 - P5.4
      r2c1 = c - b; //P5.1 - P5.5
      r1c2 = d - a; //P5.0 - P5.4
      r2c2 = d - b; //P5.0 - P5.5



      printf("%f         %f\n%f         %f\n\n", r1c1, r1c2, r2c1, r2c2);
      printf("%d		%d\n", ADCvar4, ADCvar5);
      printf("%d		%d\n\n", ADCvar1, ADCvar0);
      //__no_operation();                         // SET BREAKPOINT HERE
    }

}
