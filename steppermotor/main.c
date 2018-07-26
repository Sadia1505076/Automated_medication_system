/*The following code will make the stepper motor to run continously*/


#define F_CPU 1000000UL
#include <avr/io.h>
#include<util/delay.h>

int main()
 { 
         DDRC = 0xFF;                        //All pins of PORT C as output
  PORTC = 0x00;                      //Initially all pins as output low
   while (1)                                         //infinite loop
   {
 PORTC = 0x03;                       //0011
 _delay_ms(100);
 PORTC = 0x06;                      //0110
 _delay_ms(100);
 PORTC = 0x0C;                      //1100
 _delay_ms(100);
 PORTC = 0x09;                       //1001
 _delay_ms(100);
   }
   return 0;
 }
