#include <msp430.h>
#define LS377CLOCK BIT0
#define output (BIT0 + BIT6)
#define input (BIT1 + BIT2 + BIT7)
#define input2 (BIT0 + BIT1 + BIT2)
// Global variable used to hold the current state
//char mask;
// Global variable used to hold the number of pushes
long int saved;
/*
 * Initialization function runs once before the state machine while(1) loop.
 */



void init(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    P1DIR |= output;                      // Set P1.0 and P1.6 to output direction

                                            // P1.3 is set to input by default in the P1DIR reg
    P1DIR &= ~(BIT3);                       // So clearing it here is redundant

                                            // Setting the internal resistor to be a pullup
                                            // may be redundant if P1.3 has an external pullup
    P1REN |= input;                            // P1.3 enable internal resistor
    P1OUT |= input;                             // P1.3 make internal resistor pullup

    P1OUT |= ~(output);                 // Set display initially to zero
                                            // Using a mask so that only the desired bits are affected
    P2REN |= input2;
    P2OUT |= input2;


}

/*
 * readInput - This function reads the status of the enable button
 *              and returns its value
 *              (S2 on the Launchpad connected to Port 1 Bit 3)
 *
 *              returns 0 if the button is not pressed
 *              returns 1 if the button is pressed
 *
 *              It also waits for the button to be released so that only one
 *              button push is registered
 */



long int input1interpretation(void)
{
    char local = 0;
    // The P1IN registers holds the logical values of the pins that are
    // configured as inputs

    if (!(P1IN & BIT1))
    {           // Check for S2 input
       // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
        local = 1;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(!(P1IN & BIT1))
        {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
            __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
        }
    }

        else if(!(P1IN & BIT2))
        {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local = 2;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P1IN & BIT2))
             {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }
        }

        else if(!(P1IN & BIT7))
        {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local = 7;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P1IN & BIT7))
             {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }
        }
    return local;
        }


long int input2interpretation(void)
{
    char local2 = 0;
    // The P1IN registers holds the logical values of the pins that are
    // configured as inputs

    if (!(P2IN & BIT0))
    {           // Check for S2 input
       // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
        local2 = 1;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(!(P2IN & BIT0))
        {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
            __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
        }
    }

        else if(!(P2IN & BIT1))
        {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local2 = 2;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P2IN & BIT1))
             {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }
        }

        else if(!(P2IN & BIT2))
        {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local2 = 7;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P2IN & BIT2))
             {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }
        }
    return local2;
        }


void ClockData(void)
{
   P1OUT |= LS377CLOCK;                            // Assert P1.0 (rising clock edge)
   __delay_cycles(1);                              // Delay
   P1OUT &= ~LS377CLOCK;                           // De-assert P1.0 (falling clock)
}






/*void blink(mask)
 {
    SerialDataOut(0);
 }*/
// Shift out the value of DataOut to the connected shift register
void SerialDataOut(long int DataOut)
{
   char bit, lsb;                            // i = for loop index
                                           // lsb = Value of the Least Significant Bit of DataOut
   for(bit=0; bit<=23; bit++)
   {
       lsb = DataOut & 0x01;               // Set lsb to right most bit of DataOut
       // Place the LSB of DataOut on P1.0 and therefore to the input of the shift register
       if (lsb)
       {
           P1OUT |= 0x01;      // Assert P1.0 if the LSB of DataOut is 1
       }
       else
       {
           P1OUT &= ~0x01;     // De-assert P1.0 if the LSB of DataOut is 0
       }
       __delay_cycles(100);              // Delay
       // Clock the Data to the Shift Register
       ClockData();
       // Shift data left and repeat the for loop
       DataOut >>= 0x01;
   }
}




/*
* main.c
*/
long int main(void)
{
    char on = 0;            // Holds status of the S2 button
    char on2 = 0;
   // char enableclock = 0;
    init();                     // One time initialization function
    long int cursor = 1;
    long int mask = 0;
    /*
     * The main while loop of the state machine will constantly scan the inputs
     * and update the system based on those inputs
     */
    while(1)
 {
                          // Clear Data
            SerialDataOut(cursor^mask);
          //1`SerialDataOut(mask);
           __delay_cycles(80000);
          SerialDataOut(mask);
           __delay_cycles(80000);
        on = input1interpretation();   // Check the S2 button

        /*
         * The decision making in the state machine can be made with
         * a switch statement or nested if statements
         * Both implementations are given below.  Only one should be enabled at a time
         * Simply make one and only one of the #if statements #if 1.
         */

//#if 0
        switch(on)
        {

                case 1 :
                    if ((cursor==8)||(cursor==128)||(cursor==2048)||(cursor==32768)||(cursor==524288)||(cursor==8388608))
                    {
                    	//2^(3), 2^(7), 2^(11), 2^(15), 2^(19), 2^(23), where the powers of 2 are the bit positions. The bit positions will have the cursor do nothing since those are the places where the down button cannot wrap around to the bottom
                    }
                    else
                    {
                    cursor=cursor<<1;
                    }
                    __delay_cycles(80000);
                    break;

                case 2 :
                	if ((cursor==1)||(cursor==16)||(cursor==256)||(cursor==4096)||(cursor==65536)||(cursor==1048576))
                	{
                		//2^(0), 2^(4), 2^(8), 2^(12), 2^(16), 2^(20) positions will have the cursor do nothing since those are the places where the up button cannot wrap around to the bottomj
                	}
                else
                 	 {
                      cursor=cursor>>1;
                 	 }
                    __delay_cycles(80000);
                    break;


                case 6 :
                                    // Do nothing if the button is not pressed
                break;

                case 7 :
                saved=mask;
                __delay_cycles(80000);
                break;

        }

 } // end while loop



    while(1)
  {
                           // Clear Data
             SerialDataOut(cursor^mask);
           //1`SerialDataOut(mask);
            __delay_cycles(80000);
           SerialDataOut(mask);
            __delay_cycles(80000);
         on2 = input2interpretation();   // Check the S2 button
         /*
          * The decision making in the state machine can be made with
          * a switch statement or nested if statements
          * Both implementations are given below.  Only one should be enabled at a time
          * Simply make one and only one of the #if statements #if 1.
          */

 //#if 0
         switch(on2)
         {

         case 0 :
                       if ((cursor==1048576)||(cursor==2097152)||(cursor==4194304)||(cursor==8388608))
                        {
                           cursor=cursor>>20;
                        }

                       else
                        {
                           cursor=cursor<<4;
                        }
                       __delay_cycles(80000);
                       break;
         case 1 :
                         if ((cursor==1)||(cursor==2)||(cursor==4)||(cursor==8))
                         {
                             cursor=cursor<<20;
                         }
                         else
                         {
                            cursor=cursor>>4;
                         }
                         __delay_cycles(80000);
                         break;
                 case 5 :
                 mask=cursor^mask;
                 __delay_cycles(80000);
                 break;

                 }

  }

}  // end main
