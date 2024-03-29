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
//    Vin2 -->|P5.3/A2          |
//    Vin3 -->|P5.2/A3          |
//            |                 |
//
//   Wei Zhao
//   Texas Instruments Inc.
//   June 2014
//   Built with Code Composer Studio V6.0
//******************************************************************************
#include "msp.h"
#include <stdint.h>

#define   Num_of_Results   8

volatile uint16_t A0results[Num_of_Results];
volatile uint16_t A1results[Num_of_Results];
volatile uint16_t A2results[Num_of_Results];
volatile uint16_t A3results[Num_of_Results];
static uint8_t index;

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  // Configure GPIO
  P5SEL1 |= BIT5 | BIT4 | BIT3 |BIT2;       // Enable A/D channel A0-A3
  P5SEL0 |= BIT5 | BIT4 | BIT3 |BIT2;

  __enable_interrupt();
  NVIC_ISER0 = 1 << ((INT_ADC14 - 16) & 31);// Enable ADC interrupt in NVIC module

  ADC14CTL0 = ADC14ON | ADC14MSC | ADC14SHT0__192 | ADC14SHP | ADC14CONSEQ_3; // Turn on ADC14, extend sampling time
                                                                              // to avoid overflow of results

  ADC14MCTL0 = ADC14INCH_0;                 // ref+=AVcc, channel = A0
  ADC14MCTL1 = ADC14INCH_1;                 // ref+=AVcc, channel = A1
  ADC14MCTL2 = ADC14INCH_2;                 // ref+=AVcc, channel = A2
  ADC14MCTL3 = ADC14INCH_3+ADC14EOS;        // ref+=AVcc, channel = A3, end seq.
  ADC14IER0 = ADC14IE3;                     // Enable ADC14IFG.3

  SCB_SCR &= ~SCB_SCR_SLEEPONEXIT;          // Wake up on exit from ISR

  while(1)
  {
    ADC14CTL0 |= ADC14ENC | ADC14SC;        // Start conversion-software trigger
    __sleep();
    __no_operation();                       // For debugger
  }

}

// ADC14 interrupt service routine
void ADC14IsrHandler(void)
{
    if (ADC14IFGR0 & ADC14IFG3)
    {
        A0results[index] = ADC14MEM0;       // Move A0 results, IFG is cleared
        A1results[index] = ADC14MEM1;       // Move A1 results, IFG is cleared
        A2results[index] = ADC14MEM2;       // Move A2 results, IFG is cleared
        A3results[index] = ADC14MEM3;       // Move A3 results, IFG is cleared
        index = (index + 1) & 0x7;          // Increment results index, modulo
        __no_operation();                   //Set Breakpoint1 here
    }

}

