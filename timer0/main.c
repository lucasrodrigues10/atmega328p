/*
* timer0.c
*
* Created: 22/10/2017 13:22:36
* Author : lucas
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL;
/*
Timer 0 - 8bit - CTC
X - Timer counter nubmer
x = a,b compatre unit

TCCRxn -
TCNTn - onde guarda os numero que contam
OCRnx - valor que vai ser comparado com TCNTn
TIMSK0 - controla a interrupcao do timer

(se nao usar interrupcao)
TIFR0 -  tem o bit OCF0A que é uma flag para ver tem a interrupcao
*/

unsigned int count = 0;
int main(void)
{
	cli();
	
	DDRB |= (1<<DDD0);
	PORTB |= (1<<PORTB0);
	
	TCCR0A |= ((1<<WGM10)); //set ctc bit
	TCCR0B |= (1<<CS02 | 1<<CS00); //seta o prescaler para 1024
	OCR0A = 15625; // frequencia de 1 hz
	TIMSK0 |= (1<<OCIE0A); //habilita a interrupcao do timer
	sei(); //comeca interrupcao
	while (1)
	{
	}
}
ISR(TIMER0_COMPA_vect){
	count++;
	if(count >100){
		PORTB ^= (1<<PORTB0);//muda estado
		count = 0;
	}
}

