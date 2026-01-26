#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

float V, T;
char data[32];
uint8_t i = 0;

// USART initialization function
void init_USART(void) {
	// Set baud rate to 2400
	UBRR0H = (uint8_t)(416 >> 8);
	UBRR0L = (uint8_t)(416);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	i = 0;
}
void init_ADC(void) {
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX |= (1 << REFS0);
}
uint16_t read_adc(void) {
	// Set the MUX bits to select channel F0
	ADMUX = (ADMUX & 0xF0) | (0x00);

	ADCSRA |= (1 << ADSC);              // Start the conversion
	while ((ADCSRA & (1 << ADSC)));     // Wait until the conversion is complete
	return ADC;                         // Read the ADC value
}
/*
// USART initialization function
void init_USART(void) {
	// Set baud rate to 9600
	UBRR0H = (uint8_t)(103 >> 8);
	UBRR0L = (uint8_t)(103);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}*/
// Function to send a character over USART
void USART_sendChar(char data) {
	// Wait until the transmitter is ready
	while (!(UCSR0A & (1 << UDRE0)));
	// Send the data
	UDR0 = data;
}

// Function to send a string over USART
void USART_puts(const char* data) {
	while (*data) {
		USART_sendChar(*data++);
	}
}

int main(void) {
	init_ADC();
	init_USART();

	while (1) {
		if (i==0){
		uint16_t adcValue = read_adc();
		//V = (((float)adcValue * 15) / 1024);
		//T = V * 100.0;
		V= (((float)adcValue * 5000) / 1024);
		T=V/10;
		
		

		sprintf(data, "D%.1f\n", T);
		USART_puts(data);
		_delay_ms(1000);
		}
	}
}