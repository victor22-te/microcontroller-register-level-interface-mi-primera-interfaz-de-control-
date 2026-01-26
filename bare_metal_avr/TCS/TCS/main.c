#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char str[32];
volatile uint16_t t3;

void USART_Init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void usart_puts(const char *s) {
	while (*s) {
		USART_Transmit(*s);
		s++;
	}
}

void init_timer5(void) {
	TCCR5A = 0x00; // Modo normal para Timer 5
	TCCR5B = 0x00;
	TCCR5B |= (1 << WGM52) | (1 << CS51); // Preescalador de 8
	OCR5A = 3;
	TCNT5 = 0; // Inicializar el contador
	TIMSK5 |= (1 << OCIE5A); // Habilita la interrupción por comparación en A
	sei(); // Habilitar interrupciones globales
}

void init_TCS3200(void) {
	DDRK |= (1 << DDK0) | (1 << DDK1) | (1 << DDK2) | (1 << DDK3) | (1 << DDK5); // S0, S1, S2, S3, LED
	DDRK &= ~(1 << DDK4); // OUT

	PORTK |= (1 << PORTK0) | (1 << PORTK1); // S0_PIN y S1_PIN HIGH

	init_timer5();
}

int CLR_ROJO(void) {
	PORTK &= ~(1 << PORTK2); // S2_PIN LOW
	PORTK &= ~(1 << PORTK3); // S3_PIN LOW

	while (!(PINK & (1 << PINK4))); // Espera a que el pulso sea bajo
	t3 = 0;
	TIMSK5 |= (1 << OCIE5A);

	while (PINK & (1 << PINK4)); // Espera a que el pulso sea alto
	return t3;
}

int CLR_BLUE(void) {
	PORTK &= ~(1 << PORTK2); // S2_PIN LOW
	PORTK |= (1 << PORTK3); // S3_PIN HIGH

	while (!(PINK & (1 << PINK4))); // Espera a que el pulso sea bajo
	t3 = 0;
	TIMSK5 |= (1 << OCIE5A);

	while (PINK & (1 << PINK4)); // Espera a que el pulso sea alto
	return t3;
}

uint16_t CLR_GREEN(void) {
	PORTK |= (1 << PORTK2); // S2_PIN HIGH
	PORTK |= (1 << PORTK3); // S3_PIN HIGH

	while (!(PINK & (1 << PINK4))); // Espera a que el pulso sea bajo
	t3 = 0;
	TIMSK5 |= (1 << OCIE5A);

	while (PINK & (1 << PINK4)); // Espera a que el pulso sea alto
	return t3;
}

int main(void) {
	// Inicializar UART
	USART_Init(103); // Baud rate = 9600 bps para F_CPU = 16MHz

	init_TCS3200();
	PORTK |= (1 << PORTK5);

	while (1) {
		// Formatear y enviar los resultados por UART
		sprintf(str,"R%u\nB%u\nG%u\n", CLR_ROJO(), CLR_BLUE(), CLR_GREEN());
		usart_puts(str);
		_delay_ms(500);
	}
}

ISR(TIMER5_COMPA_vect) {
	t3 += 1;
}
