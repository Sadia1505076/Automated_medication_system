#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#define enable            5
#define registerselection 6
void give_command_to_lcd(unsigned char command);
void character_to_lcd(unsigned char character);
void string_to_lcd(char *string_of_characters);
static volatile int pulse = 0;
static volatile int i = 0;
int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRD = 0b11111011;
	_delay_ms(50);
	
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC00);
	
	TCCR1A = 0;
	
	int16_t COUNTA = 0;
	char SHOWA [16];
	
	give_command_to_lcd(0x01); 
	_delay_ms(50);
	give_command_to_lcd(0x38);
	_delay_ms(50);
	give_command_to_lcd(0b00001111);
	_delay_ms(50);
	
	sei();
	
	while(1)
	{
		PORTD|=(1<<PIND0);
		_delay_us(15);
		PORTD &=~(1<<PIND0);
		
		COUNTA = pulse/58;
		string_to_lcd("ELECTRONIFY.ORG");
		give_command_to_lcd(0x80 + 0x40 + 0);
		string_to_lcd("DISTANCE=");
		itoa(COUNTA,SHOWA,10);
		string_to_lcd(SHOWA);
		string_to_lcd("cm    ");
		give_command_to_lcd(0x80 + 0);
		_delay_ms(100);
	}
}
ISR(INT0_vect)
{
	if (i==1)
	{
		TCCR1B=0;
		pulse=TCNT1;
		TCNT1=0;
		i=0;
	}
	if (i==0)
	{
		TCCR1B|=(1<<CS10);
		i=1;
	}
}
void give_command_to_lcd(unsigned char command)
{
	PORTB = command;
	PORTD &= ~ (1<<registerselection);
	PORTD |= 1<<enable;
	_delay_ms(8);
	PORTD &= ~1<<enable;
	PORTB = 0;
}
void character_to_lcd(unsigned char character)
{
	PORTB = character;
	PORTD |= 1<<registerselection;
	PORTD |= 1<<enable;
	_delay_ms(8);
	PORTD &= ~1<<enable;
	PORTB = 0;
}
void string_to_lcd(char *string_of_characters)
{
	while(*string_of_characters > 0)
	{
		character_to_lcd(*string_of_characters++);
	}
}
