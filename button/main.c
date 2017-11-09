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

	DDRD |= (1<<DDB6); // DEFINE LED COMO OUTPUTI
	
	DDRC &= ~(1<<DDC5); // DEFINE BOTAO COMO SAIR DO ARMARIO
	PORTC |= (1<<PORTC5); // PULL UP LIGADO PARA BOTAO
	
	PCICR |= (1<<PCIE1); // HABILITA INTERRUPCAO DA PORT D
	PCMSK1 |= (1<<PCINT13); // HABILITA INTERRUPCAO DA PORT D PINO 0
	
	sei(); //habilita interrupcao da porra toda
	
	//SUPER LUPI
	while(1){
		//Brilha brilha estrelinha a cada 1S
		PORTD |= (1<<PORTD6);
		_delay_ms(2000);
		PORTD &= ~(1<<PORTD6);
		_delay_ms(1000);
	}
	
}
ISR(PCINT1_vect){ //ISR (Interrupt Service Routine) - Funcao da interrupcao
	if( (PINC & (1<<PINC5)) == 0){
		_delay_ms(100);
		while( (PINC & (1<<PINC5)) == 0 ){
			PORTD |= (1<<PORTD6); //FASSASSE A LUIZ
		}
		if( (PINC & (1<<PINC5)) != 0 ){
			PORTD &= ~(1<<PORTD6); //AI QUE ESCURIDAUM
		}
	}
}