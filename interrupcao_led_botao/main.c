/*Programa que pisca o led a cada 1s e se clicar no botao fica acesso (utiliza interrupcaum)*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
SREG – Global Interrupt Flag
sei(); - habilita interrupcao no micro
cli(); - desabilita

PCICR – Pin Change Interrupt Control Register (PCIEx):
1 - habilita interrupcao em cada port
0 - desabilita

PCMSK – Pin Change Mask Register (PCMSK0/1/2 = A/B/C ) - PCINTx:
1 - habilita interrupcao do pino em alguma port
0 - desabilita

uma imagem diz mais que 1000 bits
portal.vidadesilicio.com.br/wp-content/uploads/2017/05/PCINTESQUEMA.png
*/

int main(void)
{
	cli();// desabilita interrupcao para configurar as bagaca

	DDRB |= (1<<DDB0); // DEFINE LED COMO OUTPUTI
	
	DDRD &= ~(1<<DDD0); // DEFINE BOTAO COMO SAIR DO ARMARIO
	PORTD |= (1<<PORTD0); // PULL UP LIGADO PARA BOTAO
	
	PCICR |= (1<<PCIE2); // HABILITA INTERRUPCAO DA PORT D
	PCMSK2 |= (1<<PCINT0); // HABILITA INTERRUPCAO DA PORT D PINO 0
	
	sei(); //habilita interrupcao da porra toda
	
	//SUPER LUPI
	while(1){
		//Brilha brilha estrelinha a cada 1S
		PORTB |= (1<<PORTB0);
		_delay_ms(1000);
		PORTB &= ~(1<<PORTB0);
		_delay_ms(1000);		
	}
	
}
ISR(PCINT2_vect){ //ISR (Interrupt Service Routine) - Funcao da interrupcao
	if( (PIND & (1<<PIND0)) == 0){
		_delay_ms(100);
		while( (PIND & (1<<PIND0)) == 0 ){
			PORTB |= (1<<PORTB0); //FASSASSE A LUIZ
		}
		if( (PIND & (1<<PIND0)) != 0 ){
			PORTB &= ~(1<<PORTB0); //AI QUE ESCURIDAUM
		}
	}
}