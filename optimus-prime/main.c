/* Robot using ATMEGA328P */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>


/* USART config */
#define F_CPU   16000000UL
#define BAUD    9600
#define MYUBRR     ((F_CPU/16/BAUD) - 1)

/* Data For Usart Print */
#define MAX_DATA 1000

/* Motor */
#define step .1
#define time 50

#include <util/delay.h>

/* Duty Cycle in percent */
double dutyCycle = 50;

void USART_Init(unsigned int ubrr){
	/* Set Baud Rate */
	UBRR0H = (unsigned int) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	/* Activate Transmissor (TX) and Receptor (RX) */
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
	/* Turn off LED */
	PORTD &= ~(1<<PORTD6);
}
void Button_Init(){
	DDRC &= ~(1<<DDC5);
	PORTC |= (1<<PORTC5);
}

void Button_Interrupt_Init(){
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT13);
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

void HBridge_Init(){
	DDRB |= (1<<DDB1); //IN1 output
	DDRB |= (1<<DDB2); //IN2 output
	DDRC |= (1<<DDC1); //IN3 output
	DDRC |= (1<<DDC0); //IN4 output
	
	PORTC |= (1<<PORTC0); // IN4 HIGH
	PORTC &= ~(1<<PORTC1); // IN3 LOW
	PORTB |= (1<<PORTB1); // IN1 HIGH
	PORTB &= ~(1<<PORTB2); // IN2 LOW
}

void Timer_Init(){
	/* Prescaler timer */
	TCCR0A |= ((1<<WGM00)|(1<<COM0A1)|(1<<WGM01));
	
	/* Enable timer interrupt */
	TIMSK0 = (1<<TOIE0);
	
	/* Set dutyCycle in PWM */
	OCR0A = (dutyCycle/100)*255;
	
	/* Dont remember what is this */
	TCCR0B = (1<<CS00);
}

void Forward_Wheels(){
	PORTC |= (1<<PORTC0);
	PORTC &= ~(1<<PORTC1);
	PORTB |= (1<<PORTB1);
	PORTB &= ~(1<<PORTB2);
}
void Backward_Wheels(){
	PORTC |= (1<<PORTC1);
	PORTC &= ~(1<<PORTC0);
	PORTB |= (1<<PORTB2);
	PORTB &= ~(1<<PORTB1);
}
void Stop_Wheels(){
	OCR0A = 0;
}

int main()
{
	/* Disable interrupt */
	cli();
	
	/* H Bridge Init */
	HBridge_Init();
	
	/* Initialize Timer PWM Mode */
	Timer_Init();
	
	/* Led Init */
	LED_Init();
	
	/* Button Init */
	Button_Init();
	Button_Interrupt_Init();
	
	/* USART Init */
	USART_Init(MYUBRR);
	
	/* Enable Interrupt */
	sei();
	
	/* Super Loop */
	while(1)
	{
	}
	
}
/* Data */
int vel[MAX_DATA];

/* Aux Global Variable */
int i = 0;

/* Motor PWM */
ISR(TIMER0_OVF_vect){
	_delay_ms(time);
	
	if(sin(i*step) >= 0){
		dutyCycle = sin(i*step)*50+50;
		Forward_Wheels();
	}
	else{
		dutyCycle = (-1)*sin(i*step)*50 + 50;
		Backward_Wheels();
	}
	if(dutyCycle>=100){
		dutyCycle = 0;
	}
	vel[i] = (int)dutyCycle;
	OCR0A = (dutyCycle/100)*255;
	i++;
}

/* Button Interrupt */
ISR(PCINT1_vect){
	if( (PINC & (1<<PINC5)) == 0){
		/* Debounce */
		_delay_ms(50);
		if( (PINC & (1<<PINC5)) == 0 ){
			/* Disable Motor Interrupt and Stop */
			Stop_Wheels();
			TIMSK0 &= ~(1<<TOIE0);
			/* Print Data */
			usart_putstring("Stop\n\r");
			char str[10];
			int k;
			for(k=0;k<MAX_DATA;k++){
				itoa(vel[k],str,10);
				usart_putstring(str);
				usart_putchar(';');
			}
			usart_putstring("\n\rFinish\n\r");
		}
	}
}