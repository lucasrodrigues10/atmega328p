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
	PORTB = (1<<DDB0);
	//_NOP(); 
	
	//--------------------------------------------------------------
	// cristal externo a 7.3728MHz, então clock interno a 7.3728MHz
	// 7.3728MHz / 1024 (prescale) = 7.2Khz => T=1/7.2kHz = 138,88us
	// 138,88us x 180 = 25ms
	// então TCNT = 256 - 180 = 76 (4CH)
	//--------------------------------------------------------------

	TCCR0B = 0x05;
	TCNT0 = 0x4C;
	TIMSK1 = 0X01;
	
	PORTB = 0;
	sei();
	
    while (1) 
    {
    }
}
ISR(TIMER1_OVF_vect){
	TCNT0 = 0x4C;
	++timecount;
	
	if (timecount==40){
		timecount = 0;
		PORTB ^= PORTB0;//inverte led
	}
}

