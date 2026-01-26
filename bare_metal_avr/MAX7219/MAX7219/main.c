/*
 * PF_M_T2_IslasCamachoMariano.c
 *
 * Created: 21/06/2024 12:44:30 p. m.
 * Author : maria
 */

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>


// Definir una fuente de 5x7 para caracteres ASCII (32-127)
uint8_t font[96][5] = {
	{0x00, 0x00, 0x00, 0x00, 0x00}, // ' ' (espacio)
	{0x00, 0x00, 0x5F, 0x00, 0x00}, // '!'
	{0x00, 0x07, 0x00, 0x07, 0x00}, // '"'
	{0x14, 0x7F, 0x14, 0x7F, 0x14}, // '#'
	{0x24, 0x2A, 0x7F, 0x2A, 0x12}, // '$'
	{0x23, 0x13, 0x08, 0x64, 0x62}, // '%'
	{0x36, 0x49, 0x55, 0x22, 0x50}, // '&'
	{0x00, 0x05, 0x03, 0x00, 0x00}, // '''
	{0x00, 0x1C, 0x22, 0x41, 0x00}, // '('
	{0x00, 0x41, 0x22, 0x1C, 0x00}, // ')'
	{0x14, 0x08, 0x3E, 0x08, 0x14}, // '*'
	{0x08, 0x08, 0x3E, 0x08, 0x08}, // '+'
	{0x00, 0x50, 0x30, 0x00, 0x00}, // ','
	{0x08, 0x08, 0x08, 0x08, 0x08}, // '-'
	{0x00, 0x60, 0x60, 0x00, 0x00}, // '.'
	{0x20, 0x10, 0x08, 0x04, 0x02}, // '/'
	{0x3E, 0x51, 0x49, 0x45, 0x3E}, // '0'
	{0x00, 0x42, 0x7F, 0x40, 0x00}, // '1'
	{0x42, 0x61, 0x51, 0x49, 0x46}, // '2'
	{0x21, 0x41, 0x45, 0x4B, 0x31}, // '3'
	{0x18, 0x14, 0x12, 0x7F, 0x10}, // '4'
	{0x27, 0x45, 0x45, 0x45, 0x39}, // '5'
	{0x3C, 0x4A, 0x49, 0x49, 0x30}, // '6'
	{0x01, 0x71, 0x09, 0x05, 0x03}, // '7'
	{0x36, 0x49, 0x49, 0x49, 0x36}, // '8'
	{0x06, 0x49, 0x49, 0x29, 0x1E}, // '9'
	{0x00, 0x36, 0x36, 0x00, 0x00}, // ':'
	{0x00, 0x56, 0x36, 0x00, 0x00}, // ';'
	{0x08, 0x14, 0x22, 0x41, 0x00}, // '<'
	{0x14, 0x14, 0x14, 0x14, 0x14}, // '='
	{0x00, 0x41, 0x22, 0x14, 0x08}, // '>'
	{0x02, 0x01, 0x51, 0x09, 0x06}, // '?'
	{0x32, 0x49, 0x79, 0x41, 0x3E}, // '@'
	{0x7E, 0x11, 0x11, 0x11, 0x7E}, // 'A'
	{0x7F, 0x49, 0x49, 0x49, 0x36}, // 'B'
	{0x3E, 0x41, 0x41, 0x41, 0x22}, // 'C'
	{0x7F, 0x41, 0x41, 0x22, 0x1C}, // 'D'
	{0x7F, 0x49, 0x49, 0x49, 0x41}, // 'E'
	{0x7F, 0x09, 0x09, 0x09, 0x01}, // 'F'
	{0x3E, 0x41, 0x49, 0x49, 0x7A}, // 'G'
	{0x7F, 0x08, 0x08, 0x08, 0x7F}, // 'H'
	{0x00, 0x41, 0x7F, 0x41, 0x00}, // 'I'
	{0x20, 0x40, 0x41, 0x3F, 0x01}, // 'J'
	{0x7F, 0x08, 0x14, 0x22, 0x41}, // 'K'
	{0x7F, 0x40, 0x40, 0x40, 0x40}, // 'L'
	{0x7F, 0x02, 0x04, 0x02, 0x7F}, // 'M'
	{0x7F, 0x04, 0x08, 0x10, 0x7F}, // 'N'
	{0x3E, 0x41, 0x41, 0x41, 0x3E}, // 'O'
	{0x7F, 0x09, 0x09, 0x09, 0x06}, // 'P'
	{0x3E, 0x41, 0x51, 0x21, 0x5E}, // 'Q'
	{0x7F, 0x09, 0x19, 0x29, 0x46}, // 'R'
	{0x46, 0x49, 0x49, 0x49, 0x31}, // 'S'
	{0x01, 0x01, 0x7F, 0x01, 0x01}, // 'T'
	{0x3F, 0x40, 0x40, 0x40, 0x3F}, // 'U'
	{0x1F, 0x20, 0x40, 0x20, 0x1F}, // 'V'
	{0x3F, 0x40, 0x38, 0x40, 0x3F}, // 'W'
	{0x63, 0x14, 0x08, 0x14, 0x63}, // 'X'
	{0x07, 0x08, 0x70, 0x08, 0x07}, // 'Y'
	{0x61, 0x51, 0x49, 0x45, 0x43}, // 'Z'
	{0x00, 0x7F, 0x41, 0x41, 0x00}, // '['
	{0x02, 0x04, 0x08, 0x10, 0x20}, // '\'
	{0x00, 0x41, 0x41, 0x7F, 0x00}, // ']'
	{0x04, 0x02, 0x01, 0x02, 0x04}, // '^'
	{0x80, 0x80, 0x80, 0x80, 0x80}, // '_'
	{0x00, 0x03, 0x07, 0x00, 0x00}, // '`'
	{0x20, 0x54, 0x54, 0x54, 0x78}, // 'a'
	{0x7F, 0x44, 0x44, 0x44, 0x38}, // 'b'
	{0x38, 0x44, 0x44, 0x44, 0x44}, // 'c'
	{0x38, 0x44, 0x44, 0x44, 0x7F}, // 'd'
	{0x38, 0x54, 0x54, 0x54, 0x18}, // 'e'
	{0x04, 0x04, 0x7E, 0x05, 0x05}, // 'f'
	{0x08, 0x54, 0x54, 0x54, 0x3C}, // 'g'
	{0x7F, 0x08, 0x08, 0x08, 0x70}, // 'h'
	{0x00, 0x00, 0x7D, 0x00, 0x00}, // 'i'
	{0x40, 0x80, 0x80, 0x80, 0x7D}, // 'j'
	{0x7F, 0x10, 0x10, 0x28, 0x44}, // 'k'
	{0x00, 0x00, 0x7F, 0x00, 0x00}, // 'l'
	{0x7C, 0x04, 0x78, 0x04, 0x78}, // 'm'
	{0x7C, 0x08, 0x04, 0x04, 0x78}, // 'n'
	{0x38, 0x44, 0x44, 0x44, 0x38}, // 'o'
	{0xFC, 0x18, 0x24, 0x24, 0x18}, // 'p'
	{0x18, 0x24, 0x24, 0x18, 0xFC}, // 'q'
	{0x7C, 0x08, 0x04, 0x04, 0x08}, // 'r'
	{0x08, 0x54, 0x54, 0x54, 0x20}, // 's'
	{0x04, 0x04, 0x3F, 0x44, 0x44}, // 't'
	{0x3C, 0x40, 0x40, 0x40, 0x7C}, // 'u'
	{0x1C, 0x20, 0x40, 0x20, 0x1C}, // 'v'
	{0x3C, 0x40, 0x30, 0x40, 0x3C}, // 'w'
	{0x44, 0x28, 0x10, 0x28, 0x44}, // 'x'
	{0x04, 0x48, 0x30, 0x08, 0x04}, // 'y'
	{0x44, 0x64, 0x54, 0x4C, 0x44}, // 'z'
	{0x08, 0x36, 0x41, 0x00, 0x00}, // ''
	{0x00, 0x00, 0x7F, 0x00, 0x00}, // '|'
	{0x00, 0x00, 0x41, 0x36, 0x08}, // ''
	{0x08, 0x04, 0x08, 0x10, 0x08}  // '~'
};

char message7219[100] = "";  // Buffer para almacenar el mensaje recibido por UART
volatile uint8_t buffer7219[32] = {0};
volatile uint8_t currentChar7219 = 0;
volatile uint8_t charColumn7219 = 0;
volatile uint16_t totalColumns7219 = 0;
volatile uint8_t blankColumns7219 = 32;
volatile uint8_t newMessageReceived = 0;

void updateDisplay(void);
void SPI_init(void);
void SPI_write(uint8_t data);
void MAX7219_write(uint8_t address, uint8_t data);
void MAX7219_init(void);
void displayPattern(volatile uint8_t *pattern);
void changemessage7219(char *newmessage7219);
void Timer4_init(void);
void UART_init(void);
void UART_sendChar(char c);
char UART_receiveChar(void);
void UART_sendString(char *str);

ISR(TIMER4_COMPA_vect) {
	updateDisplay();
}

ISR(USART0_RX_vect) {
	static uint8_t index = 0;
	static char firstChar = '\0';  // Variable para almacenar el primer car cter recibido

	char receivedChar = UDR0;

	// Si es el primer car cter recibido
	if (index == 0) {
		firstChar = receivedChar;  // Almacena el primer car cter
		index++;  // Incrementa el  ndice para procesar el siguiente car cter
		return;  // Sale de la ISR para esperar el siguiente car cter
	}

	// Seg n el primer car cter recibido, ejecuta diferentes operaciones
	switch (firstChar) {
		case '$':
		// Operaciones para el caso '$'
		if (receivedChar == ';') {
			if (index > 1) {
				message7219[index - 1] = '\0';  // Termina el mensaje con nulo
				newMessageReceived = 1;  // Se aliza que se recibi  un nuevo mensaje
			}
			index = 0;  // Reinicia el  ndice para el pr ximo mensaje
			firstChar = '\0';  // Reinicia el primer car cter
			} else if (index <= 100) {  // Asegura que no se desborde el buffer
			message7219[index - 1] = receivedChar;  // Almacena el car cter recibido
			index++;  // Incrementa el  ndice para el pr ximo car cter
 			} else {
			// Puedes manejar aqu  el caso cuando el mensaje excede la longitud m xima permitida
			index = 0;  // Reinicia el  ndice por seguridad
			firstChar = '\0';  // Reinicia el primer car cter
		}
		break;

		case '#':
		// Operaciones para el caso '#'
		// Agrega aqu  las operaciones espec ficas para el primer car cter '#'
		break;

		default:
		// Manejo para otros casos o car cteres no esperados
		index = 0;  // Reinicia el  ndice
		firstChar = '\0';  // Reinicia el primer car cter
		break;
	}
}

void SPI_init(void) {
	DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB0);
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_write(uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
}

void MAX7219_write(uint8_t address, uint8_t data) {
	SPI_write(address);
	SPI_write(data);
}

void MAX7219_init(void) {
	PORTB &= ~(1 << PB0);
	for(uint8_t i = 0; i < 4; i++) {
		MAX7219_write(0x0A, 0x0F);
	}
	PORTB |= (1 << PB0);
	PORTB &= ~(1 << PB0);
	for(uint8_t i = 0; i < 4; i++) {
		MAX7219_write(0x0B, 0x07);
	}
	PORTB |= (1 << PB0);
	PORTB &= ~(1 << PB0);
	for(uint8_t i = 0; i < 4; i++) {
		MAX7219_write(0x0C, 0x01);
	}
	PORTB |= (1 << PB0);
	PORTB &= ~(1 << PB0);
	for(uint8_t i = 0; i < 4; i++) {
		MAX7219_write(0x0F, 0x00);
	}
	PORTB |= (1 << PB0);
}

void displayPattern(volatile uint8_t *pattern) {
	for (uint8_t row = 0; row < 8; row++) {
		PORTB &= ~(1 << PB0);
		for (uint8_t dev = 0; dev < 4; dev++) {
			MAX7219_write(0x01 + row, pattern[row + dev * 8]);
		}
		PORTB |= (1 << PB0);
	}
}

void Timer4_init(void) {
	TCCR4B |= (1 << WGM12);
	TIMSK4 |= (1 << OCIE4A);
	OCR4A = 24999;
	TCCR4B |= (1 << CS41) | (1 << CS40);
}

void UART_init(void) {
	uint16_t ubrr = F_CPU/16/9600-1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void updateDisplay(void) {
	// Check if all characters in the message have been displayed and there are still blank columns left
	if (currentChar7219 >= strlen(message7219) && blankColumns7219 > 0) {
		// Shift the buffer columns to the left
		blankColumns7219--;
		for (uint8_t row = 0; row < 8; row++) {
			for (uint8_t dev = 0; dev < 4; dev++) {
				buffer7219[row + dev * 8] <<= 1;
				// If it's not the last device column and the next column has a 1 in the MSB, set LSB of the current column
				if (dev < 3 && (buffer7219[row + (dev + 1) * 8] & 0x80)) {
					buffer7219[row + dev * 8] |= 1;
				}
			}
		}
		// Display the updated pattern on the LED matrix
		displayPattern(buffer7219);
		return;
	}
	
	// Check if all characters have been displayed, if so reset for next message
	else if (currentChar7219 >= strlen(message7219)) {
		currentChar7219 = 0;       // Reset current character index
		charColumn7219 = 0;        // Reset column index within the character
		blankColumns7219 = 32;     // Reset blank columns counter
		return;
	}
	
	// Check if current character is a non-printable ASCII character
	if (message7219[currentChar7219] < 32 || message7219[currentChar7219] > 127) {
		currentChar7219++;         // Skip non-printable characters
		charColumn7219 = 0;        // Reset column index within the character
		return;
	}
	
	// Shift the buffer columns to the left
	for (uint8_t row = 0; row < 8; row++) {
		for (uint8_t dev = 0; dev < 4; dev++) {
			buffer7219[row + dev * 8] <<= 1;
			// If it's not the last device column and the next column has a 1 in the MSB, set LSB of the current column
			if (dev < 3 && (buffer7219[row + (dev + 1) * 8] & 0x80)) {
				buffer7219[row + dev * 8] |= 1;
			}
		}
		// Check if current column index is within the width of the character (5 columns)
		if (charColumn7219 < 5) {
			// Set the LSB of the 4th device column if the current pixel in the font array is set
			if (font[message7219[currentChar7219] - 32][charColumn7219] & (1 << row)) {
				buffer7219[row + 24] |= 1;
				} else {
				buffer7219[row + 24] &= ~1;
			}
			} else {
			buffer7219[row + 24] &= ~1; // Clear the LSB of the 4th device column
		}
	}
	
	// Display the updated pattern on the LED matrix
	displayPattern(buffer7219);
	
	// Move to the next column of the current character
	charColumn7219++;
	totalColumns7219++;
	
	// If all columns of the current character have been displayed, move to the next character
	if (charColumn7219 > 5) {
		charColumn7219 = 0;
		currentChar7219++;
	}
}

int main(void) {
	SPI_init();
	MAX7219_init();
	Timer4_init();
	UART_init();
	sei();  // Habilitar interrupciones globales
	while (1) {
		// Bucle principal
	}
}