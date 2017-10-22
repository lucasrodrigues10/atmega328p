#include <avr/io.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> //para usar o _NOP()

#define F_CPU 16000000UL

/*
TCCR0 (Controle do timer e contador)
TIMSK (Mascara de interrupção para timer e contador);
TCNT0 (Registro para contagem do timer e contador).
*/
unsigned char timecount;

int main(void)
{
	cli();
	
	DDRB |= (1<<DDD0); //seta led como output
	PORTB |= (1<<PORTB0); //acende led
	
	
	TCCR1A = 0; //operacao normal
	TCCR1B = 0;
	TCCR1B |= (1<<CS10 | 1<<CS12);
	
	TCNT1 = 0xC2F7;

	TIMSK1 |= (1<<TOIE1);
	
	sei();
	
	while (1)
	{
		
	}
}
ISR(TIMER1_OVF_vect){
	TCNT1 = 0xC2F7;
	++timecount;
	
		//PORTB &= ~(1<<PORTB0); //acende led

			PORTB ^= 0b00000001;//inverte led

}

