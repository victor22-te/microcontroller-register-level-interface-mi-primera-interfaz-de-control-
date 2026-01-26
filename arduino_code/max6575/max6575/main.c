#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define LED_PIN PB7  // Declarar el pin del LED

char print[32];

#define CS1_HIGH PORTB |= (1 << PORTB0)
#define CS1_LOW  PORTB &= ~(1 << PORTB0)

// Configuración de SPI
void init_spi(void) {
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);  // CK, MOSI, CS
	PORTB |= (1 << PORTB0); // CS HIGH
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_write(uint8_t d) {
	SPDR = d;
	while (!(SPSR & (1 << SPIF)));
}

uint8_t spi_read(void) {
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

// Configuración de UART
void init_usart0(void) {
	UBRR0 = 103;  // BAUD RATE = 9600 bps
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // 8-BIT
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   // ENABLE TX, RX
}

void usart_putc(char x) {
	while (!(UCSR0A & (1 << UDRE0))); // Espero a poder enviar un dato
	UDR0 = x;  // Enviar un dato
}

void usart_puts(char *s) {
	uint8_t i = 0;
	while (s[i] != '\0') {
		usart_putc(s[i]);
		i++;
	}
}

int main(void) {
	init_usart0();
	init_spi();
	// Declarar el pin del LED como salida
	DDRB |= (1 << LED_PIN);
	usart_puts("SERIAL ACTIVADO\n");
	uint16_t data;
	while (1) {
		CS1_LOW;
		spi_write(0x00);
		data = spi_read();
		spi_write(0x00);
		data = (data << 8) | spi_read();
		data &= 0x0fff;
		data = (data >> 3);
		float n = (data + 1) / 2;  // Calcular el valor de conversión
		CS1_HIGH;
		sprintf(print, "D2: %.1f\r\n", n);
		usart_puts(print);
		_delay_ms(1000);
	}
}
