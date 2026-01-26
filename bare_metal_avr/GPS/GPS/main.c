#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void USART0_init(void) {
	// Set baud rate
	UBRR0 = 103;
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable RX complete interrupt
	// Set frame format: 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART3_init(void) {
	// Set baud rate
	UBRR3 = 103;
	// Enable receiver and transmitter
	UCSR3B = (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3); // Enable RX complete interrupt
	// Set frame format: 8 data bits, 1 stop bit
	UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
}

ISR(USART3_RX_vect) {
	static char buffer[128];
	static uint8_t buffer_index = 0;
	unsigned char data = UDR3;

	if (data == '\n' || buffer_index >= 127) {
		buffer[buffer_index] = '\0'; // Terminate buffer with null character
		process_gps_data(buffer);
		buffer_index = 0; // Reset buffer index for next message
		} else {
		buffer[buffer_index++] = data;
	}
}

void USART0_transmit(unsigned char data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = data;
}

void USART0_print(const char* str) {
	while (*str) {
		while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
		UDR0 = *str++; // Put data into buffer, sends the data
	}
}

void process_gps_data(char* buffer) {
	if (strncmp(buffer, "$GPRMC,",7) == 0) {
		char *token;
		char *ptr = buffer;
		int field_count = 0;
		
		// Parse comma-separated values
		while ((token = strtok_r(ptr, ",", &ptr)) != NULL && field_count < 12) {
			switch (field_count) {
				case 1: // Time (hhmmss.sss)
				if (strlen(token) >= 6) {
					// Extract hours, minutes, seconds
					char hours[3], minutes[3], seconds[5];
					strncpy(hours, token, 2);
					hours[2] = '\0';
					strncpy(minutes, token + 2, 2);
					minutes[2] = '\0';
					strncpy(seconds, token + 4, 5);
					seconds[4] = '\0';

					// Print formatted time
					USART0_print("Hora: ");
					USART0_print(hours);
					USART0_print(":");
					USART0_print(minutes);
					USART0_print(":");
					USART0_print(seconds);
					USART0_print("\r\n");
				}
				break;
				case 2: // Status (A)
				USART0_print("Estado: ");
				if (strcmp(token, "A") == 0) {
					USART0_print("OK\r\n");
					} else if (strcmp(token, "V") == 0) {
					USART0_print("Warning\r\n");
					} else {
					USART0_print(token); // Print as-is if not A or V
					USART0_print("\r\n");
				}
				break;
				case 3: // Latitude (ddmm.mmmm)
				if (strlen(token) >= 7) {
					// Extract degrees, minutes
					char degrees[3], minutes[3], seconds[3],dsec[3];
					strncpy(degrees, token, 2);
					degrees[2] = '\0';
					strncpy(minutes, token + 2, 2);  // Cambiado a +3
					minutes[2] = '\0';
					strncpy(seconds, token + 5, 2);  // Cambiado a +5
					seconds[2] = '\0';
					strncpy(dsec, token + 7, 2);  // Cambiado a +5
					dsec[2] = '\0';
					
					// Print formatted latitude
					USART0_print("Latitud: ");
					USART0_print(degrees);
					USART0_print("\xC2\xB0");
					USART0_print(minutes);
					USART0_transmit(39);  // Imprime el s mbolo de comillas simples (')
					USART0_print(seconds);
					USART0_print(".");
					USART0_print(dsec);
					USART0_transmit(34);  // Imprime el s mbolo de comillas dobles (")
				}
				break;
				case 4: // N/S
				USART0_print(token);
				USART0_print("\r\n");
				break;
				case 5: // Longitude (dddmm.mmmm)
				if (strlen(token) >= 7) {
					// Extraer grados, minutos y segundos
					char degrees[4], minutes[3], seconds[3],dsec[3];
					strncpy(degrees, token, 3);
					degrees[3] = '\0';
					strncpy(minutes, token + 3, 2);  // Cambiado a +3
					minutes[2] = '\0';
					strncpy(seconds, token + 6, 2);  // Cambiado a +5
					seconds[2] = '\0';
					strncpy(dsec, token + 8, 2);  // Cambiado a +5
					dsec[2] = '\0';
					

					// Imprimir longitud formateada
					USART0_print("Longitude: ");
					USART0_print(degrees);
					USART0_print("\xC2\xB0");
					USART0_print(minutes);
					USART0_transmit(39);  // Imprime el s mbolo de comillas simples (')
					USART0_print(seconds);
					USART0_print(".");
					USART0_print(dsec);
					USART0_transmit(34);  // Imprime el s mbolo de comillas dobles (")
				}

				break;
				case 6: // E/W
				USART0_print(token);
				USART0_print("\r\n");
				break;
				case 7: // Speed (spd)
				USART0_print("Velocidad: ");
				USART0_print(token);
				USART0_print(" nudos\r\n");
				break;
				case 8: // Date (ddmmyy)
				if (strlen(token) >= 4) {
					// Extract hours, minutes, seconds
					char days[3], mon[3], yea[3];
					strncpy(days, token, 2);
					days[2] = '\0';
					strncpy(mon, token + 2, 2);
					mon[2] = '\0';
					strncpy(yea, token + 4, 2);
					yea[2] = '\0';

					// Print formatted date
					USART0_print("Fecha : ");
					USART0_print(days);
					USART0_print("/");
					USART0_print(mon);
					USART0_print("/");
					USART0_print(yea);
					USART0_print("\r\n");
				}
				break;

				case 9: // Magnetic variation (mm.m)
				if (strlen(token) >= 4) {
					// Extract magnetic variation
					char mag_variation[4];
					strncpy(mag_variation, token, 3);
					mag_variation[3] = '\0';

					// Print formatted magnetic variation
					USART0_print("Variacion Magnetica: ");
					USART0_print(mag_variation);
					USART0_print(" grados\r\n");
				}
				break;
				default:
				break;
			}
			field_count++;
		}
	}
}

int main(void) {
	USART3_init();
	USART0_init();
	
	sei(); // Enable global interrupts
	
	while (1) {
		// Main loop can be empty or used for other tasks
	}
	
	return 0;
}