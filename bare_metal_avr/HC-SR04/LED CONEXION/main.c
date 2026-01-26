#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
char str[80];
#include "serial.h"
#include "hcsr04.h"
#define BAUD 300

void init_UART2400(void) {
	UBRR0H = (uint8_t)(103 >> 8);
	UBRR0L = (uint8_t)(103);
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	//i = 0;
}

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

int main(void) {
	HCSR04_init();
	init_UART2400();
	while (1) {
		float HCSR04_data;

		// HCSR04
		HCSR04_data = medir_distancia_cm();

		// 60 ms de espera
		_delay_ms(600);

		sprintf(str, "D%.3f\r\n", HCSR04_data);
		usart0_puts(str);

		// Other tasks after the inner loop
	}

	return 0;
}
