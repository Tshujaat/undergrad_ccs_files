#include <msp430g2553.h>
#define DATA P1OUT //DATA LINES
#define CNTRL P2OUT //CONTROL LINES
void Commd(unsigned char commd);
void Data(unsigned char Lcd_Data);
unsigned int i,j;
unsigned char msg[]={" OH MY "}; //MESSAGE TO BE DISPLAYED

void main()
{

WDTCTL = WDTPW + WDTHOLD;
P1DIR=0xFF;
P2DIR=0x0F;
Commd(0x38); //Function Set: 8-bit, 2 Line, 5x7 Dots
Commd(0x06); //Entry Mode
Commd(0x0F); //Display on Cursor blinking
Commd(0x01); //Clear Display (also clear DDRAM content)
Commd(0x80); //Set DDRAM address or cursor position to first row first col

while(1) // Infinite Loop
{
Commd(0x01); //Clear Display (also clear DDRAM content)
Commd(0x80); //Move cursor position to first row first col
for(j=0;j<16;j++) // To display 0 to 16 characters
Data(msg[j]); //DISPLAYING CHARACTERS
Commd(0xC0); //Move cursor position to 2ND LINE
for(j=0;j<16;j++)
Data(msg1[j]); //DISPLAYING NEXT CHARACTERS
for(i=0;i<50000;i++); //DELAY
}

void Data(unsigned char Lcd_Data)
{
DATA = Lcd_Data; // Place Data in PORT1 - (Data pins connected)
CNTRL = 0X05; // RS=1,RW=0,EN=1
for(i=0;i<50;i++); //Delay
CNTRL = 0X01; // RS=1,RW=0,EN=0
for(i=0;i<50;i++);//Delay
}

void Commd(unsigned char commd)
{
DATA = commd; // Place Data in PORT1 - (Data pins connected)
CNTRL = 0X04; //RS=0,RW=0,EN=0
for(i=0;i<50;i++);//Delay
CNTRL = 0X00; //RS=0,RW=0,EN=0
for(i=0;i<50;i++);//Delay
}
