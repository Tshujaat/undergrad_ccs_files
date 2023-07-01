// Taha Shujaat (1001046728)
//HW 5 Problem 3b
//******************************************************************************
//   MSP432P401 Demo - Configure MCLK for 12MHz/16 operation, ACLK to 32kHz/8
//
//   Description: Configure SMCLK = MCLK = 12MHz, ACLK = REFOCLK and set REFOCLK to 32kHz.
//
//           MSP432p401rpz
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|---> LED
//        |           P4.2|---> ACLK = ~32kHz
//        |           P4.3|---> MCLK = SMCLK = 12MHz
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

    CSCLKEN = 0; //from manual we know 0b or 0d will set REFOSEL to 32.768kHz, while 1b or 1d will set it to 128kHz
    CSCLKEN =  REFOFSEL; //last two lines are an unnceccessary step since ACLK defaults to 32KHz as seen in the figure above

    CSCTL0 = 0;                            // Reset tuning parameters
    CSCTL0 = DCORSEL_3;           // Set DCO 011b to 12MHz (nominal, center of 8-16MHz range) and REFOCLK to 32kHz from reference manual

    // Select ACLK = REFO, SMCLK = MCLK = DCO = 12 MHz, DIVM_4 = MCLK/16 = 12MHz/16,  DIVA_3 = ACLK/8 = 32kHz/8
    CSCTL1 = SELA_2 | SELS_3 | SELM_3 | DIVM_4 |  DIVA_3;
    CSKEY = 0;                             // Lock CS module from unintended accesses


    while (1)                               // continuous loop
    {
        P1OUT ^= BIT0;                          // XOR P1.0
        for (i = 20000; i > 0; i--)
            ;                   // Delay
    }
}
