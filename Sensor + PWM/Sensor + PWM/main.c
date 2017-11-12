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
#define MAX_DATA 30

/* Motor */
#define step .1
#define time 50

#include <util/delay.h>

/* Duty Cycle in percent */
double dutyCycle = 99;

/* Time taken from HIGH to LOW on echo */
static volatile int pulse = 0;

/* Identifier of HIGH / LOW on echo */
static volatile int i = 0;

/* Stores timer overflow */
static volatile int timer = 0;

/* Stores PWM interruption */
static volatile direction = 0;

/* Counts number of interruptions */
static volatile Int_Counter = 0;

/* Data */
int dist[MAX_DATA];

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
}

void Timer_Init(){
	/* Prescaler timer */
	TCCR0A |= ((1<<WGM00)|(1<<COM0A1)|(1<<WGM01));
	
	/* Enable timer interrupt */
	//TIMSK0 = (1<<TOIE0);
	
	/* Set dutyCycle in PWM */
	OCR0A = (dutyCycle/100)*255;
	
	/* Dont remember what is this */
	TCCR0B = (1<<CS00);
	
	TCCR2A = 0;
	
	/* Initializing timer counter 2 */
	TCCR2B = (1 << CS20)|(1 << CS21)|(1 << CS22);
	
	/* Enable Interruption */
	TIMSK2 = (1<<TOIE2);
	
	/* Starts clock 2 */
	TCNT2 = 0;
}

void Sensor_Init(){
	/* Trigger OUTPUT*/
	DDRD |= (1<<DDD7);
	/* Echo INPUT */
	DDRD &= ~(1<<DDD2);

	_delay_ms(50);

	EIMSK|=(1<<INT0);//enabling interrupt0

	EICRA |=(1<<ISC00);//setting interrupt triggering logic change
	
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

int j = 0;

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
	
	/* Sensor Init */
	Sensor_Init();
	
	/* Button Init */
	Button_Init();
	Button_Interrupt_Init();
	
	/* USART Init */
	USART_Init(MYUBRR);
	
	/* Starts moving forward */
	Forward_Wheels();
	
	/* Enable Interrupt */
	sei();
	
	int16_t COUNTA = 0;//storing digital output

	char SHOWA [3];
	
	/* Super Loop */
	while(1)
	{
		/* Activates sonar */
		/* Trigger Enable */
		PORTD|=(1<<PIND7);

		_delay_us(15);

		/* Trigger Disable */
		PORTD &=~(1<<PIND7);
		
		/* Calculates distance */
		COUNTA = pulse/(58*2);
		
		dist[j] = COUNTA;
		
		j++;
	
		_delay_ms(200);
	}
	
}

/* Motor PWM */
ISR(TIMER2_OVF_vect){
	
	/* Increments interruption counter variable */
	Int_Counter ++;
	
	/* Adjusts changes to occur every x seconds, in this case : 800 / 61 = 13 seconds */
	if(Int_Counter == 200){
		if(direction == 1){
			Stop_Wheels();
			direction = -1;
		}
		if(direction == 0){
			Backward_Wheels();
			direction = 1;
		}
	Int_Counter = 0;
	}
	//i++;
}

ISR(INT0_vect)//interrupt service routine when there is a change in logic level

{

	if (i==1)//when logic from HIGH to LOW

	{

		TCCR1B=0;//disabling counter

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
		PORTD ^= (1<<PORTD6);
	}
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
				itoa(dist[k],str,10);
				usart_putstring(str);
				usart_putchar(';');
			}
			usart_putstring("\n\rFinish\n\r");
		}
	}
}
