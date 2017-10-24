// program to change brightness of an LED
// demonstration of PWM
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int i =0;
double dutyCycle = 50;

int main()
{
	cli();
	
	TCCR0A |= ((1<<WGM00)|(1<<COM0A1)|(1<<WGM01)); //timer que vai set prescaler

	TIMSK0 = (1<<TOIE0);//habilita interrupcao do timer
	
	DDRD |= (1<<DDB6); // seta led como output
	//data direction registor
	DDRC |= (1<<DDC0); //IN4 ponte h  seta os pinos como output 
	DDRC |= (1<<DDC1); //IN3 ponte h	
	//port
	PORTC |= (1<<PORTC0); // seta pinos da ponte H como 1 e 0
	PORTC &= ~(1<<PORTC1);	
	//seta os pinos como output
	DDRB |= (1<<DDB1); //IN1 ponte h  seta os pinos como output
	DDRB |= (1<<DDB2); //IN2 ponte h
	//port
	PORTB |= (1<<PORTB1); // seta pinos da ponte H como 1 e 0
	PORTB &= ~(1<<PORTB2);
	
	
	OCR0A = (dutyCycle/100)*255; 
	
	TCCR0B = (1<<CS00);
	sei(); // DESELIGA INTERRUBCAO 
	while(1)
	{
	}
	
}

ISR(TIMER0_OVF_vect){
	_delay_ms(1000);
	if(sin(i*.4) >= 0){
		dutyCycle = sin(i*.4)*50+50;
		//port
		PORTC |= (1<<PORTC0); // seta pinos da ponte H como 1 e 0
		PORTC &= ~(1<<PORTC1);
		//port
		PORTB |= (1<<PORTB1); // seta pinos da ponte H como 1 e 0
		PORTB &= ~(1<<PORTB2);
	}
	else{
		dutyCycle = (-1)*sin(i*.4)*50 + 50;
		//port
		PORTC |= (1<<PORTC1); // seta pinos da ponte H como 1 e 0
		PORTC &= ~(1<<PORTC0);
		//port
		PORTB |= (1<<PORTB2); // seta pinos da ponte H como 1 e 0
		PORTB &= ~(1<<PORTB1);
	}
	if(dutyCycle>=100){
		dutyCycle = 0;
		/*
		
		*/
	}
	OCR0A = (dutyCycle/100)*255;
	i++;
}
