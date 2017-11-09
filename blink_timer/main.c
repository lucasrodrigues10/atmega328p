/*
*      Author: Charles Minchau (SmiteMeSmith)
*      Program uses a ATMega328p MCU to query a HC-SR04 ultrasonic module. Then Calculates and displays the distance to a object in cm on the NewHaven display.
*
*      The ultrasonic module is triggered every 60 ms by setting pin PC4 (the trigger) high for 10 us. A change in state on pin PC5 (the echo)
*      causes a interrupt. The interrupt checks if PC5 is high. If it is a timer is started that increments every micro second. Once PC5 triggers a
*      interrupt again and is low the result is calculated and displayed in centimeters on the NewHaven display. Distance in cm is calculated
*      by the amount of microseconds PC5 (the echo) is active high divided by 58.
*
*      Pin placement of ATMega328p:
*      Port D[7-0]			NewHavenDisplay DB[7-0]
*      Pin PC0				NewHavenDisplay RW
*      Pin PC1				NewHavenDisplay RS
*      Pin PC2				NewHavenDisplay E
*      Pin PC3				Debugging LED Active High
*      Pin PC4				HC-SR04 Trig
*      Pin PC5				HC-SR04 Echo
*/

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

/* USART config */
#define F_CPU   16000000UL
#define BAUD    9600
#define MYUBRR     ((F_CPU/16/BAUD) - 1)
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))


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
/*******************************************INITALIZE PORTS, TIMER, AND INTURRUPTS*******************************************/
void init() {
	USART_Init(MYUBRR);
	LED_Init();
	
	DDRC &= ~(1<<DDC5);						// Set Pin C5 as input to read Echo
	PORTC |= (1<<PORTC5);					// Enable pull up on C5
	PORTC &= ~(1<<PORTC4);						// Init C4 as low (trigger)

	PRR &= ~(1<<PRTIM1);					// To activate timer1 module
	TCNT1 = 0;								// Initial timer value
	TCCR1B |= (1<<CS10) | (1<<CS12);					// Timer without prescaller. Since default clock for atmega328p is 1Mhz period is 1uS
	TCCR1B |= (1<<ICES1);					// First capture on rising edge

	PCICR = (1<<PCIE1);						// Enable PCINT[14:8] we use pin C5 which is PCINT13
	PCMSK1 = (1<<PCINT13);					// Enable C5 interrupt
	sei();									// Enable Global Interrupts
}

uint16_t numuS = 0;
char str[5];

/*******************************************MAIN PROGRAM*******************************************/
int main() {
	init();
	PORTD &= ~(1<<PORTD6);
	while (1) {
		_delay_ms(60); 							// To allow sufficient time between queries (60ms min)
		PORTC |= (1<<PORTC4);						// Set trigger high
		_delay_us(15);							// for 10uS
		PORTC &= ~(1<<PORTC4);						// to trigger the ultrasonic module
	}
}
/*******************************************INTURRUPT PCINT1 FOR PIN C5*******************************************/

ISR(PCINT1_vect) {
	if (bit_is_set(PINC,PINC5)) {
		// Checks if echo is high
		TCNT1 = 0;								// Reset Timer
		PORTD |= (1<<PORTD6);
	} else {
		numuS = TCNT1;					// Save Timer value
		//uint8_t oldSREG = SREG;
		cli();
		usart_putstring("Distancia:   ");
		itoa((int)(numuS/58),str,10);
		usart_putstring(str);		// Write Timer Value / 58 (cm). Distance in cm = (uS of echo high) / 58
		usart_putstring("\n\r");
		//sei();
		//SREG = oldSREG;							// Enable interrupts		
	}
}