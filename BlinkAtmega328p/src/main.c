#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	/*
		i = 100;      // 100 is stored in i
		i = 0b100;    // 100b is stored in i which is 4 in decimal
		i = 0x100;    // 100h is stored in i which is also 4 in decimal
	*/
	
	/*
		SET BIT
	PORTB = 8;
	PORTB |= (1 << 2);
		CLEAR BIT
	PORTC = 34;
	PORTC &= ~(1 << 5);
		FLIP BIT
	PORTD = 8;
	PORTD ^= (1 << 5);
		GET BIT
	PORTB = 8;
	if ( (PORTB & (1 << 3)) > 0)
	{
		// do this if the 3 bit is set(1)
	}
	else
	{
		// do this if the 3 bit is not set(0)
	}

		
	*/
	//Setando o bit DDB0 do Data Direction Register da Porta B para 1 - OUTPUT
	DDRB |= (1<<DDB0);
	while(1)
	{
		//Setando o bit PORTB0 - Acende led
		PORTB |= (1<<PORTB0);    
		_delay_ms(500);        //Delay for 1000ms => 1 sec
		
		PORTB &= ~(1<<PORTB0);    //Turn 1th bit on PORTB (i.e. PB0) to 0 => off
		_delay_ms(500);        //Delay for 1000ms => 1 sec
	}
}