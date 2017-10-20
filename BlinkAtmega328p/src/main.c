/* Acende ou apaga o led conforme vc aperta o botao - Super Inedito*/

#define F_CPU 16000000UL //Define a frequencia do microcontrolador usa a maxima pq quero fps mlk
#include <util/delay.h> //para usar delay
#include <avr/io.h> //para modificar registradores especiais

int main(void)
{
	/*
	-SET BIT:
	PORTB = 8; // 0000 1000
	PORTB |= (1 << 2);
	-CLEAR BIT:
	PORTC = 34;  // 0010 0010
	PORTC &= ~(1 << 5);
	-FLIP BIT:
	PORTD = 8; // 0000 1000
	PORTD ^= (1 << 5);
	-GET BIT:
	PORTB = 8; // 0000 1000
	if ( (PORTB & (1 << 3)) == 0)
	{
		// do this if the 3 bit is set(1)
	}
	else
	{
		// do this if the 3 bit is not set(0)
	}
	
	
	-DDRx Data Direction Register (para cada port)
	0 = input
	1 = output
	
	-PORTx - Port X Data Register (para cada pino)
	0 - manda ground
	1 - manda vcc (5v)
	
	-PINx - Port X Input Pins Register (valor logico do negocio)
	0 - ta recebendo gnd
	1 - ta recebendo vcc 
	
	*/
	
	DDRB |= (1<<DDB0); //PORTB OUTPUT para led
	DDRD &= ~(1 << DDD0);//DDRD INPUT para botao
	PORTD |= (1 << PORTD0);//PORTD PULLUP LIGADO para botao
	
	//SUPER LUP UHU
	while(1)
	{
		if ( (PIND & (1 << PIND0)) == 0) //Checa se o botão está ligado
		{
			_delay_ms(25); //Tempo de espera para o debaubaunce
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