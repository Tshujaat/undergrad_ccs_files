#include <msp430.h>
#define LS377CLOCK BIT0
#define outputs (BIT6 + BIT0)
#define inputs (BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT7)
long int saved;

void init(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    P1DIR |= outputs;                      // Set P1.0 and P1.6 to output direction

    P1DIR &= ~(BIT3);
    P1REN |= inputs;
    P1OUT |= inputs;

    P1OUT |= ~(outputs);
}


long int inputinterpretation(void)
{
    char local = 0;
    // The P1IN registers holds the logical values of the pins that are
    // configured as inputs
    if (!(P1IN & BIT1)) {           // Check for S2 input
       // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
        local = 1;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(!(P1IN & BIT1))   {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
            __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
        }}
        else if(!(P1IN & BIT2)) {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local = 2;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P1IN & BIT2))   {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }}
        else if(!(P1IN & BIT3)) {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local = 3;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P1IN & BIT3))   {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }}
        else if(!(P1IN & BIT4)) {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local = 4;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P1IN & BIT4))   {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }}
        else if(!(P1IN & BIT5)) {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local = 5;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P1IN & BIT4))   {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }}
        else if(!(P1IN & BIT7)) {           // Check for S2 input
            // pushes = pushes + 1;                                // Code that executes when S2 button is pushed
             local = 7;
             __delay_cycles(10000);          // Wait for debounce to end (bad code)
             while(!(P1IN & BIT7))   {
                 // Wait for Up button to return to "normal"
                 // Must re-read the input port
                 // Not good code because loop could run forever
                 __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
             }}
    return local;
        }
void ClockData(void)
{
   P1OUT |= LS377CLOCK;                            // Assert P1.6 (rising clock edge)
   __delay_cycles(1);                              // Delay
   P1OUT &= ~LS377CLOCK;                           // De-assert P1.6 (falling clock)
}
/*void blink(mask)
 {
    SerialDataOut(0);
 }*/
// Shift out the value of DataOut to the connected shift register
void SerialDataOut(long int DataOut)
{
   char i, lsb;                            // i = for loop index
                                           // lsb = Value of the Least Significant Bit of DataOut
   for(i=0; i<=23; i++)
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
    char enable = 0;            // Holds status of the S2 button
   // char enableclock = 0;
    init();                     // One time initialization function
    long int cursor = 1;
    long int mask = 0;

while(1)
    {

            SerialDataOut(cursor^mask);
            __delay_cycles(80000);
            SerialDataOut(mask);
            __delay_cycles(80000);
            enable = inputinterpretation();

 switch(enable)
  	  {
                case 0 :

                    break;
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

                case 3 :
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

                case 4 :
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

                case 7 :
                saved=mask;
                __delay_cycles(80000);
                break;
  	  }

    }
}
