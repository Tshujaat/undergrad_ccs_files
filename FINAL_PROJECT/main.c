/* EE2441 Final Project
   Copyright (C) <2017> <Taha Shujaat> <taha.shujaatt@mavs.uta.edu> <tahashujaat1997@gmail.com>

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License the terms of the General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied waranty of
   MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have recieved a copy of the GNU General Public License
   along with this program. If not, see <http:/www.gnu.org/licenses/>.

   */

#include <msp430.h>

#define inputs (BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT7)
#define outputs (BIT0 + BIT6)
#define LS377CLOCK BIT6



void ClockData(void)
{
   P1OUT |= LS377CLOCK;
   __delay_cycles(1);
   P1OUT &= ~LS377CLOCK;
}



void init(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    P1DIR |= outputs;
    P1DIR &= ~(BIT3);

    P1OUT |= inputs;
    P1OUT &= ~(outputs);

    P1REN |= inputs;
}



long int savefunc;

long int main(void)
{
	init();
    char on = 0;
    long int bitmask = 0;
    long int bitlocation = 1;


    while(1)
    {

    	  on = inputdebounce();
          shiftbit(bitlocation^bitmask);
          __delay_cycles(80000);
          shiftbit(bitmask);
          __delay_cycles(80000);

        switch(on)
        {
                case 0 :
                    break;

                case 1 :
                 if ((bitlocation==8)||(bitlocation==128)||(bitlocation==2048)||(bitlocation==32768)||(bitlocation==524288)||(bitlocation==8388608))
               	 	 {
               	     	 /*2^(3), 2^(7), 2^(11), 2^(15), 2^(19), 2^(23), where the powers of 2 are the bit positions. The bit positions will have the cursor do nothing since those are the places where the down button cannot wrap around to the bottom*/
               	 	 }
                    	else
                    		{
                    			bitlocation=bitlocation<<1;
                    		}
                 __delay_cycles(160000);
                 break;

                case 2 :
                	if ((bitlocation==1)||(bitlocation==16)||(bitlocation==256)||(bitlocation==4096)||(bitlocation==65536)||(bitlocation==1048576))
                        {
                        	/*2^(0), 2^(4), 2^(8), 2^(12), 2^(16), 2^(20) positions will have the cursor do nothing since those are the places where the up button cannot wrap around to the bottom*/
                        }
                        	else
                            	{
                        			bitlocation=bitlocation>>1;
                            	}
                	__delay_cycles(160000);
                	break;

                case 3 :
                	if ((bitlocation==1048576)||(bitlocation==2097152)||(bitlocation==4194304)||(bitlocation==8388608))
                		{
                		bitlocation=bitlocation>>20;
                		//2^(20), 2^(21), 2^(22), 2^(23) positions will have the cursor will shift the cursor location by 20 in order to wrap around to the other side when the left button is pressed otherwise it will just shift to the left by 4 bits as it regularly would.
                	    }

                    		else
                    			{
                    				bitlocation=bitlocation<<4;
                    			}
                __delay_cycles(160000);
                break;

                case 4 :
                	if ((bitlocation==1)||(bitlocation==2)||(bitlocation==4)||(bitlocation==8))
                		{
                		bitlocation=bitlocation<<20;
                        }
                        	else
                        		{
                        			bitlocation=bitlocation>>4;
                        		}
                __delay_cycles(160000);
                break;

                case 5 :
                bitmask=bitlocation^bitmask;
                __delay_cycles(160000);
                break;

                case 7 :
                savefunc=bitmask;
                __delay_cycles(160000);
                break;
       }

    }
}



long int inputdebounce(void)
{
    char recorded_val = 0;

    if (!(P1IN & BIT1))
    	{
        	recorded_val = 1;
        	__delay_cycles(160000);
        	while(!(P1IN & BIT1))
        		{
        			__delay_cycles(160000);
        		}
    	}

    else if(!(P1IN & BIT2))
    {
    	recorded_val = 2;
        __delay_cycles(160000);
        while(!(P1IN & BIT2))
        	{
            	__delay_cycles(160000);
            }
    }

    else if(!(P1IN & BIT3))
    {
    	recorded_val = 3;
        __delay_cycles(160000);
        while(!(P1IN & BIT3))
        	{
            	__delay_cycles(160000);
            }
    }

    else if(!(P1IN & BIT4))
    {
    	recorded_val = 4;
        __delay_cycles(160000);
        while(!(P1IN & BIT4))
        {
        	__delay_cycles(160000);
        }
    }

    else if(!(P1IN & BIT5))
    {
    	recorded_val = 5;
    	__delay_cycles(160000);
    	while(!(P1IN & BIT4))
    	{
    		__delay_cycles(160000);
        }
    }

    else if(!(P1IN & BIT7))
    {
    	recorded_val = 7;
        __delay_cycles(160000);
        while(!(P1IN & BIT7))
        {
        	__delay_cycles(160000);
        }
    }
    return recorded_val;
        }







void shiftbit(long int D)
{
   char bit, lsb;
   for(bit=0; bit<=23; bit++)
   {
       lsb = D & 0x01;
       if (lsb)
       {
           P1OUT |= 0x01;
       }
       else
       {
           P1OUT &= ~0x01;
       }

       ClockData();
       D >>= 0x01;
   }
}



