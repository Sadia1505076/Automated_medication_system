#define F_CPU 16000000L // run CPU at 16 MHz
#define LED 5 // Boarduino LED on PB5
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
// ---------------------------------------------------------------------------
// INCLUDES
#include <avr/io.h> // deal with port registers
#include <util/delay.h> // used for _delay_ms function
#include <string.h> // string manipulation routines
#include<stdio.h>
#include<stdlib.h>
#include "lcd.c"
typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;
// ---------------------------------------------------------------------------
// MISC ROUTINES



void InitAVR()
{
DDRB = 0x3F;
DDRC = 0x00;
}

void FlashLED()
{
SetBit(PORTB,LED);
msDelay(250);
ClearBit(PORTB,LED);
msDelay(250);
}


void LCD_TwoDigits(byte data)
// helper function for WriteDate()
// input is two digits in BCD format
// output is to LCD display at current cursor position
{
byte temp = data>>4;
LCD_Char(temp+'0');
data &= 0x0F;
LCD_Char(data+'0');
}


int main()
{
	InitAVR(); // set port direction
        LCD_Init();
        LCD_String("Ready.");  
        _delay_ms(300);
        LCD_Clear();      

        int hour,min,sec,i,m,l;
	hour=1;min=1;sec=10;
	for(i=sec;i>0;i--)
	{
		//printf("%d:%d:%d\n",hour,min,i);
                //LCD_String(hour+":"+min+":"+i);
                LCD_Integer(hour);
                LCD_Char(':');
                LCD_Integer(min);
                LCD_Char(':');
                LCD_Integer(i);
                _delay_ms(80);
               LCD_Clear();
                
			
	}
    min=min-1;
   
    while(1)
    {
    	sec=60;
    	for(i=sec;i>0;i--)
	      {
		      //printf("%d:%d:%d\n",hour,min,i);
                
                LCD_Integer(hour);
                LCD_Char(':');
                LCD_Integer(min);
                LCD_Char(':');
                LCD_Integer(i);
                _delay_ms(80);
                LCD_Clear();
		      
	     }
	   min=min-1;
	   if(min<0) break;
    }
    //printf("%d:%d:%d\n",hour,min,0);
    hour=hour-1;
    while(1)
    {
    	min=60;
    	min=min-1;
    	 while(1)
        {
    	     sec=60;
    	     for(i=sec;i>=0;i--)
	            {
		          //printf("%d:%d:%d\n",hour,min,i);
                         //LCD_String(hour+":"+min+":"+i);
                         LCD_Integer(hour);
                         LCD_Char(':');
                         LCD_Integer(min);
                         LCD_Char(':');
                         LCD_Integer(i);
                         _delay_ms(80);
                        LCD_Clear();
		       
	            }
	      	   min=min-1;
	           if(min<0) break;
        }
       hour=hour-1;
	   if(hour<0) break;
    }
	return 0;
}


