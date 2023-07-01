// Taha Shujaat (1001046728)
/*HW 5 Problem 3a
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
//   MSP432P401 Demo - Output MCLK & ACLK @ DCO default frequency
//
//   Description: With MCLK=SMCLK=default/4 = 3MHz/4  DCO frequency, ACLK = REFOCLK, toggle
//   P1.0 LED & output MCLK & ACLK to port pins
//
//           MSP432p401rpz
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|---> LED
//        |           P4.2|---> ACLK = ~32kHz
//        |           P4.3|---> MCLK = SMCLK = default DCO = 3MHz
//   Dung Dang
//   Texas Instruments Inc.
//   Nov 2013
//   Built with Code Composer Studio V6.0
//******************************************************************************
#include "msp.h"

int main(void) {
    volatile unsigned int i;

    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
    P1DIR |= BIT0;                          // P1.0 set as output

    P4DIR |= BIT2 | BIT3;
    P4SEL0 |= BIT2 | BIT3;                         // Output ACLK & MCLK
    P4SEL1 &= ~(BIT2 | BIT3);


    CSKEY = CSKEY_VAL;                        // Unlock CS module for register access
    // Select ACLK = REFO, SMCLK = MCLK = DCO, DIVM to MCLK/4 i.e. 3MHz/4 = 750kHz
    CSCTL1 = SELA_2 | SELS_3 | SELM_3 | DIVM_2;
    CSKEY = 0;                             // Lock CS module from unintended accesses, doesn't have to be 0 can also be a different value from CSKEY_VAL


    while (1)                               // continuous loop
    {
        P1OUT ^= BIT0;                      // XOR P1.0
        for (i = 20000; i > 0; i--);        // Delay
    }
}

