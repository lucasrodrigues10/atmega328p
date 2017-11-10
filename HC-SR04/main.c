/* circuitdigest.com/microcontroller-projects/distance-measurement-using-hc-sr04-avr */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/* USART config */
#define F_CPU   16000000UL
#define BAUD    9600
#define MYUBRR     ((F_CPU/16/BAUD) - 1)

#include <util/delay.h>

static volatile int pulse = 0;

static volatile int i = 0;

static volatile int timer = 0;


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
	
	TIMSK1 |=(1<<TOIE1); //enable timer interrupt
	
	/* Trigger OUTPUT*/
	DDRD |= (1<<DDD7);
	/* Echo INPUT */
	DDRD &= ~(1<<DDD2);

	_delay_ms(50);

	EIMSK|=(1<<INT0);//enabling interrupt0

	EICRA |=(1<<ISC00);//setting interrupt triggering logic change

	int16_t COUNTA = 0;//storing digital output

	char SHOWA [3];
	
	sei();// enabling global interrupts

	while(1)

	{
		/* Trigger Enable */
		PORTD|=(1<<PIND7);

		_delay_us(15);

		/* Trigger Disable */
		PORTD &=~(1<<PIND7);

		COUNTA = pulse/(58*2);
		
		usart_putstring("DISTANCE = \n\r");

		itoa(COUNTA,SHOWA,10); 

		usart_putstring(SHOWA);
		usart_putstring("cm\n\r");
		
		
		_delay_ms(500);
	}

}

ISR(INT0_vect)//interrupt service routine when there is a change in logic level

{

	if (i==1)//when logic from HIGH to LOW

	{

		//TCCR1B=0;//disabling counter

		pulse=TCNT1 + timer*65535;//count memory is updated to integer

		TCNT1=0;//resetting the counter memory

		i=0;
		
		timer = 0;

	}

	if (i==0)//when logic change from LOW to HIGH

	{
		timer = 0;

		TCCR1B|=(1<<CS11);//enabling counter

		i=1;

	}

}
ISR(TIMER1_OVF_vect)
{
	if (i == 1) {
		TCNT1 = 0;
		timer++;
		PORTD |= (1<<PORTD6);
	}
}
