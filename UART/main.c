/*
 * txSerial.c
 *
 *  Author: HHD
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU	16000000
#define BUAD	9600
#define BRC		((F_CPU/16/BUAD) - 1)
#define TX_BUFFER_SIZE	128

#include <util/delay.h>

char serialBuffer[TX_BUFFER_SIZE];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;

void appendSerial(char c);
void serialWrite(char  c[]);

int main(void)
{	
	UBRR0H = (BRC >> 8);
	UBRR0L =  BRC;
	
	UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	sei();
	
	serialWrite("HELLO\n\r");
	serialWrite("woRLd\n\r");
	
	_delay_ms(1500);
	
	serialWrite("GOODByE\n\r");
	
	DDRD |= (1<<DDD6); //seta como output
	PORTD |= (1<<PORTD6); // acende led
	
    while(1)
    {
		serialWrite("HELLO\n\r");
		serialWrite("woRLd\n\r");
		
		_delay_ms(1500);
		
		serialWrite("GOODByE\n\r");
    }
}

void appendSerial(char c)
{
	serialBuffer[serialWritePos] = c;
	serialWritePos++;
	
	if(serialWritePos >= TX_BUFFER_SIZE)
	{
		serialWritePos = 0;
	}
}

void serialWrite(char c[])
{
	for(uint8_t i = 0; i < strlen(c); i++)
	{
		appendSerial(c[i]);
	}
	
	if(UCSR0A & (1 << UDRE0))
	{
		UDR0 = 0;
	}
}

ISR(USART_TX_vect)
{
	if(serialReadPos != serialWritePos)
	{
		UDR0 = serialBuffer[serialReadPos];
		serialReadPos++;
		
		if(serialReadPos >= TX_BUFFER_SIZE)
		{
			serialReadPos++;
		}
	}
}

