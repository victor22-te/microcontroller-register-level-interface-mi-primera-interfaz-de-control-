/*

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD/lcd.h"

#define I2C_READ 1
#define I2C_WRITE 0
#define MLX90614_ADDRESS 0x5A

// Funciones para I2C
void i2c_init(void) {
	TWCR = (1<<TWEN);//HABILITA I2C
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0));//TWSR DETERMINA PREESCALADOR -> PRESCALADOR DE 1
	TWBR = 3; //PARA LA FRECUENCIA DE 400KHz -> (F_CPU/SCL-16)/(2)(4)^1* /
}

void i2c_start(void) {
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
}

void i2c_stop(void) {
	TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT);
}

void i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read_ack(void) {
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack(void) {
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

float read_temperature_mlx90614(uint8_t reg) {
	uint8_t lsb, msb;
	float temperature;

	i2c_start();
	i2c_write((MLX90614_ADDRESS << 1) | I2C_WRITE);
	i2c_write(reg);
	i2c_start();
	i2c_write((MLX90614_ADDRESS << 1) | I2C_READ);

	lsb = i2c_read_ack();
	msb = i2c_read_nack();
	i2c_stop();

	temperature = ((msb << 8) | lsb) * 0.02 - 273.15;

	// Manejo básico de errores
	if (temperature < -273.15 || temperature > 1000.0) {
		temperature = -999.0; // Valor fuera de rango para indicar error
	}

	return temperature;
}

// USART initialization function
void init_USART(void) {
	// Set baud rate to 9600
	UBRR0H = (uint8_t)(103 >> 8);
	UBRR0L = (uint8_t)(103);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function to send a character over USART
void USART_sendChar(char buffer) {
	// Wait until the transmitter is ready
	while (!(UCSR0A & (1 << UDRE0)));

	// Send the data
	UDR0 = buffer;
}

// Function to send a string over USART
void USART_puts(const char* buffer) {
	while (*buffer) {
		USART_sendChar(*buffer++);
	}
}

int main(void) {
	float temperature_obj, temperature_ambient;
	char buffer[50];

	// Inicializar I2C y USART
	i2c_init();
	init_USART();

	while (1) {
		// Leer temperatura del objeto del sensor MLX90614 (registro 0x07)
		temperature_obj = read_temperature_mlx90614(0x07);

		// Leer temperatura ambiente del sensor MLX90614 (registro 0x06)
		temperature_ambient = read_temperature_mlx90614(0x06);

		// Verificar si la lectura del sensor fue exitosa antes de mostrar
		if (temperature_obj != -999.0 && temperature_ambient != -999.0) {
			dtostrf(temperature_obj, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Obj: %.2f C\n", temperature_obj); // Mostrar temperatura del objeto en el monitor serial
			USART_puts(buffer);
			_delay_ms(1000);

			dtostrf(temperature_ambient, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Amb: %.2f C\n", temperature_ambient); // Mostrar temperatura ambiente en el monitor serial
			USART_puts(buffer);

			_delay_ms(500); // Esperar medio segundo antes de la siguiente lectura
			} else {
			USART_puts("Error en la lectura del sensor\n");
			_delay_ms(500);
		}
	}
}
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD/lcd.h"

#define I2C_READ 1
#define I2C_WRITE 0
#define MLX90614_ADDRESS 0x5A

// Funciones para I2C
void i2c_init(void) {
	TWCR = (1<<TWEN); // Habilita I2C
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); // Preescalador de 1
	TWBR = 72; // Para una frecuencia de 100KHz
}

int i2c_start(void) {
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08) {
		return -1; // Error en la condición START
	}
	return 0;
}

void i2c_stop(void) {
	TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT);
}

int i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x28) {
		return -1; // Error en la escritura del dato
	}
	return 0;
}

int i2c_read_ack(uint8_t *data) {
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x50) {
		return -1; // Error en la lectura con ACK
	}
	*data = TWDR;
	return 0;
}

int i2c_read_nack(uint8_t *data) {
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x58) {
		return -1; // Error en la lectura sin ACK
	}
	*data = TWDR;
	return 0;
}

int read_temperature_mlx90614(uint8_t reg, float *temperature) {
	uint8_t lsb, msb;

	if (i2c_start() < 0) return -1;
	if (i2c_write((MLX90614_ADDRESS << 1) | I2C_WRITE) < 0) return -1;
	if (i2c_write(reg) < 0) return -1;
	if (i2c_start() < 0) return -1;
	if (i2c_write((MLX90614_ADDRESS << 1) | I2C_READ) < 0) return -1;

	if (i2c_read_ack(&lsb) < 0) return -1;
	if (i2c_read_nack(&msb) < 0) return -1;
	i2c_stop();

	*temperature = ((msb << 8) | lsb) * 0.02 - 273.15;

	// Manejo básico de errores
	if (*temperature < -273.15 || *temperature > 1000.0) {
		*temperature = -999.0; // Valor fuera de rango para indicar error
	}

	return 0;
}

// USART initialization function
void init_USART(void) {
	// Set baud rate to 9600
	UBRR0H = (uint8_t)(103 >> 8);
	UBRR0L = (uint8_t)(103);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function to send a character over USART
void USART_sendChar(char buffer) {
	// Wait until the transmitter is ready
	while (!(UCSR0A & (1 << UDRE0)));

	// Send the data
	UDR0 = buffer;
}

// Function to send a string over USART
void USART_puts(const char* buffer) {
	while (*buffer) {
		USART_sendChar(*buffer++);
	}
}

int main(void) {
	float temperature_obj, temperature_ambient;
	char buffer[50];

	// Inicializar I2C y USART
	i2c_init();
	init_USART();

	while (1) {
		// Leer temperatura del objeto del sensor MLX90614 (registro 0x07)
		if (read_temperature_mlx90614(0x07, &temperature_obj) < 0) {
			USART_puts("Error: Sensor no encontrado (0x07)\n");
			_delay_ms(500);
			continue;
		}

		// Leer temperatura ambiente del sensor MLX90614 (registro 0x06)
		if (read_temperature_mlx90614(0x06, &temperature_ambient) < 0) {
			USART_puts("Error: Sensor no encontrado (0x06)\n");
			_delay_ms(500);
			continue;
		}

		// Verificar si la lectura del sensor fue exitosa antes de mostrar
		if (temperature_obj != -999.0 && temperature_ambient != -999.0) {
			dtostrf(temperature_obj, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Obj: %.2f C\n", temperature_obj); // Mostrar temperatura del objeto en el monitor serial
			USART_puts(buffer);
			_delay_ms(1000);

			dtostrf(temperature_ambient, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Amb: %.2f C\n", temperature_ambient); // Mostrar temperatura ambiente en el monitor serial
			USART_puts(buffer);

			_delay_ms(500); // Esperar medio segundo antes de la siguiente lectura
			} else {
			USART_puts("Error en la lectura del sensor\n");
			_delay_ms(500);
		}
	}
}
*/
/*
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD/lcd.h"

#define I2C_READ 1
#define I2C_WRITE 0
#define MLX90614_ADDRESS 0x5A

// Funciones para I2C
void i2c_init(void) {
	TWCR = (1<<TWEN); // Habilita I2C
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); // Preescalador de 1
	TWBR = 72; // Para una frecuencia de 100KHz
}

int i2c_start(void) {
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08 && (TWSR & 0xF8) != 0x10) {
		return -1; // Error en la condición START o RESTART
	}
	return 0;
}

void i2c_stop(void) {
	TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT);
}

int i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x18 && (TWSR & 0xF8) != 0x28) {
		return -1; // Error en la escritura del dato con ACK o NACK
	}
	return 0;
}

int i2c_read_ack(uint8_t *data) {
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x50) {
		return -1; // Error en la lectura con ACK
	}
	*data = TWDR;
	return 0;
}

int i2c_read_nack(uint8_t *data) {
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x58) {
		return -1; // Error en la lectura sin ACK
	}
	*data = TWDR;
	return 0;
}

int read_temperature_mlx90614(uint8_t reg, float *temperature) {
	uint8_t lsb, msb;

	if (i2c_start() < 0) return -1;
	if (i2c_write((MLX90614_ADDRESS << 1) | I2C_WRITE) < 0) return -1;
	if (i2c_write(reg) < 0) return -1;
	if (i2c_start() < 0) return -1;
	if (i2c_write((MLX90614_ADDRESS << 1) | I2C_READ) < 0) return -1;

	if (i2c_read_ack(&lsb) < 0) return -1;
	if (i2c_read_nack(&msb) < 0) return -1;
	i2c_stop();

	*temperature = ((msb << 8) | lsb) * 0.02 - 273.15;

	// Manejo básico de errores
	if (*temperature < -273.15 || *temperature > 1000.0) {
		*temperature = -999.0; // Valor fuera de rango para indicar error
		return -1; // Error de lectura de temperatura fuera de rango
	}

	return 0;
}

// USART initialization function
void init_USART(void) {
	// Set baud rate to 9600
	UBRR0H = (uint8_t)(103 >> 8);
	UBRR0L = (uint8_t)(103);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function to send a character over USART
void USART_sendChar(char buffer) {
	// Wait until the transmitter is ready
	while (!(UCSR0A & (1 << UDRE0)));

	// Send the data
	UDR0 = buffer;
}

// Function to send a string over USART
void USART_puts(const char* buffer) {
	while (*buffer) {
		USART_sendChar(*buffer++);
	}
}

int main(void) {
	float temperature_obj, temperature_ambient;
	char buffer[50];

	// Inicializar I2C y USART
	i2c_init();
	init_USART();

	while (1) {
		// Leer temperatura del objeto del sensor MLX90614 (registro 0x07)
		if (read_temperature_mlx90614(0x07, &temperature_obj) < 0) {
			USART_puts("Error: Sensor no encontrado (0x07)\n");
			_delay_ms(500);
			continue;
		}

		// Leer temperatura ambiente del sensor MLX90614 (registro 0x06)
		if (read_temperature_mlx90614(0x06, &temperature_ambient) < 0) {
			USART_puts("Error: Sensor no encontrado (0x06)\n");
			_delay_ms(500);
			continue;
		}

		// Verificar si la lectura del sensor fue exitosa antes de mostrar
		if (temperature_obj != -999.0 && temperature_ambient != -999.0) {
			dtostrf(temperature_obj, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Obj: %.2f C\n", temperature_obj); // Mostrar temperatura del objeto en el monitor serial
			USART_puts(buffer);
			_delay_ms(1000);

			dtostrf(temperature_ambient, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Amb: %.2f C\n", temperature_ambient); // Mostrar temperatura ambiente en el monitor serial
			USART_puts(buffer);

			_delay_ms(500); // Esperar medio segundo antes de la siguiente lectura
			} else {
			USART_puts("Error en la lectura del sensor\n");
			_delay_ms(500);
		}
	}
}*/


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD/lcd.h"

#define I2C_READ 1
#define I2C_WRITE 0
#define MLX90614_ADDRESS 0x5A

// Funciones para I2C
void i2c_init(void) {
	TWCR = (1<<TWEN); // Habilita I2C
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); // Preescalador de 1
	TWBR = 72; // Para una frecuencia de 100KHz
}

int i2c_start(void) {
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08 && (TWSR & 0xF8) != 0x10) {
		return -1; // Error en la condición START o RESTART
	}
	return 0;
}

void i2c_stop(void) {
	TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT);
}

int i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x18 && (TWSR & 0xF8) != 0x28) {
		return -1; // Error en la escritura del dato con ACK o NACK
	}
	return 0;
}

int i2c_read_ack(uint8_t *data) {
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x50) {
		return -1; // Error en la lectura con ACK
	}
	*data = TWDR;
	return 0;
}

int i2c_read_nack(uint8_t *data) {
	TWCR = (1<<TWEN) | (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x58) {
		return -1; // Error en la lectura sin ACK
	}
	*data = TWDR;
	return 0;
}

int read_temperature_mlx90614(uint8_t reg, float *temperature) {
	uint8_t lsb, msb;

	if (i2c_start() < 0) return -1;
	if (i2c_write((MLX90614_ADDRESS << 1) | I2C_WRITE) < 0) return -1;
	if (i2c_write(reg) < 0) return -1;
	if (i2c_start() < 0) return -1;
	if (i2c_write((MLX90614_ADDRESS << 1) | I2C_READ) < 0) return -1;

	if (i2c_read_ack(&lsb) < 0) return -1;
	if (i2c_read_nack(&msb) < 0) return -1;
	i2c_stop();

	*temperature = ((msb << 8) | lsb) * 0.02 - 273.15;

	// Manejo básico de errores
	if (*temperature < -273.15 || *temperature > 1000.0) {
		*temperature = -999.0; // Valor fuera de rango para indicar error
		return -1; // Error de lectura de temperatura fuera de rango
	}

	return 0;
}

// USART initialization function
void init_USART(void) {
	// Set baud rate to 9600
	UBRR0H = (uint8_t)(103 >> 8);
	UBRR0L = (uint8_t)(103);

	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function to send a character over USART
void USART_sendChar(char buffer) {
	// Wait until the transmitter is ready
	while (!(UCSR0A & (1 << UDRE0)));

	// Send the data
	UDR0 = buffer;
}

// Function to send a string over USART
void USART_puts(const char* buffer) {
	while (*buffer) {
		USART_sendChar(*buffer++);
	}
}

int main(void) {
	float temperature_obj, temperature_ambient;
	char buffer[50];

	// Inicializar I2C y USART
	i2c_init();
	init_USART();

	while (1) {
		// Leer temperatura del objeto del sensor MLX90614 (registro 0x07)
		if (read_temperature_mlx90614(0x07, &temperature_obj) < 0) {
			USART_puts("Error: Sensor no encontrado (0x07)\n");
			_delay_ms(500);
			continue;
		}

		// Leer temperatura ambiente del sensor MLX90614 (registro 0x06)
		if (read_temperature_mlx90614(0x06, &temperature_ambient) < 0) {
			USART_puts("Error: Sensor no encontrado (0x06)\n");
			_delay_ms(500);
			continue;
		}

		// Verificar si la lectura del sensor fue exitosa antes de mostrar
		if (temperature_obj != -999.0 && temperature_ambient != -999.0) {
			dtostrf(temperature_obj, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Obj: %.2f C\n", temperature_obj); // Mostrar temperatura del objeto en el monitor serial
			USART_puts(buffer);
			_delay_ms(1000);

			dtostrf(temperature_ambient, 6, 2, buffer); // Convertir temperatura a cadena con 2 decimales
			sprintf(buffer, "Temp Amb: %.2f C\n", temperature_ambient); // Mostrar temperatura ambiente en el monitor serial
			USART_puts(buffer);

			_delay_ms(500); // Esperar medio segundo antes de la siguiente lectura
			} else {
			USART_puts("Error en la lectura del sensor\n");
			_delay_ms(500);
		}
	}
}
