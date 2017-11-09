#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <stdlib.h>


volatile long avg = 0;
volatile unsigned char up = 0;
volatile uint32_t running = 0;
volatile uint32_t timercounter =0;
int turn = 0;
char str[10];

/* USART config */
#define F_CPU   16000000UL
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

/*
	Trig       =    PD6
	Echo     =    PD3
*/

// interrupt for INT1 pin, to detect high/low voltage changes
ISR(TIMER0_OVF_vect)
{
	if (up) {
		timercounter++;
	}
}

// We assume, that high voltage rise comes before low drop and not vice versa
// Check change in the level at the PD3 for falling/rising edge
SIGNAL(INT1_vect){
	if(running){ //accept interrupts only when sonar was started
		if (up == 0) { // voltage rise, start time measurement
			up = 1;
			timercounter = 0;
			TCCR0A |= (0 << CS02)|(0 << CS01)|(1 << CS00); // Start/initialize timer with prescalar 0
			TCNT0 = 0; // Initialize Counter
			} else { // voltage drop, stop time measurement
			up = 0;
			avg = (timercounter*256+TCNT0)/58;// divide by 58 to get distance in cm
			running = 0;
			itoa((int)avg,str,10);
			usart_putstring(str);
			usart_putstring("ola\n\r");
		}
	}
}

//send signal for trigger the ultrasonic for 10uS
void send_trigger()
{
	PORTD = 0x00;
	_delay_us(5);
	PORTD = 0xf0;
	running = 1;
	_delay_us(20);
	PORTD = 0x00;
}

int k = 0;
int main()
{
	USART_Init(MYUBRR);
	
	DDRD |= (1<<DDD6);
	DDRD &= ~(1<<DDD3);
	PORTD = 0x00;

	EICRA |= (0 << ISC11) | (1 << ISC10); // enable interrupt on any(rising/droping) edge
	EIMSK |= (1 << INT1);  //Turns on INT1

	TIMSK0 |= (1 << TOIE0);  // enable timer interrupt
	
	sei();  // enable all(global) interrupts
	while(1)
	{
		if(running == 0) {
			_delay_ms(500);
			send_trigger();
		}
	}
}