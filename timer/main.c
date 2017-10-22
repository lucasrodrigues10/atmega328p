#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

/*
	www.embarcados.com.br/timers-do-atmega328-no-arduino/
	TCCRxA/B (Controle do timer e contador)
	TIMSK (Mascara de interrupção para timer e contador);
	TCNTx (Registro para contagem do timer e contador).
*/

int main(void)
{
	cli();
	
	DDRB |= (1<<DDD0); //seta led como output
	PORTB |= (1<<PORTB0); //acende led
	
	TCCR1A = 0; //operacao normal 
	TCCR1B = 0; //limpa o registrador
	TCCR1B |= (1<<CS10 | 1<<CS12); //prescaler = 1024 CS12=1 CS10=1 (Clock Select)
	
	TCNT1 = 0xC2F7; //inicia timer com estouro em (2^16-(16MHz/1024)*1Hz) = 49911 = 0xC2F7
	TIMSK1 |= (1<<TOIE1);
	
	sei();
	
	while (1)
	{
		
	}
}
ISR(TIMER1_OVF_vect){
	TCNT1 = 0xC2F7; //reseta contador	
	PORTB ^= (1<<PORTB0);//inverte led
}

