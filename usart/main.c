#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/* USART config */
#define F_CPU   16000000
#define BAUD    9600
#define MYUBRR     ((F_CPU/16/BAUD) - 1)

#include <util/delay.h>

void USART_Init(unsigned int ubrr){
	/* Set Baud Rate */
	UBRR0H = (unsigned int) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	/* Activate Transmissor (TX) e Receptor (RX) */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data; 1stop bit */
	UCSR0C = (1<<UCSR0B) | (1<<UCSZ00) | (1<<UCSZ01);

}
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void LED_Init(){
	/* Set led OUTPUT */
	DDRD |= (1<<DDD6);
	/* Turn on LED */
	PORTD |= (1<<PORTD6);
}

void usart_putchar( unsigned char data )
{
	// Do nothing while UDRE0 indicates data still in transit
	while( !(UCSR0A & (1<<UDRE0)) ) {}
	
	// Put data into buffer
	UDR0 = data;
}


void usart_putstring(char s[])
{
	int i = 0;
	
	while (s[i] != 0x00)
	{
		usart_putchar(s[i]);
		i++;
	}
}

int main(void)
{
	/* Turn off interrupt */
	cli();
	
	/* Init USART */
	USART_Init(MYUBRR);
	
	/* Init LED */
	LED_Init();
		
	/* Turn on interrupt */
	sei();
	
	while(1)
	{
		/* Wait 1500ms */
		_delay_ms(1500);
		/* Change led state */
		PORTD ^= (1<<PORTD6);
		/* Print important string */
		usart_putstring( "Sou foda!\r\n" );
	}
}
