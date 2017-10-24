/*
 * pisca_led.c
 *
 * Created: 21/10/2017 22:41:02
 * Author : lucas
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD |= (1<<DDD0); //seta como output 
	PORTD |= (1<<PORTD0); // acende led
    while (1) 
    {
		PORTD ^= (1<<PORTD0);//inverte estado led
		_delay_ms(500);
    }
}

