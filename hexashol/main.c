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

#include <avr/io.h>
#include <util/delay.h>
#include <string.h> // string manipulation routines
#include <stdlib.h>
#include "lcd.c"
typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;

void GetKeyPressed()
   {
      int r,c;

      PORTA|= 0X0F;

      for(c=0;c<3;c++)
      {
         DDRA&=~(0X7F);

         DDRA|=(0X40>>c);
         for(r=0;r<4;r++)
         {
            if(!(PINA & (0X08>>r)))
            {
		if(r*3+c+1==0x0b) LCD_Integer (0);
               else  LCD_Integer (r*3+c+1);
		_delay_ms(20);
            }
         }
      }

      //return 0xFF;//Indicate No key pressed
   }

void InitAVR()
{
DDRB = 0x3F;
//DDRC = 0x00;
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




   void main()
   {
	InitAVR();
      //Wait for LCD To Start
	LCD_Init();
      //_delay_loop_2(0);
	_delay_ms(50);
	

      //Now initialize the module
      //LCDInit(LS_NONE);
	LCD_String("press a key:");

	LCD_Line(1); 

      int key;

      while(1)
      {
         GetKeyPressed(); //Get the keycode of pressed key
	//_delay_ms(20);

         //LCD_Hex(key+1);  //Print it at location 0,0 on LCD.
	
}

   }
