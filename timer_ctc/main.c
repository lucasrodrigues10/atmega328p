#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL


int main(void)
{
	cli();
	
	DDRB |= (1<<DDB0);
	PORTB |= (1<<PORTB0);
	
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	
	OCR1A = 15625; //(16M/1024)*1 = 15625 = 3D09
	TCCR1B |= ((1<<WGM12)|(1<<CS10)|(1<<CS12)); //prescaler 1024 e ctc mode
	TIMSK1 |= (1<<OCIE1A);
	
	sei();
    while (1) 
    {
    }
}
ISR(TIMER1_COMPA_vect){
	PORTB ^= (1<<PORTB0);
}

