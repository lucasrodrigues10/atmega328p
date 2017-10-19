#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h> //para usar delay
#include <avr/io.h> //para modificar registradores especiais
#include <avr/interrupt.h> //para usar interrupcao

volatile uint8_t test;


int main(void)
{
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
	
	
	sei(); //habilitar interrupcao
	//cli(); //desabilita interrupcao
	
	
	
	DDRB |= (1<<DDB0); //PORTB OUTPUT para led
	DDRD &= ~(1 << DDD0);//DDRD INPUT para botao
	PORTD |= (1 << PORTD0);//PORTD PULLUP LIGADO para botao
	
	while(1)
	{
		if ( (PIND & (1 << PIND0)) == 0) //Checa se o botão está ligado
		{
			_delay_ms(25); //Tempo de espera para o debounce
			if ( (PIND & (1 << PIND0)) == 0) //Para debounce
			{
				PORTB |= (1<<PORTB0);  //Acende Led
			}
		}
		else
		{
			PORTB &= ~(1<<PORTB0); //Apaga Led
		}
	}
}

ISR (INT0_vect)    // INT0 interrupt function
{
	/* enter code to execute here */
}

ISR (BADISR_vect)  // special function, to execute if a bad interrupt is called
{
	/* enter code to execute here */
}