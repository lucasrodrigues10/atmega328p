// program to change brightness of an LED
// demonstration of PWM
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


double dutyCycle = 10;
int main()
{
	cli();0
	
	TCCR0A |= ((1<<WGM00)|(1<<COM0A1)|(1<<WGM01));

	TIMSK0 = (1<<TOIE0);
	
	DDRD |= (1<<DDB6);
	OCR0A = (dutyCycle/100)*255;
	
	TCCR0B = (1<<CS00);
	sei();
	while(1)
	{
	}
	
}
ISR(TIMER0_OVF_vect){
	_delay_ms(1000);
	dutyCycle += 10;
	if(dutyCycle>100){
		dutyCycle = 0;
	}
	OCR0A = (dutyCycle/100)*255;
}